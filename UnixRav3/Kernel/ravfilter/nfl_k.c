#include <linux/init.h>  
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/types.h>
#include <linux/sched.h>  
#include <linux/if_ether.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/ip.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>  
#include <linux/version.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/moduleparam.h>
#include <net/sock.h>  
#include "usrlink.h"  

#define IP 0x800
#define TCP 0x6
/* Necessary because we use the proc fs */
#define procfs_name "port"

#ifndef NIPQUAD
#define NIPQUAD(addr) ((unsigned char*)&addr)[0],((unsigned char*)&addr)[1],((unsigned char*)&addr)[2],((unsigned char*)&addr)[3] 
#endif

#define NFL_MAX_URL_LENGTH 1024 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mengqy");
MODULE_DESCRIPTION("(nfl)netfilter netlink module");


DEFINE_MUTEX(m_Lock);

char **m_TmpIPPortList;

typedef struct _T_RAV_IPPort{
    uint32_t ip;
    uint32_t port;
}tRavIPPort;

tRavIPPort  **m_pIPPorts;

char **m_UrlList; 
char **m_TmpUrlList; 
char **m_pLines;
unsigned char *m_pSendData;

int m_nTmpIPPortCount = 0;
int m_nIPPortCount = 0;
int m_nUrlCount = 0;
int m_nTmpUrlCount = 0;
int m_nLineCount = 0;
int m_nDestPid;


char payload_url[NFL_MAX_URL_LENGTH];
char payload_host[NFL_MAX_URL_LENGTH];
bool f[51][51];

struct sock *m_netlink_fd;  
struct nlmsghdr *m_nlmsghdr_rcv;
 
bool init_line_buffer(char ***buf, int size, int count)
{
    int i;
    mutex_lock(&m_Lock);
    *buf = (char**)kmalloc(count* sizeof(char*), GFP_KERNEL);
    if((*buf) == NULL){
        mutex_unlock(&m_Lock);
        printk("error, nfl_k out of memory\n");
        return false;
    }
    for(i=0; i<count; i++){
        (*buf)[i] = (char*)kmalloc(size, GFP_KERNEL);
        if((*buf)[i] == NULL){
            mutex_unlock(&m_Lock);
            printk("error, nfl_k out of memory\n");
            return false;
        }
    }
    mutex_unlock(&m_Lock);
    return true;
}

void uninit_line_buffer(char ***lines, int size, int count)
{
    int i;
    mutex_lock(&m_Lock);
    for(i=0; i<count; i++){
        kfree((*lines)[i]);
    }
    kfree(*lines);
    (*lines) = NULL;
    mutex_unlock(&m_Lock);
}

bool init_all_buffer(void)
{
    bool bret = false;
    int i;
    bret = init_line_buffer(&m_TmpIPPortList, MAX_MSG_LEN, MAX_MSG_COUNT);
    if(!bret)
        return false;
    m_pSendData = (unsigned char*)kmalloc(NFL_MAX_URL_LENGTH, GFP_KERNEL);
    if(m_pSendData == NULL)
        return false;

    m_pIPPorts = (tRavIPPort**)kmalloc(sizeof(tRavIPPort**)*MAX_MSG_COUNT, GFP_KERNEL);
    if(m_pIPPorts == NULL)
        return false;
    for(i=0; i<MAX_MSG_COUNT; i++){
        m_pIPPorts[i] = (tRavIPPort*)kmalloc(sizeof(tRavIPPort), GFP_KERNEL);
        if(m_pIPPorts[i] == NULL)
            return false;
    }
    bret = init_line_buffer(&m_UrlList, MAX_MSG_LEN, MAX_MSG_COUNT);
    if(!bret)
        return false;
    bret = init_line_buffer(&m_pLines, MAX_MSG_LEN, MAX_MSG_COUNT);
    if(!bret)
        return false;
    bret = init_line_buffer(&m_TmpUrlList, MAX_MSG_LEN, MAX_MSG_COUNT);
    return bret;
}

void uninit_all_buffer(void)
{
    int i;
    uninit_line_buffer(&m_TmpIPPortList, MAX_MSG_LEN, MAX_MSG_COUNT);
    for(i=0; i<MAX_MSG_COUNT; i++){
        kfree(m_pIPPorts[i]);
    }
    kfree(m_pIPPorts);
    uninit_line_buffer(&m_UrlList, MAX_MSG_LEN, MAX_MSG_COUNT);
    uninit_line_buffer(&m_TmpUrlList, MAX_MSG_LEN, MAX_MSG_COUNT);
    uninit_line_buffer(&m_pLines, MAX_MSG_LEN, MAX_MSG_COUNT);
    kfree(m_pSendData);
}

