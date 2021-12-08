
#ifndef __UKCOM_H__
#define __UKCOM_H__

#ifdef __KERNEL__
#include <net/netlink.h>
#include <net/sock.h>
#endif

#define UKCOM_GROUP 0

struct ukcom_channel {

#ifdef __KERNEL__
	struct sock *nlfd;
#else
	int skfd;
#endif
	int id;
};

int __init_ukcom(struct ukcom_channel *ukcom, int proto, int pid);
void __deinit_ukcom(struct ukcom_channel *ukcom);

#ifdef __KERNEL__

/* timeout: 1s */
int __ukcom_trigger(struct ukcom_channel *ukcom, int target_id,
		void *msg, int msg_len, int sync, int timeout);

#else

void *ukcom_get_msg(struct ukcom_channel *ukcom, int msg_len);
int ukcom_reply_msg(struct ukcom_channel *ukcom, void *msg, int msg_len);

#endif /* __KERNEL__ */

#endif /* __UKCOM_H__ */
