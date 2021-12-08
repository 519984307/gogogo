#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/netlink.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include "ukcom.h"

#define NLMSG_HDR(msg)  ((void*)(((char*)msg) - NLMSG_LENGTH(0)))

void *ukcom_get_msg(struct ukcom_channel *ukcom, int msg_len)
{
	struct nlmsghdr *unlhdr;
	struct sockaddr_nl kpeer;
	int kpeerlen;
	int rcvlen = 0;
	void *umsg;

	unlhdr = (struct nlmsghdr *)malloc(NLMSG_SPACE(msg_len));
	umsg = (void *)NLMSG_DATA(unlhdr);

	memset(unlhdr, 0, NLMSG_SPACE(msg_len));
	kpeerlen = sizeof(struct sockaddr_nl);
	rcvlen = recvfrom(ukcom->skfd, unlhdr, NLMSG_LENGTH(msg_len),
			0, (struct sockaddr *)&kpeer, &kpeerlen);
	if (rcvlen > 0 && rcvlen == NLMSG_LENGTH(msg_len)) {
		return umsg;
	} else {
		free(unlhdr);
		return NULL;
	}
}

int ukcom_reply_msg(struct ukcom_channel *ukcom, void *msg, int msg_len)
{
	struct sockaddr_nl kpeer;
	int ret = 0, kpeerlen;
	struct nlmsghdr *knlhdr = NULL;
	struct nlmsghdr *unlhdr = NULL;
	void *kmsg;

	if (msg == NULL) {
		return -1;
	}

	unlhdr = (struct nlmsghdr *)NLMSG_HDR(msg);
	if (unlhdr->nlmsg_seq != -1) {
		memset(&kpeer, 0, sizeof(kpeer));
		kpeer.nl_family = AF_NETLINK;
		kpeer.nl_pid = unlhdr->nlmsg_pid;
		kpeer.nl_groups = 0;

		knlhdr = (struct nlmsghdr *)calloc(1, NLMSG_SPACE(msg_len));
		kmsg = (void *)NLMSG_DATA(knlhdr);
		memcpy(kmsg, msg, msg_len);

		knlhdr->nlmsg_len = NLMSG_LENGTH(msg_len);
		knlhdr->nlmsg_flags = 0;
		knlhdr->nlmsg_seq = unlhdr->nlmsg_seq;
		knlhdr->nlmsg_type = 0;
		knlhdr->nlmsg_pid = ukcom->id;

		memcpy((void *)NLMSG_DATA(knlhdr), kmsg, sizeof(*kmsg));

		ret = sendto(ukcom->skfd, knlhdr, knlhdr->nlmsg_len, 0,
				(struct sockaddr*)&kpeer, sizeof(kpeer));
		free(knlhdr);
	}
	free(unlhdr);

	return ret;
}

int __init_ukcom(struct ukcom_channel *ukcom, int proto, int id)
{
	struct sockaddr_nl local;
	struct sockaddr_nl kpeer;
	int kpeerlen;
	struct nlmsghdr *unlhdr = NULL;
	int rcvlen = 0;

	memset(ukcom, 0, sizeof(*ukcom));

	ukcom->id = id;

	ukcom->skfd = socket(AF_NETLINK, SOCK_RAW, proto);
	if (ukcom->skfd < 0) {
		return -1;
	}

	memset(&local, 0, sizeof(local));
	local.nl_family = AF_NETLINK;
	local.nl_pid = ukcom->id;
	local.nl_groups = UKCOM_GROUP;
	if (bind(ukcom->skfd, (struct sockaddr*)&local, sizeof(local)) != 0) {
		return -1;
	}

	return 0;
}

void __deinit_ukcom(struct ukcom_channel *ukcom)
{
	if (ukcom->skfd != 0) {
	    	close(ukcom->skfd);
	}
}