uint32_t trans_str_ip_to_u32(const char *ip_str)
{
    uint32_t u8ip[4] = {};
    uint32_t uip = 0;
    if(ip_str == NULL)
        return 0;
    if(strcmp(ip_str, "0.0.0.0") == 0)
        return 0;
    sscanf(ip_str, "%u.%u.%u.%u", &u8ip[0], &u8ip[1], &u8ip[2], &u8ip[3]);

    uip = u8ip[0]|u8ip[1]<<8|u8ip[2]<<16|u8ip[3]<<24;
    return uip;
}

void update_ip_port_url_list(void)
{
    int i,j;
    char *pline = NULL;
    char stmp[MAX_MSG_LEN];
    char *cpos = NULL;
    unsigned long ultmp;
    
    m_nTmpIPPortCount = 0;
    m_nTmpUrlCount = 0;
    
    for(i=0; i<m_nLineCount; i++){
        pline = m_pLines[i];
        if(pline[0] == 'I' && pline[1] == '#'){
            strcpy(m_TmpIPPortList[m_nTmpIPPortCount++], pline+2);        
            //printk("ip=%s\n", pline+2);
        }else if(pline[0] == 'U' && pline[1] == '#'){
            //printk("url=%s\n", pline+2);
            strcpy(m_TmpUrlList[m_nTmpUrlCount++], pline+2);
        }
    }
    //printk("ipcount=%d urlcount=%d\n", m_nTmpIPPortCount, m_nTmpUrlCount);
    mutex_lock(&m_Lock);

    for(i=0; i<m_nTmpIPPortCount; i++){
        if(strchr(m_TmpIPPortList[i], ':') == NULL) {//ip a.b.c.d
            m_pIPPorts[i]->ip = trans_str_ip_to_u32(m_TmpIPPortList[i]);
            m_pIPPorts[i]->port = 0;
        }else{// ip:port
            if(strstr(m_TmpIPPortList[i],"0.0.0.0") == NULL){//a.b.c.d:port
                j = 0;
                while(m_TmpIPPortList[i][j] != ':'){
                    stmp[j] = m_TmpIPPortList[i][j];
                    j++;
                }
                stmp[j] = '\0';
                m_pIPPorts[i]->ip = trans_str_ip_to_u32(stmp);
                cpos = strchr(m_TmpIPPortList[i], ':');
                if(kstrtoul(cpos+1, 10, &ultmp) != 0){
                    m_pIPPorts[i]->port = 0;
                }else{
                    m_pIPPorts[i]->port = ultmp;
                }
            }else{//0.0.0.0:port
                cpos = strchr(m_TmpIPPortList[i], ':');
                m_pIPPorts[i]->ip = 0;
                if(kstrtoul(cpos+1, 10, &ultmp) != 0){
                    m_pIPPorts[i]->port = 0;
                }else{
                    m_pIPPorts[i]->port = ultmp;
                }
            }
        }
    }
    m_nIPPortCount = m_nTmpIPPortCount;

    for(i=0; i<m_nTmpUrlCount; i++)
        strcpy(m_UrlList[i], m_TmpUrlList[i]);
    m_nUrlCount = m_nTmpUrlCount;

    mutex_unlock(&m_Lock);
    //printk("at end of update_ip_port_url_list\n");
}

/*
   return value: true found, false not find
   */
bool find_ip_port_in_blacklist(uint32_t ip, uint32_t port)
{
    int i;
    if(mutex_trylock(&m_Lock) == 0){
        return false;
    }
    for(i=0; i<m_nIPPortCount; i++){
        if(m_pIPPorts[i]->ip == ip){
            if(m_pIPPorts[i]->port == 0 || m_pIPPorts[i]->port == port){
                mutex_unlock(&m_Lock);
                return true;
            }
        }
        if(m_pIPPorts[i]->port == port){
            if(m_pIPPorts[i]->ip == 0 || m_pIPPorts[i]->ip == ip){
                mutex_unlock(&m_Lock);
                return true;
            }
        }
    }
    mutex_unlock(&m_Lock);
    return false;
}

/*
   return value: true found, false not find
   */
bool find_url_in_blacklist(const char *url)
{
    int i;
    if(url == NULL)
        return false;
    //printk("find url from blacklist,url:%s", url);
    if(mutex_trylock(&m_Lock) == 0){
        return false;
    }
    for(i=0; i<m_nUrlCount; i++){
        //printk("compare:%s##%s##", url, m_UrlList[i]);
        if(strstr(url, m_UrlList[i]) != NULL){
            mutex_unlock(&m_Lock);
            return true;
        }
    }
    mutex_unlock(&m_Lock);
    return false;
}

