#include "clientinfo.h"
#include "commonapi.h"
#include "baseconfig.h"
#include "cinifile.h"

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
#include <vector>
#include <fstream>

map<string, string> ClientInfo::mIPMacs;
string ClientInfo::IP = "";
string ClientInfo::MAC = "";

int ClientInfo::GetMacAddr(string& out, const string& ip) {
	// 获取获取失败, 尝试再获取一次. 
	int  bAgain = true;

	AGAIN_GetMacAddr:
    if (mIPMacs.size() == 0) {
        if (InitEthInfo() <= 0) {
            return -1;
        }
    }

    map<string, string>::iterator iter = mIPMacs.find(ip);
    if (iter != mIPMacs.end()) {
        out = MAC = iter->second;
        return 0;
    }

	if(bAgain) {
		bAgain = false;
		mIPMacs.clear();
		goto AGAIN_GetMacAddr;
	}

    return -1;
}

string ClientInfo::GetMac(const string& eth) {
    if (eth.empty()) {
        return "00:00:00:00:00:00";
    }

    int   sock;
    struct   ifreq   ifr;
    char mac[48] = {0};

    sock = socket(AF_INET,   SOCK_DGRAM,   0);
    if(sock < 0) {
        return "00:00:00:00:00:00";
    }

    strcpy(ifr.ifr_name, eth.c_str());
    if( ioctl(sock, SIOCGIFHWADDR, &ifr ) == 0) {
        for(int i = 0; i < 6; i++){
            sprintf(mac+3*i, "%02X:", (unsigned char)ifr.ifr_hwaddr.sa_data[i]);
        }
        if (strlen(mac) > 0) {
            mac[strlen(mac)-1] = '\0';
        }
    }
    close(sock);
    return mac;
}

int ClientInfo::InitEthInfo() {
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

        string mac = GetMac(ifreq->ifr_name);
        if (!mac.empty()) {
			LOG_GLTX(">> init ethinfo, get mac -->%s, %s", ip, mac.c_str());
            mIPMacs.insert(make_pair(ip, mac));
        }
        ifreq++;
    }
    return mIPMacs.size();
}

int ClientInfo::GetLocalIP(string &outIP, const string& centerip) {
    LOG_GLTX("into Get ip addr center is %s", centerip.c_str());
    if (!IP.empty()) {
        LOG_GLTX("get cache ip %s", IP.c_str());
        outIP = IP;
        return 0;
    }

    while(mIPMacs.size() == 0) {
        if (InitEthInfo() <=0 ) {
            return -1;
        }
    }

    LOG_GLTX("get %d IP address", mIPMacs.size());

    if (mIPMacs.size() > 0 && centerip.empty()) {
        outIP = mIPMacs.begin()->first;
        return 0;
    }

    vector<string> centerseg;
    Utils::String::Split(centerip, ".", &centerseg);

    int weight = 0;
    map<string, string>::iterator iter;
    for (iter=mIPMacs.begin(); iter!=mIPMacs.end(); ++iter) {
        vector<string> dest;
        Utils::String::Split(iter->first, ".", &dest);

        int myweight = 0;
        for (int i=0; i<centerseg.size(); ++i) {
            if (centerseg[i] == dest[i]) {
                myweight++;
            }
            else {
                break;
            }
        }

        if (myweight >= weight) {
            LOG_GLTX("get %s and center is %s", iter->first.c_str(), centerip.c_str());

            outIP = iter->first;
            if (!centerip.empty()) {
                IP = iter->first.c_str();
            }
            weight = myweight;
        }
    }

    return 0;
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
    std::string ret;
    if (Utils::Path::FileAvailed("/etc/.systeminfo")) {
        CINIFile inifile("/etc/.systeminfo", true);
        inifile.readitemstr("SYSTEM", "NAME", ret, "");
        if (ret.empty()) {
            inifile.readitemstr("SYSTEM", "Name", ret, "");
        }
    }
    else if (Utils::Path::FileAvailed("/etc/.productinfo")) {
        ifstream ifs("/etc/.productinfo");
        if (ifs) {
            std::string line;
            while(std::getline(ifs, line)) {
                ret += line;
            }
            ret = Utils::String::TrimAll(ret);
            if (Utils::String::StartsWith(ret, "-")) {
                ret = ret.substr(1);
            }
            if (Utils::String::EndsWith(ret, "-")) {
                ret = ret.substr(0, ret.size()-1);
            }
        }
    }

    if (!ret.empty()) {
        outName = ret;
        return 0;
    }

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
    GetLocalIP(data4, "");
    if (data4.empty()) {
        srand((int)time(NULL));
        data4 = Utils::String::itoa(rand()%255) + "."
                + Utils::String::itoa(rand()%255) + "."
                + Utils::String::itoa(rand()%255) + "."
                + Utils::String::itoa(rand()%255);
    }

    string data5;
    GetMacAddr(data5, data4);
    if (data5.empty()) {
        char temp[128] = {0};
        srand((int)time(NULL));
        sprintf(temp, "%02x:%02x:%02x:%02x:%02x:%02x",
                rand()%0xff, rand()%0xff, rand()%0xff, rand()%0xff, rand()%0xff, rand()%0xff);
        data5 = temp;
    }

    return data + data2 + data3 + data4 + data5;
}

std::string ClientInfo::GetOSReleaseName() {
    std::string ret;
    if (Utils::Path::FileAvailed("/etc/.productinfo")) {
        ifstream ifs("/etc/.productinfo");
        if (ifs) {
            std::string line;
            while(std::getline(ifs, line)) {
                ret += line;
            }
            ret = Utils::String::TrimAll(ret);
            if (Utils::String::StartsWith(ret, "-")) {
                ret = ret.substr(1);
            }
            if (Utils::String::EndsWith(ret, "-")) {
                ret = ret.substr(0, ret.size()-1);
            }
        }
    }
    if (!ret.empty()) {
        return ret;
    }

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

void   ClientInfo::SetLocalIP(const char * szIp)
{
	IP = szIp;
}

bool   ClientInfo::CheckLocalMac(const char * szMac)
{
    if(mIPMacs.empty())
	{
		// get error return ok. 
		InitEthInfo();
        if(mIPMacs.empty()) return true;
    }

    for(std::map<std::string, std::string>::iterator it = mIPMacs.begin(); it != mIPMacs.end(); it++) 
	{
        if(strcmp(szMac, it->second.c_str()) == 0) return true;
    }

    return false;
}
