/**
 * Copyright 2019
 */
#include "clientinfo.h"

#include <unistd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/vfs.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <net/if.h>
#include <linux/if.h>
#include <utmp.h>
#include <time.h>

int get_mac_addr(const char *eth, char *mac) {
  struct ifreq ifr;
  int sock;
  int i;

  if (strlen(eth) == 0) {
    return -1;
  }
  strncpy(ifr.ifr_name, eth, 16);

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    return -1;
  }

  if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
    for (i = 0; i < 6; i++) {
      snprintf(mac+3*i,
                sizeof(mac),
                "%02X:",
                (unsigned char)ifr.ifr_hwaddr.sa_data[i]);
    }
    if (strlen(mac) > 0) {
      mac[strlen(mac)-1] = '\0';
    }
  }
  close(sock);
  return 0;
}

int get_machine_arch(char *out_arch) {
  struct utsname uts;
  if (uname(&uts) < 0) {
    return -1;
  }
  strncpy(out_arch, uts.machine, 32);
  return 0;
}

int get_ip_addr(ip_macs_pointer *ipmacs, int *size) {
  int sockfd;
  int count;
  int i, index;
  char buf[512];

  struct ifconf ifconf;
  struct ifreq *ifreq;

  ifconf.ifc_len = 512;
  ifconf.ifc_buf = buf;
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    return -1;
  }
  ioctl(sockfd, SIOCGIFCONF, &ifconf);
  close(sockfd);

  ifreq = (struct ifreq*)buf;
  count = (ifconf.ifc_len/sizeof(struct ifreq));
  *size = 0;
  *ipmacs = (ip_macs_pointer)malloc(count * 2 *32);

  for (i = 0, index = 0; i < count; ++i) {
    char *ip = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
    if (strncmp(ip, "127.", 4) == 0 || strncmp(ip, "0.0.", 4) == 0) {
      ifreq++;
      continue;
    }
    if (0 == get_mac_addr(ifreq->ifr_name, buf)) {
      strncpy((*ipmacs)[index][0], ip, 32);
      strncpy((*ipmacs)[index][1], buf, 32);

      (*size)++;
      index++;
    }
    ifreq++;
  }
  return 0;
}

int get_disk_size(char *out, int maxlen) {
  struct statfs stats;
  u_int64_t blocks;
  u_int64_t size;

  if (statfs("/", &stats)) {
    return -1;
  }

  blocks = stats.f_blocks;
  size = blocks * stats.f_bsize;

#if __WORDSIZE == 64
  snprintf(out, maxlen, "%ld GB", size>>30);
#else
  snprintf(out, maxlen, "%lld GB", size>>30);
#endif
  return 0;
}

int get_memory_size(char *out, int maxlen) {
  struct sysinfo minfo;
  u_int64_t memsize;

  if (sysinfo(&minfo)) {
    return -1;
  }

  memsize = minfo.totalram/1024/1024;
#if __WORDSIZE == 64
  snprintf(out, maxlen, "%ld MB", memsize);
#else
  snprintf(out, maxlen, "%lld MB", memsize);
#endif
  return 0;
}

int get_host_name(char *name, int maxlen) {
  struct utsname uts;
  if (uname(&uts) < 0) {
    return -1;
  }
  strncpy(name, uts.nodename, maxlen);
  return 0;
}

int get_current_username(char *name, int maxlen) {
  struct passwd user;
  struct passwd *result;
  char buf[512];
  if (0 == getpwuid_r(getuid(), &user, buf, 512, &result)) {
    strncpy(name, user.pw_name, maxlen);
    return 0;
  }
  return -1;
}

int get_user_home(char *home, int maxlen) {
  char user[64] = {0};
  if (get_current_username(user, maxlen) < 0) {
    return -1;
  }

  if (0 == strcmp("root", user)) {
    strncpy(home, "/root", maxlen);
  } else {
    snprintf(home, maxlen, "/home/%s", user);
  }
  return 0;
}

int get_product_info(const char *filepath, char *name, int maxlen) {
  char buf[128] = {0};
  int index = 0;
  FILE *file;

  file = fopen(filepath, "r");
  if (NULL == file) {
    return -1;
  }

  name[0] = '\0';
  while (fgets(buf, sizeof(buf), file)) {
    if (buf[strlen(buf)-1] == '\n') {
      buf[strlen(buf)-1] = '\0';
    }
    if (0 == strlen(name)) {
      if (buf[0] == '-') {
        strncpy(name, buf+1, maxlen);
      } else {
        strncpy(name, buf, maxlen);
      }
    } else {
      strncat(name, buf, maxlen);
    }
  }
  fclose(file);
  return 0;
}

int get_os_release(const char *filepath, char *name, int maxlen) {
  char buffer[256] = {0};
  FILE *f;
  f = fopen(filepath, "r");
  if (!f) {
    return -1;
  }
  name[0] = '\0';
  while (fgets(buffer, 255, f)) {
    if (buffer[strlen(buffer)-1] == '\n') {
      buffer[strlen(buffer)-1]='\0';
    }

    if (0 == strncmp(buffer, "NAME=", 5)) {
      if (buffer[5] == '"' || buffer[5] == '\'') {
        snprintf(name, maxlen, "%s", buffer+6);
        name[strlen(name)-1] = ' ';
      }
    } else if (0 == strncmp(buffer, "VERSION=", 8)) {
      if (buffer[8] == '"' || buffer[8] == '\'') {
        strncat(name, buffer+9, maxlen);
        name[strlen(name)-1] = '\0';
      }
    }
  }
  fclose(f);
  return 0;
}

