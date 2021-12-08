#include "clientinfo.h"
#include "commonapi.h"

#include <unistd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/vfs.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>

int ClientInfo::GetMacAddr(string& out) {
    int retry = 0;
    while (mEthName.empty() && retry++<3) {
        GetLocalIP(mIP);
        sleep(1);
    }

    if (mEthName.empty()) {
        out = "00:00:00:00:00:00";
        return 0;
    }

    int   sock;
    struct   ifreq   ifr;
    char mac[48] = {0};

    sock = socket(AF_INET,   SOCK_DGRAM,   0);
    if(sock < 0) {
        return -1;
    }

    strcpy(ifr.ifr_name, mEthName.c_str());
    if( ioctl(sock, SIOCGIFHWADDR, &ifr ) == 0) {
        for(int i = 0; i < 6; i++){
            sprintf(mac+3*i, "%02X:", (unsigned char)ifr.ifr_hwaddr.sa_data[i]);
        }
        if (strlen(mac) > 0) {
            mac[strlen(mac)-1] = '\0';
        }

        out = mac;
    }
    close(sock);
    return strlen(mac)>0?0:-1;
}

int ClientInfo::GetLocalIP(string &outIP) {
    if (!mIP.empty()) {
        outIP = mIP;
        return 0;
    }

    char outip[32] = {0};
    int i = 0;
    int sockfd;
    struct ifconf ifconf;
    char buf[512];
    struct ifreq *ifreq;
    char* ip;

    ifconf.ifc_len = 512;
    ifconf.ifc_buf = buf;

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0) {
        return -1;
    }
    ioctl(sockfd, SIOCGIFCONF, &ifconf);
    close(sockfd);
    ifreq = (struct ifreq*)buf;

    for(i=(ifconf.ifc_len/sizeof(struct ifreq)); i>0; i--) {
        ip = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
        if(strncmp(ip,"127.",4)==0 || strncmp(ip, "0.0.", 4)==0) {
            ifreq++;
            continue;
        }
        strcpy(outip,ip);
        outIP = outip;

        mIP = outip;
        mEthName = ifreq->ifr_name;
        return 0;
    }
    return -1;
}

int ClientInfo::GetDiskSize(string &outSize) {
    struct statfs diskStatfs;
    statfs("/", &diskStatfs);

    unsigned long long totalBlocks = diskStatfs.f_blocks;
    unsigned long long totalSize = totalBlocks * diskStatfs.f_bsize;

    char temp[32] = {0};
    sprintf(temp, "%lld", totalSize>>30);
    outSize = temp;
    outSize += "GB";

    return 0;
}

int ClientInfo::GetMemorySize(string &outSize) {
    struct sysinfo minfo;
    sysinfo(&minfo);
    unsigned long memsize = minfo.totalram/1024;

    char temp[32] = {0};
    sprintf(temp, "%ldKB", memsize);

    outSize = temp;
    return 0;
}

int ClientInfo::GetOSName(string &outName) {
    char osname[128] = {0};
    if (access("/usr/bin/lsb_release", F_OK) == 0) {
        FILE *fp = NULL;
        if((fp=popen("lsb_release -ds","r"))==NULL){
            return -1;
        }

        if (fgets(osname, sizeof(osname)-1, fp) == NULL) {
            pclose(fp);
            return -1;
        }

        pclose(fp);
    }
    else if (access("/etc/redhat-release", F_OK) == 0) {
        FILE *fp = NULL;
        if((fp=popen("cat /etc/redhat-release","r"))==NULL) {
            return -1;
        }

        if (fgets(osname, sizeof(osname)-1, fp) == NULL) {
            pclose(fp);
            return -1;
        }

        pclose(fp);
    }
    else if (access("/etc/issue", F_OK) == 0) {
        FILE *fp = NULL;
        if((fp=popen("cat /etc/issue|awk 'NR==1{print}'","r")) == NULL) {
            return -1;
        }

        if (fgets(osname, sizeof(osname)-1, fp) == NULL) {
            pclose(fp);
            return -1;
        }

        pclose(fp);
    }

    if (strlen(osname) < 4) {
        return -1;
    }

    if (osname[strlen(osname)-1] == '\n') {
        osname[strlen(osname)-1] = '\0';
    }

    if (osname[0] == '\"' && osname[strlen(osname)-1] == '\"') {
        for (int j=0;j<strlen(osname)-2;j++) {
            osname[j] = osname[j+1];
        }
        osname[strlen(osname)-2] = '\0';
    }

    if (osname[strlen(osname)-1] == '\n') {
        osname[strlen(osname)-1] = '\0';
    }

    outName = osname;
    int str_len = strlen(osname);
    if (str_len > 0) {
        if (osname[0] < 33) {
            return -1;
        }
    }
    return str_len>0?0:-1;
}


