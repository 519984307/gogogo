#ifndef USRLINK_H_  
#define USRLINK_H_  
  

#define USER_NETLINK_CMD    30

#define MAX_MSG_COUNT       1024
#define MAX_MSG_LEN         60 
#define MAX_TOTAL_BUFFER_LEN 61440   //MAX_MSG_LEN*MAX_MSG_COUNT

#ifdef __DEBUG__
#ifndef _LINUX_MODULE_H
#include <stdio.h>
    #define DEBUG(format, args...) fprintf(stdout, format, ##args);fprintf(stdout, "\n");
#endif
#else
    #define DEBUG(format, args...)
#endif

typedef enum nfl_error_e {  
    NET_ERROR = -1,  
    NET_OK = 0,  
    NET_PARAM,  
    NET_MEM,  
    NET_SOCK,  
} nfl_netlink_err;  
  
typedef enum nfl_module_e {  
    NFL_CMD = 1,  
} nfl_netlink_module;  
  
typedef enum nfl_cmd_type_e {  
    NFL_BLACKLIST_SET_START = 0,
    NFL_BLACKLIST_SET_END,
    NFL_BLACKLIST_SET_REQ,  
    NFL_BLACKLIST_SET_ACK,  
    NFL_BLACKLIST_GET_REQ,  
    NFL_BLACKLIST_GET_ACK,  
    NFL_BLACKLIST_BLOCK_IP,  
    NFL_BLACKLIST_BLOCK_URL,  
} nfl_netlink_cmd_type;  
  

#endif /* USRLINK_H_ */  