int get_issue(const char *filepath, char *name, int maxlen) {
  FILE *f;
  f = fopen(filepath, "r");
  if (!f) {
    return -1;
  }
  name[0] = '\0';
  fgets(name, maxlen, f);
  if (name[strlen(name)-1] == '\n') {
    name[strlen(name)-1]='\0';
  }

  char *end = strstr(name, "\\n");
  if (end) {
    name[end-name] = '\0';
  }
  fclose(f);
  return 0;
}

int get_redhat_release(const char *filepath, char *name, int maxlen) {
  FILE *f;
  f = fopen(filepath, "r");
  if (!f) {
    return -1;
  }
  name[0] = '\0';
  fgets(name, maxlen, f);
  if (name[strlen(name)-1] == '\n') {
    name[strlen(name)-1]='\0';
  }
  fclose(f);
  return 0;
}

int get_os_name(char *name, int maxlen) {
  if (0 == get_product_info("/etc/.productinfo", name, maxlen)) {
    return 0;
  } else if (0 == get_os_release("/etc/os-release", name , maxlen)) {
    return 0;
  } else if (0 == get_issue("/etc/issue", name, maxlen)) {
    return 0;
  } else if (0 == get_redhat_release("/etc/redhat-release", name, maxlen)) {
    return 0;
  }
  return -1;
}

int availd_guid(const char *guid) {
  char buf[5][16];
  memset(buf, 0, 5*16);
  sscanf(guid, "%8s-%4s-%4s-%4s-%12s", buf[0], buf[1], buf[2], buf[3], buf[4]);
  for (int i  =0; i < 5; i++) {
    for (int j = 0; j < strlen(buf[i]); j++) {
      if (!(buf[i][j] >= '0' && buf[i][j] <= '9' ||
            buf[i][j] >= 'a' && buf[i][j] <= 'f' ||
            buf[i][j] >= 'A' && buf[i][j] <= 'F')) {
        return -1;
      }
    }
  }
  return 0;
}

int get_bios_uuid(char *out, int maxlen) {
  if (access("/usr/sbin/dmidecode", F_OK)) {
      return -1;
  }
  FILE *pp = popen("/usr/sbin/dmidecode -s system-uuid|grep -v \"#\"", "r");
  if (pp == NULL) {
    return -1;
  }
  if (fgets(out, maxlen, pp) == NULL) {
    pclose(pp);
    return -1;
  }
  if (strlen(out) < 36) {
    pclose(pp);
    return -1;
  }

  if (out[strlen(out)-1] == '\n') {
    out[strlen(out)-1] = '\0';
  }

  pclose(pp);
  return 0;
}

int get_disk_uuid(char *out, int maxlen) {
  if (access("/sbin/blkid", F_OK)) {
      return -1;
  }

  FILE *pp = popen("/sbin/blkid|grep -i uuid|head -n 1|cut -d \\\" -f 2", "r");
  if (pp == NULL) {
    return -1;
  }

  if (fgets(out, maxlen, pp) == NULL) {
    pclose(pp);
    return -1;
  }

  if (out[strlen(out)-1] == '\n') {
    out[strlen(out)-1] = '\0';
  }

  pclose(pp);
  return 0;
}

int get_shutdown_info() {
  const char *LOGS = "/var/log/wtmp";

  struct utmp *p_utent;
  long t;

  utmpname(LOGS);
  /* rewinds the file pointer to the beginning of the utmp file */
  setutent();

  while((p_utent = getutent()) != NULL){
    // if(p_utent->ut_type != USER_PROCESS)
    //   continue;

    printf("%s\t", p_utent->ut_user);
    printf("%s\t", p_utent->ut_line);

    t = p_utent->ut_tv.tv_sec;

    printf("%.20s\t", ctime(&t) + 4);
    printf("(%s)\t", p_utent->ut_host);

    printf("%d\n", p_utent->ut_time);
  }
  /* closes the utmp file. */
  endutent();
  return 0;
}

int get_users() {
  FILE *f = fopen("/etc/passwd", "r");
  if (f) {
    char temp[1024];
    while(!feof(f)) {
      char *content = fgets(temp, 1024, f);
      if (content) {
        char user[32], passwd[12], user_desc[32], home[64], login[32];
        int uid, gid;
        sscanf(temp, "%s:%s:%d:%d:%s:%s:%s", user, passwd, &uid, &gid, user_desc, home, login);
      }
    }
    fclose(f);
  }

  FILE *shadow = fopen("/etc/shadow", "r");
  if (shadow) {
    char temp[1024];
    while(!feof(shadow)) {
      char *content = fgets(temp, 1024, f);
      if (content) {
        char user[32], passwd[12], user_desc[32], home[64], login[32];
        int passwd_last_change, change_inteval, passwd_valid, passwd_warning_days, passwd_expire_usesd;
        sscanf(temp, "%s:%s:%d:%d:%d:%d:::", user, passwd, &passwd_last_change, &change_inteval, &passwd_valid, &passwd_warning_days);
      }
    }
    fclose(shadow);
  }
  return 0;
}