int ClientInfo::GetClientName(string& outName) {
    while (mHostName.empty()) {
        struct utsname uts;
        char computer[256] = {0};

        //get system infomation
        if (gethostname(computer, 255) != 0 || uname(&uts) < 0) {
            return -1;
        }
        mHostName = uts.nodename;
        mCPUArch = uts.machine;
    }

    outName = mHostName;
    return 0;
}

int ClientInfo::GetClientArch(string &outArch) {
    while (mCPUArch.empty()) {
        struct utsname uts;
        char computer[256] = {0};

        //get system infomation
        if (gethostname(computer, 255) != 0 || uname(&uts) < 0) {
            return -1;
        }
        mHostName = uts.nodename;
        mCPUArch = uts.machine;
    }

    outArch = mCPUArch;
    return 0;
}

int ClientInfo::GetCurrentUser(string &outUser) {
    struct passwd *user;
    user= getpwuid(getuid());
    if (user) {
        outUser = user->pw_name;
        mUser = outUser;
        return 0;
    }
    return -1;
}

int ClientInfo::GetUserRoot(string &outRoot) {
    int retry = 0;
    while (mUser.empty() && retry++<3) {
        GetCurrentUser(mUser);
    }

    if (mUser == "root") {
        outRoot = "/";
    }
    else {
        outRoot = "/home/" + mUser;
    }
    return 0;
}

int ClientInfo::AvaildGUID(const char * guid) {
    //const char *GUID_REG = "[0-9|a-fA-F]{8}-[0-9|a-fA-F]{4}-[0-9|a-fA-F]{4}-[0-9|a-fA-F]{4}-[0-9|a-fA-F]{12}";
    char buf[5][16] = {0};
    sscanf(guid, "%8s-%4s-%4s-%4s-%12s", buf[0], buf[1], buf[2], buf[3], buf[4]);
    for (int i=0; i<5; i++) {
        for(int j=0; j<strlen(buf[i]); j++) {
            if (!(buf[i][j]>='0' && buf[i][j]<='9' ||
                  buf[i][j]>='a' && buf[i][j]<='f' ||
                  buf[i][j]>='A' && buf[i][j]<='F')) {
                return -1;
            }
        }
    }
    return 0;
}

int ClientInfo::GetBiosUUID(string &bios)
{
    if (access("/usr/sbin/dmidecode", F_OK)) {
        return -1;
    }

    FILE *pp = NULL;
    char uuid[40] = {0};

    if ((pp = popen("/usr/sbin/dmidecode -s system-uuid|grep -v \"#\"", "r")) == NULL) {
        return -1;
    }

    if (fgets(uuid, sizeof(uuid)-1, pp) == NULL) {
        pclose(pp);
        return -1;
    }

    if (strlen(uuid) < 36) {
        pclose(pp);
        return -1;
    }

    if (uuid[strlen(uuid)-1] == '\n') {
        uuid[strlen(uuid)-1] = '\0';
    }

    pclose(pp);
    bios = uuid;
    return AvaildGUID(uuid);
}

int ClientInfo::GetDiskUUID(string &diskuuid)
{
    if (access("/sbin/blkid", F_OK)) {
        return -1;
    }

    FILE *pp = NULL;
    char uuid[40] = {0};

    if ((pp = popen("/sbin/blkid|grep -i uuid|head -n 1|cut -d \\\" -f 2", "r")) == NULL) {
        return -1;
    }

    if (fgets(uuid, sizeof(uuid)-1, pp) == NULL) {
        pclose(pp);
        return -1;
    }

    if (uuid[strlen(uuid)-1] == '\n') {
        uuid[strlen(uuid)-1] = '\0';
    }

    pclose(pp);
    diskuuid = uuid;
    return AvaildGUID(uuid);
}

int ClientInfo::GetSystem(string& system) {
    system = "linux";
    return 0;
}

string ClientInfo::GetDescription() {
    string data;
    GetClientName(data);

    string data2;
    GetClientArch(data2);

    string data3;
    GetCurrentUser(data3);

    string data4;
    GetLocalIP(data4);

    string data5;
    GetMacAddr(data5);
    return data + data2 + data3 + data4 + data5;
}

std::string ClientInfo::GetOSReleaseName() {
    std::string ret;
    if (access("/etc/os-release", R_OK|F_OK) == 0) {
        FILE *f = fopen("/etc/os-release", "r");
        if (!f) return "";

        char buffer[256] = {0};

        while (!feof(f)) {
            memset(buffer, 0, 256);
            fgets(buffer, 255, f);
            if (buffer[strlen(buffer)-1] == '\n') {
                buffer[strlen(buffer)-1]='\0';
            }

            std::vector<std::string> datas;
            Utils::String::Split(buffer, "=", &datas);
            if (datas.size() > 1) {
                if (datas[0] == "NAME") {
                    ret = datas[1];
                    break;
                }
            }
        }
        fclose(f);
    }
    return ret;
}