int hex2dec(char c)
{
    if ('0' <= c && c <= '9'){
        return c - '0';
    }else if('a' <= c && c <= 'f'){
        return c - 'a' + 10;
    }else if('A' <= c && c <= 'F'){
        return c - 'A' + 10;
    }
    return -1;
}

void urldecode(char *url)
{
    int i = 0;
    int len = strlen(url);
    int res_len = 0;
    char res[NFL_MAX_URL_LENGTH];
    for (i = 0; i < len; ++i){
        char c = url[i];
        if (c != '%'){
            res[res_len++] = c;
        }else{
            char c1 = url[++i];
            char c0 = url[++i];
            int num = 0;
            num = hex2dec(c1) * 16 + hex2dec(c0);
            res[res_len++] = num;
        }
    }
    res[res_len] = '\0';
    strcpy(url, res);
}

/*
   return value: true found, false not find
   */
bool check_payload_filter_url(const char *payload, int nlen)
{
    int i,k,j,tlen;//,len;
    if(payload == NULL || nlen < 11)
        return false;
    if(payload[0] != 'G' || payload[1] != 'E' || payload[2] != 'T')
        return false;
    //printk("payload:%s\n", payload);
    tlen = nlen-3;
    for (i = 0; i < tlen; ++i)
    {
        if(payload[i] == 'G' && payload[i+1] == 'E' && payload[i+2] == 'T' && payload[i+3] == ' ') {
            j = 0;
            k = i + 4;
            while(payload[k] != ' ' && k < tlen) {
                payload_url[j++] = payload[k++];
            }
            payload_url[j] = '\0';
            i = k + 9;
        }
        if (payload[i] == 'H' && payload[i+1] == 'o' && payload[i+2] == 's' && payload[i+3] == 't' && payload[i+4] == ':')
        {
            k = 0;
            i += 6;

            while(payload[i] != '\r' && payload[i+1] != '\n'){
                payload_host[k++] = payload[i++];
                if(i >= tlen){
                    return false;
                }
            }
            payload_host[k] = '\0';
            if(strstr(payload_host, ".") == NULL)
                return false;
            urldecode(payload_url);
            strcat(payload_host, payload_url);
            //printk("payload_host:%s\n", payload_host);
            //printk("payload_url:%s\n", payload_url);
            //len = k+1;//strlen(payload_host);
            if(find_url_in_blacklist(payload_host)){
                printk("find black url:%s\n", payload_host);
                return true;
            }
        }
    }
    return false;
}

static void netlink_to_user(int dest, int msg_type, int result, void *buf, int len)  
{  
    struct nlmsghdr *nl = NULL;  
    struct sk_buff *skb = NULL;  
    int size, nret;  
  
    size = NLMSG_SPACE(len);  
    //printk("size=%d len=%d  sizeof(struct nlmsghdr)=%u\n", size, len, (unsigned int)sizeof(struct nlmsghdr));
    skb = alloc_skb(size, GFP_ATOMIC);  
    if(!skb || !buf)  
    {  
        //printk(KERN_ALERT "netlink_to_user skb of buf null!\n");  
        return;  
    }  
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,14))
    nl = nlmsg_put(skb, 0, 0, 0, NLMSG_SPACE(len) - sizeof(struct nlmsghdr), 0);  
#else
    nl = nlmsg_put(skb, 0, 0, 0, NLMSG_SPACE(len) - sizeof(struct nlmsghdr));
#endif
    memcpy(NLMSG_DATA(nl), buf, len);  
    //NETLINK_CB(skb).groups = 0;
    //NETLINK_CB(skb).pid = 0;
    //NETLINK_CB(skb).dst_pid = dest; 
    //NETLINK_CB(skb).dst_groups = 0;
    nl->nlmsg_type = msg_type;
    nl->nlmsg_pid = 0; //result;
    nl->nlmsg_flags = result;
    nl->nlmsg_len = len;  
    
    nret = netlink_unicast(m_netlink_fd, skb, dest, MSG_DONTWAIT);  
    //printk(KERN_ALERT "K send packet return %d, dest=%d\n", nret, dest);  
}  


