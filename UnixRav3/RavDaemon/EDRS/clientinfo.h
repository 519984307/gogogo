/**
 * Copyright 2019
 */
#ifndef RAVDAEMON_EDRS_CLIENTINFO_H_
#define RAVDAEMON_EDRS_CLIENTINFO_H_

// 获取机器的架构
int get_machine_arch(char *out_arch);

/**
 * 获取机器的IP和MAC地址
 * params:
 *  ip/addr: char [*size][2][32]
 * [0]为IP地址, [1]为MAC地址
 * 完成后，必须free
 */
typedef char (*ip_macs_pointer)[2][32];
int get_ip_addr(ip_macs_pointer *ipmacs, int *size);

int get_disk_size(char *out, int maxlen);

int get_memory_size(char *out, int maxlen);

int get_host_name(char *name, int maxlen);

int get_current_username(char *name, int maxlen);

int get_user_home(char *home, int maxlen);

int get_os_name(char *name, int maxlen);

int avalid_guid(const char *guid);

int get_bios_uuid(char *out, int maxlen);
int get_disk_uuid(char *out, int maxlen);

int get_shutdown_info();

int get_users();

#endif  // RAVDAEMON_EDRS_CLIENTINFO_H_
