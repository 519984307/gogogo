

#include <net/netlink.h>
#include <net/sock.h>
#include <linux/version.h>
#include <linux/skbuff.h>
#include <linux/list.h>
#include "debug.h"
#include "ukcom.h"

#define MAX_CHANNEL 200

//FIXME: use list?
struct channel {
	int valid;
	struct semaphore sem;
	void *msg;
	int msg_len;
};


static struct channel channels[MAX_CHANNEL];
static DEFINE_MUTEX(channel_mutex);

static struct channel *__get_channel(int seq)
{
	struct channel *ch;

	if (seq < 0 || seq > MAX_CHANNEL)
		return NULL;

	mutex_lock(&channel_mutex);
	ch = (channels[seq].valid == 0) ? NULL : &channels[seq];
	channels[seq].valid++;
	mutex_unlock(&channel_mutex);
	return ch;
}

static void __free_seq(int seq)
{
	if (seq < 0 || seq > MAX_CHANNEL)
		return;
	mutex_lock(&channel_mutex);
	if (channels[seq].valid == 0) {
		__log_error("seq:%d valid == 0\r\n", seq);
	}
	channels[seq].valid--;
	if (channels[seq].valid == 0) {
		channels[seq].msg = NULL;
		channels[seq].msg_len = 0;
		sema_init(&channels[seq].sem, 1);
	}
	mutex_unlock(&channel_mutex);
}

static int __alloc_seq(void *msg, int msg_len)
{
	int i;
	int seq = -1;
	struct channel *ch = NULL;

	mutex_lock(&channel_mutex);

	for (i = 0; i < MAX_CHANNEL; i++) {
		if (channels[i].valid == 0) {
			channels[i].valid = 1;
			channels[i].msg = msg;
			channels[i].msg_len = msg_len;
			seq = i;
			ch = &channels[seq];
			break;
		}
	}

	mutex_unlock(&channel_mutex);

	if (ch != NULL) {
		down(&ch->sem);
	}

	return seq;
}

static void __init_seq(void)
{
	int i;

	for (i = 0; i < MAX_CHANNEL; i++) {
		channels[i].valid = 0;
		channels[i].msg = NULL;
		channels[i].msg_len = 0;
		sema_init(&channels[i].sem, 1);
	}
}

static void __deinit_seq(void)
{
}


static void __release_seq(int seq, void *new_msg)
{
	struct channel *ch;

	ch = __get_channel(seq);
	if (ch == NULL)
		return;

	memcpy(ch->msg, new_msg, ch->msg_len);
	up(&ch->sem);

	__free_seq(seq);
}


static void __wait_seq(int seq, int timeout)
{
	int ret;
	struct channel *ch;

	ch = __get_channel(seq);
	if (ch == NULL)
		return;
	ret = down_timeout(&ch->sem, 250 * timeout);
	
	__free_seq(seq);

	if (ret < 0) {
		__log_error("down_timeout ret = %d\r\n", ret);
	}
}

static void __input(struct sk_buff *skb)
{
	struct nlmsghdr *nlh;
	void *msg;

	if (skb == NULL) {
		return;
	}

	nlh = (struct nlmsghdr *)skb->data;
	msg = (void *)NLMSG_DATA(nlh);
	__log_info("skb_len = %d, nlmsg_len = %d, nlmsg_type = %d\n", skb->len, nlh->nlmsg_len, nlh->nlmsg_type);

	if (skb->len > nlh->nlmsg_len) {
		return;
	}

	__release_seq(nlh->nlmsg_seq, msg);
	__free_seq(nlh->nlmsg_seq);
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
static void event_recv(struct sock *sk, int length)
{
	struct sk_buff *skb;

	while ((skb = skb_dequeue(&sk->sk_receive_queue))) {
		__input(skb);
		kfree_skb(skb);
	}
}
#endif

static int __send(struct ukcom_channel *ukcom, int target_id, void *msg, int msg_len, int seq)
{
	int ret;
	int size;
	sk_buff_data_t old_tail;
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	void *lmsg;
	size = NLMSG_SPACE(msg_len);

	skb = alloc_skb(size, GFP_ATOMIC);
	if (!skb) {
		__log_error("alloc_skb failed\r\n");
		return -1;
	}
	old_tail = skb->tail;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,14))
	nlh = nlmsg_put(skb, 0, 0, 0, msg_len, 0);
#else
	nlh = nlmsg_put(skb, 0, 0, 0, msg_len);
#endif
	if (nlh == NULL) {
		__log_error("nlmsg_put failed\r\n");
		kfree_skb(skb);
		return -1;
	}
	lmsg = NLMSG_DATA(nlh);
	memset(lmsg, 0, msg_len);
	memcpy(lmsg, msg, msg_len);

	nlh->nlmsg_len = skb->tail - old_tail;
	nlh->nlmsg_seq = seq;
	nlh->nlmsg_pid = ukcom->id;
	NETLINK_CB(skb).dst_group = UKCOM_GROUP;
	ret = netlink_unicast(ukcom->nlfd, skb, target_id, MSG_DONTWAIT);  //单播

	//FIXME: need free?
	//kfree_skb(skb);

	return ret;
}

int __ukcom_trigger(struct ukcom_channel *ukcom, int target_id, void *msg, int msg_len, int sync, int timeout)
{
	int rc = 0;

	if (ukcom->nlfd == NULL) {
		return -1;
	}

	if (sync == 0) {
		__send(ukcom, target_id, msg, msg_len, -1);
		if (rc < 0) {
			__log_error("async rc = %d\r\n", rc);
		}
	} else {
		int seq = 0;
		seq = __alloc_seq(msg, msg_len);
		__get_channel(seq);

		__log_info("seq = %d\r\n", seq);
		rc = __send(ukcom, target_id, msg, msg_len, seq);
		if (rc < 0 || seq == -1) {
			__log_error("rc = %d, seq = %d\r\n", rc, seq);
			__free_seq(seq);
			if (seq != -1) {
				__free_seq(seq);
			}
			return 0;
		}

		__wait_seq(seq, timeout);
		__free_seq(seq);
	}

	return rc;
}

int __init_ukcom(struct ukcom_channel *ukcom, int proto, int id)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
	struct netlink_kernel_cfg cfg = {
		.groups = 1,
		.input = __input,
	};
#endif

	memset(ukcom, 0, sizeof(*ukcom));

	ukcom->id = id;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0))
	ukcom->nlfd = netlink_kernel_create(&init_net, proto, &cfg);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
	ukcom->nlfd = netlink_kernel_create(&init_net, proto, THIS_MODULE, &cfg);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24))
	ukcom->nlfd = netlink_kernel_create(&init_net, proto, 1, __input, NULL, THIS_MODULE);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,23))
	ukcom->nlfd = netlink_kernel_create(proto, 1, event_recv, NULL, THIS_MODULE);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,14))
	ukcom->nlfd = netlink_kernel_create(proto, 1, event_recv, THIS_MODULE);
#else
	ukcom->nlfd = netlink_kernel_create(proto, event_recv);
#endif
	if (ukcom->nlfd == NULL) {
		__log_error("netlink_kernel_create failed\r\n");
		return -1;
	}

	__init_seq();

	return 0;

}

void __deinit_ukcom(struct ukcom_channel *ukcom)
{
	__deinit_seq();

	if (ukcom->nlfd != NULL) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24))
		netlink_kernel_release(ukcom->nlfd);
#else
		sock_release(ukcom->nlfd->sk_socket);
#endif
		ukcom->nlfd = NULL;
	}
}