static unsigned int nfl_hook_func(
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0) && LINUX_VERSION_CODE > KERNEL_VERSION(3,9,0)) || LINUX_VERSION_CODE == KERNEL_VERSION(2,6,32)
    unsigned int hooknum,
    struct sk_buff *skb,
    const struct net_device *in,
    const struct net_device *out,
    int (*okfn)(struct sk_buff *)
#elif ((LINUX_VERSION_CODE >= KERNEL_VERSION(3,11,0) && LINUX_VERSION_CODE < KERNEL_VERSION(4,0,5)) || LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,0))
    void *priv, struct sk_buff *skb,
#ifndef __GENKSYMS__
        const struct nf_hook_state *state
#else
        int (*okfn)(struct sk_buff *)
#endif
#else
    const struct nf_hook_ops *ops,struct sk_buff *skb,const struct net_device *in,const struct net_device *out,
#ifndef __GENKSYMS__
        const struct nf_hook_state *state
#else
        int (*okfn)(struct sk_buff *)
#endif
#endif
)
{
    const struct iphdr *iph = ip_hdr(skb);
    struct tcphdr *ptcp = NULL;
//    struct udphdr *pudp = NULL;
    int len, slen;
    char *payload = NULL;

    if(iph->protocol == IPPROTO_TCP){
        ptcp = (struct tcphdr *)skb_transport_header(skb);
        if(find_ip_port_in_blacklist(iph->daddr, ntohs(ptcp->dest))){
            printk(KERN_INFO "find black TCP pkt to %u.%u.%u.%u:%u\n", NIPQUAD(iph->daddr), ntohs(ptcp->dest));
            memcpy(m_pSendData, &iph->saddr, 4);
            memcpy(m_pSendData+4, &iph->daddr, 4);
            netlink_to_user(m_nDestPid, NFL_BLACKLIST_BLOCK_IP, 0, (void*)m_pSendData, 8);
            return NF_DROP;
//        }else if(find_ip_in_blacklist(iph->saddr)){
//            printk(kern_info "find black tcp pkt from %u.%u.%u.%u:%u\n", nipquad(iph->saddr), ntohs(ptcp->source));
//            memcpy(m_psenddata, &iph->saddr, 4);
//            memcpy(m_psenddata+4, &iph->daddr, 4);
//            netlink_to_user(m_ndestpid, nfl_blacklist_block_ip, 0, (void*)m_psenddata, 8);
//            return nf_drop;
        }
        if (likely(ntohs(ptcp->dest) == 80)){
            if (0 != skb_linearize(skb)) 
                return NF_ACCEPT;

            len = ptcp->doff*4 + iph->ihl*4;
            payload = (void *) skb->data + len;
            slen = iph->tot_len - len;//strlen(payload);
            //printk("iph->tot_len:%u  iph->ihl:%u tcp->doff:%u len:%d  slen=%zul\n", iph->tot_len&0xFFFF, iph->ihl, ptcp->doff, len, strlen(payload));
            if(check_payload_filter_url(payload, slen)){
                slen = strlen(payload_host)+1;
                memcpy(m_pSendData, payload_host, slen);
                netlink_to_user(m_nDestPid, NFL_BLACKLIST_BLOCK_URL, 0, (void*)m_pSendData, slen);
                return NF_DROP;
            }
        }
    }else if(iph->protocol == IPPROTO_UDP) {
//        pudp = (struct udphdr *)skb_transport_header(skb);
//        if(find_port_in_blacklist(ntohs(pudp->dest))){
//            return NF_DROP;
//        }
    }
    return NF_ACCEPT;
}
  
static int process_blacklist_get(int dest, void *buf, int len)  
{  
    //printk(KERN_ALERT "In process_blacklist get!\n");  
    memcpy(buf, "I known you !", 13);  
    netlink_to_user(dest, NFL_BLACKLIST_GET_ACK, NET_OK, buf, 13);  
    return NET_OK;  
}  
  
static int process_blacklist_set(int dest, void *buf, int len)  
{  
    //printk(KERN_ALERT "In process_blacklist set! len=%d buf=%s\n", len, (char *)buf);  
    if(buf == NULL || len<17)
        return NET_ERROR;
    ((char*)buf)[len-16] = '\0';
    strcpy(m_pLines[m_nLineCount++], (char*)buf);
    //memcpy(buf, "S known you !", 13);  
    //netlink_to_user(dest, NFL_BLACKLIST_SET_ACK, NET_OK, buf, 13);  
    return NET_OK;  
}  
  
  
static void netlink_process_packet(struct nlmsghdr *nl)  
{  
    int ret;  
  
    switch(nl->nlmsg_type)  
    {  
    case NFL_BLACKLIST_GET_REQ:  
        ret = process_blacklist_get(nl->nlmsg_pid, NLMSG_DATA(nl), nl->nlmsg_len);  
        break;  
    case NFL_BLACKLIST_SET_START:
        m_nLineCount = 0;
        break;
    case NFL_BLACKLIST_SET_END:
        //printk("before enter update_ip_port_url_list\n");
        update_ip_port_url_list();
        memcpy(NLMSG_DATA(nl), "S known you !", 14);  
        netlink_to_user(nl->nlmsg_pid, NFL_BLACKLIST_SET_ACK, NET_OK, NLMSG_DATA(nl), 14);  
        break;
    case NFL_BLACKLIST_SET_REQ:  
        ret = process_blacklist_set(nl->nlmsg_pid, NLMSG_DATA(nl), nl->nlmsg_len);  
        break;  
    default:break;  
    }  
}  
  
static void netlink_recv_packet(struct sk_buff *__skb)  
{  
    struct sk_buff *skb;  
    if(__skb == NULL)
        return;
    //printk("enter netlink_recv_packet, len=%u\n", __skb->len);
    skb = skb_get(__skb);  
    if(skb->len >= sizeof(struct nlmsghdr))  
    {  
        m_nlmsghdr_rcv = (struct nlmsghdr *)skb->data;  
	    m_nDestPid = m_nlmsghdr_rcv->nlmsg_pid;
        if(m_nlmsghdr_rcv->nlmsg_len >= sizeof(struct nlmsghdr) && __skb->len >= m_nlmsghdr_rcv->nlmsg_len)  
        {  
            netlink_process_packet(m_nlmsghdr_rcv);  
        }  
    }  
    else  
        printk(KERN_ALERT "Kernel receive msg length error!\n");  
    kfree_skb(skb);
}  

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
static void event_recv(struct sock *sk, int length)
{
        struct sk_buff *skb;

        while ((skb = skb_dequeue(&sk->sk_receive_queue))) {
                netlink_recv_packet(skb);
                kfree_skb(skb);
        }
}
#endif
  
static int __init netlink_init(void)  
{  
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
    struct netlink_kernel_cfg cfg
    = {
        .groups = 1,
        .input = netlink_recv_packet,
    };
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0))
    m_netlink_fd = netlink_kernel_create(&init_net, USER_NETLINK_CMD, &cfg);  
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))
    m_netlink_fd = netlink_kernel_create(&init_net, USER_NETLINK_CMD, THIS_MODULE, &cfg);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24))
    m_netlink_fd = netlink_kernel_create(&init_net, USER_NETLINK_CMD, 1, netlink_recv_packet, NULL, THIS_MODULE);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,23))
    m_netlink_fd = netlink_kernel_create(USER_NETLINK_CMD, 1, event_recv, NULL, THIS_MODULE);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,14))
    m_netlink_fd = netlink_kernel_create(USER_NETLINK_CMD, 1, event_recv, THIS_MODULE);
#else
    m_netlink_fd = netlink_kernel_create(USER_NETLINK_CMD, event_recv);
#endif

    if(NULL == m_netlink_fd)  
    {  
        printk(KERN_ALERT "Init netlink fali!\n");  
        return -1;  
    }  
    printk(KERN_ALERT "Init netlink success!\n");  
    return 0;  
}  
  
static void __exit netlink_exit(void)  
{  
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24))
    netlink_kernel_release(m_netlink_fd);  
#else
    sock_release(m_netlink_fd);
#endif
    printk(KERN_ALERT "Exit netlink!\n");  
}  
 
static struct nf_hook_ops nfho={
    .hook           = nfl_hook_func, 
    .pf             = NFPROTO_IPV4,
    .hooknum        = NF_INET_LOCAL_OUT, 
    .priority       = NF_IP_PRI_FIRST,  
};

static int __init nfl_hook_init(void)
{
    mutex_init(&m_Lock);
    init_all_buffer();
    netlink_init();
#if (LINUX_VERSION_CODE > KERNEL_VERSION(4,14,0))
    return nf_register_net_hook(&init_net, &nfho);
#else
    return nf_register_hook(&nfho);
#endif
}

static void __exit nfl_hook_fini(void)
{
    netlink_exit();
#if (LINUX_VERSION_CODE > KERNEL_VERSION(4,14,0))
    nf_unregister_net_hook(&init_net, &nfho);
#else
    nf_unregister_hook(&nfho);
#endif
    uninit_all_buffer();
}

module_init(nfl_hook_init);
module_exit(nfl_hook_fini);
