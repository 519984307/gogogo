#include "commonapi.h"
#include "commonapi_global.h"
#ifndef COMMON_ZIP_API
extern "C" {
    #include "crypt/crc32.h"
}
#endif

#ifdef COMMON_HASH_API
    #include "crypt/rscrypt.h"
#endif

#include "baseconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <errno.h>
#include <fstream>
#include <signal.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <fnmatch.h>
#include <pwd.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <string.h>

using namespace std;

#ifdef COMMON_7Z_API
    #define IN_BUF_SIZE     (1 << 16)
    #define OUT_BUF_SIZE    (1 << 16)

    #include "lzma/CpuArch.h"
    #include "lzma/Alloc.h"
    #include "lzma/7zFile.h"
    #include "lzma/7zVersion.h"
    #include "lzma/LzmaDec.h"
    #include "lzma/LzmaEnc.h"
    #include "lzma/7zAlloc.h"
    #include "lzma/7z.h"
    #include "lzma/7zBuf.h"
    #include "lzma/7zCrc.h"
    #include "lzma/7zVersion.h"
#endif

#ifdef COMMON_ZIP_API
    #include "zip/unixzip.h"
#endif

#ifdef COMMON_HASH_API
#ifdef MANUAL_PACK_
    #include "md5.h"
    #include "base64.h"
    #include "rc4.h"
#else
    #include "crypt/md5.h"
    #include "crypt/base64.h"
    #include "crypt/rc4.h"
    #include "zlib.h"
#endif

unsigned char ToHex(unsigned char x)
{
   return  x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x)
{
   unsigned char y;
   if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
   else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
   else if (x >= '0' && x <= '9') y = x - '0';
   return y;
}

std::string Utils::Hash::GetMD5(const std::string& s) {
    MD5 md5(s);
    return md5.hexdigest();
}

std::string Utils::Hash::GetFileMD5(const std::string& path) {
    unsigned char data[1024];
    FILE *file = fopen(path.c_str(), "rb");
    if (NULL == file) {
        return "";
    }

    MD5 md5;

    while (1) {
        int readed = fread(data, 1, 1024, file);
        if (readed <= 0) {
            fclose(file);
            break;
        }

        md5.update(data, readed);
    }

    md5.finalize();
    return md5.hexdigest();
}

std::string Utils::Hash::ESMEncrypt(const std::string& s) {
    return base64_encode(RC4_Encrypt(s, RC4_PASSWORD));
}

std::string Utils::Hash::ESMDecrypt(const std::string& s) {
    return RC4_Decrypt(base64_decode(s),  RC4_PASSWORD);
}

std::string Utils::Hash::Base64Encode(const std::string& s) {
    return base64_encode(s);
}

std::string Utils::Hash::Base64Decode(const std::string& s) {
    return base64_decode(s);
}

std::string Utils::Hash::RC4Encrypt(const std::string& data) {
    return RC4_Encrypt(data, RC4_PASSWORD);
}

std::string Utils::Hash::RC4Decrypt(const std::string& data) {
    return RC4_Decrypt(data, RC4_PASSWORD);
}

std::string Utils::Hash::UrlEncode(const std::string& str)
{
   std::string strTemp = "";
   size_t length = str.length();
   for (size_t i = 0; i < length; i++)
   {
       if (isalnum((unsigned char)str[i]) ||
           (str[i] == '-') ||
           (str[i] == '_') ||
           (str[i] == '.') ||
           (str[i] == '~'))
           strTemp += str[i];
       else if (str[i] == ' ')
           strTemp += "+";
       else
       {
           strTemp += '%';
           strTemp += ToHex((unsigned char)str[i] >> 4);
           strTemp += ToHex((unsigned char)str[i] % 16);
       }
   }
   return strTemp;
}

std::string Utils::Hash::UrlDecode(const std::string& str)
{
   std::string strTemp = "";
   size_t length = str.length();
   for (size_t i = 0; i < length; i++)
   {
       if (str[i] == '+') strTemp += ' ';
       else if (str[i] == '%')
       {
           unsigned char high = FromHex((unsigned char)str[++i]);
           unsigned char low = FromHex((unsigned char)str[++i]);
           strTemp += high*16 + low;
       }
       else strTemp += str[i];
   }
   return strTemp;
}

unsigned int Utils::Hash::FileCRC32(const std::string& file) {
    int nread;
    unsigned char buf[1024] = {0};
    unsigned int crc = 0;

    FILE *fin = fopen(file.c_str(), "rb");
    if(NULL == fin) {
        return 0;
    }

    while((nread = fread(buf,1,1024,fin)) > 0) {
#ifdef COMMON_ZIP_API
        crc = crc32(crc,buf,nread);
#else
        crc = rs_crc32(buf, nread);
#endif
    }
    fclose(fin);
    return crc;
}

unsigned int Utils::Hash::StrCRC32(const std::string& data) {
    unsigned int crc = 0;
#ifdef COMMON_ZIP_API
        crc = crc32(crc, (unsigned char*)data.c_str(), data.size());
#else
        crc = rs_crc32((unsigned char*)data.c_str(), data.size());
#endif
     return crc;
}


std::string Utils::Hash::RSEntrypt(const std::string& data) {
    size_t outlen = data.size() * 2;
    char *out = new char[outlen];
    memset(out, 0, outlen);
    if (RsUpgradeEncrypt(out, outlen, data.c_str()) > 0) {
        string ret = string(out);
        delete[] out;
        return ret;
    }
    return "";
}

std::string Utils::Hash::RSDecrypt(const std::string& data) {
    size_t outlen = data.size();
    char *out = new char[outlen];
    memset(out, 0, outlen);
    if (RsUpgradeDecrypt(out, outlen, data.c_str()) > 0) {
        string ret = string(out);
        delete[] out;
        return ret;
    }
    return "";
}

#endif



#ifdef COMMON_STRING_API
std::string Utils::String::TrimLeft(std::string &s)
{
    const std::string &space =" \f\n\t\r\v" ;
    return s.erase(0,s.find_first_not_of(space));
}

std::string Utils::String::TrimRight(std::string &s)
{
    const std::string &space =" \f\n\t\r\v" ;
    return s.erase(s.find_last_not_of(space)+1);
}

std::string Utils::String::TrimAll(std::string &s)
{
    TrimLeft(s);
    TrimRight(s);
    return s;
}


void Utils::String::Split(const std::string& s, const std::string& delim,std::vector< std::string >* ret)
{
    size_t last = 0;
    size_t index=s.find_first_of(delim,last);
    if (index == std::string::npos) {
        ret->push_back(s);
        return;
    }

    while (index!=std::string::npos) {
        string temp = s.substr(last,index-last);
        if (!temp.empty()) {
            ret->push_back(temp);
        }
        last=index+1;
        index=s.find_first_of(delim,last);
    }
    if (s.size()-last>0) {
        string temp = s.substr(last,s.size()-last);
        if (!temp.empty()) {
            ret->push_back(temp);
        }
    }
}

bool Utils::String::StartsWith(const std::string& source, const std::string& fix) {
    return source.compare(0, fix.size(), fix) == 0;
}

bool Utils::String::EndsWith(const std::string& source, const std::string& tail) {
    if (source.size() < tail.size()) {
        return false;
    }
    return source.compare(source.size() - tail.size(), tail.size(), tail) == 0;
}

void Utils::String::itoa(int i, std::string& a) {
    char temp[32];
    sprintf(temp, "%d", i);
    a = temp;
}

const std::string Utils::String::itoa(int i) {
    char temp[32];
    sprintf(temp, "%d", i);
    return temp;
}

std::string Utils::String::ToLower(string str) {
   for(unsigned i = 0; i < str.length(); i++) {
       str[i] = std::tolower(str[i]);
   }
   return str;
}

std::string Utils::String::ToUpper(string str) {
   for(unsigned i = 0; i < str.length(); i++) {
       str[i] = std::toupper(str[i]);
   }
   return str;
}

int Utils::String::trimblank(char *szStr,int nMode)
{
    int nLen,i,j,nFlag;
    char szTempBuf[512];

    if(szStr==NULL)return -1;
    nLen=strlen(szStr);
    if(nLen==0 || nLen >511)return -1;
    strcpy(szTempBuf,szStr);
    j=0;

    //trim right blank
    if(nMode==1 || nMode==2) {
        for(i=nLen-1;i>=0;i--) {
            if(szTempBuf[i]==' ' || szTempBuf[i]=='\t' || szTempBuf[i]==0x0a || szTempBuf[i]==0x0d) {
                szTempBuf[i]='\0';
            }
            else {
                break;
            }
        }
    }

    nLen=strlen(szTempBuf);
    if (nLen==0) {
        szStr[0]='\0';
        return 0;
    }

    //trim left
    nFlag=0;
    j=0;
    for(i=0;i<=nLen;i++) {
        if((szTempBuf[i]==' ' ||szTempBuf[i]=='\t' || szTempBuf[i]=='\n') && nFlag==0) {
            continue;
        }
        else {
            nFlag=1;
            szStr[j++]=szTempBuf[i];
        }
    }
    return 0;
}

int Utils::String::CompareVersion(const std::string& source, const std::string& dest) {
    vector<string> src_section;
    vector<string> dst_section;

    Split(source, ".", &src_section);
    Split(dest, ".", &dst_section);

    int result = src_section.size() - dst_section.size();
    if (result) {
        return result>0?1:-1;
    }

    for (int i=0; i<(int)src_section.size(); ++i) {
        result = atoi(src_section[i].c_str()) - atoi(dst_section[i].c_str());
        if (result) {
            return result>0?1:-1;
        }
    }

    return 0;
}

std::string Utils::String::RandomString(int len) {
    const char CCH[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    srand((unsigned)time(NULL));
    char *ch = new char[len + 1];
    memset(ch, 0, len+1);
    for (int i=0; i<len; ++i) {
        int x = rand()/(RAND_MAX/(sizeof(CCH)-1));
        ch[i] = CCH[x];
    }
    string rstr = string(ch);
    delete[] ch;
    return rstr;
}

#endif

#ifdef COMMON_OS_API
bool Utils::Date::NowInTimeArea(const std::string& start, const std::string& end) {
    char out[128] = {0};
    long now = GetTimeStamp();
    struct tm *local = (struct tm*)localtime(&now);
    strftime(out, 128, "%H:%M", local);

    return out>=start && out<=end;
}
long Utils::Date::GetTimeStamp() {
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);
    return tv.tv_sec;
}

std::string Utils::Date::FormatTimeStamp(time_t timeStamp) {
    char out[128] = {0};
    struct tm *local = localtime(&timeStamp);
    strftime(out, 128, "%Y-%m-%d %H:%M:%S", local);
    return out;
}

int Utils::OS::Read(int fd, int len, unsigned char* data) {
    int readed = 0;
    do {
        int r = read(fd, data+readed, len-readed);
        if (r <= 0) {
            printf("r=%d, Read\n", r);
            break;
        }
        readed += r;
    }while(readed<len);
    return readed;
}

int Utils::OS::Write(int fd, int len, unsigned char *data) {
    int writed = 0;
    do {
        int w = write(fd, (void*)(data+writed), len-writed);
        if (w < 0) {
            printf("w<0, Write(%d)\n", w);
            break;
        }
        writed += w;
    }while(writed<len);
    return writed;
}

int Utils::OS::SendLocal(const char *domain, const unsigned char *data, int len, int *out) {
    int ret;
    int socket_fd;

    static struct sockaddr_un srv_addr;

    socket_fd = socket(AF_UNIX,SOCK_STREAM,0);
    if(socket_fd < 0) {
        return 1;
    }
    srv_addr.sun_family = AF_UNIX;
    strcpy(srv_addr.sun_path,domain);

    ret = connect(socket_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
    if(ret == -1) {
        close(socket_fd);
        return 1;
    }

    if (Utils::OS::Write(socket_fd, len, (unsigned char*)data) != len) {
        close(socket_fd);
        return -1;
    }

    if (Utils::OS::Read(socket_fd, 4, (unsigned char*)out) != 4) {
        close(socket_fd);
        return -1;
    }
    close(socket_fd);

    return 0;
}

int Utils::OS::SendLocal(const short port, const unsigned char *data, int len) {
    int socket_fd;

    static struct sockaddr_in srv_addr;

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_fd < 0) {
        return 1;
    }
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port   = htons(port);    //
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if((len = sendto(socket_fd, data, len, 0 ,(struct sockaddr*)&srv_addr, sizeof(struct sockaddr))) < 0) {
        return 1;
    }
    close(socket_fd);

    return 0;
}

int Utils::OS::CheckModState(const char *modname) {
    FILE *fpPipe=NULL;
    char szProcess[512];

    sprintf(szProcess,"lsmod|grep %s", modname);

    if((fpPipe=popen(szProcess,"r"))==NULL) {
        return -1;
    }

    char szBuf[512];
    while(fgets(szBuf,sizeof(szBuf)-1,fpPipe)!=NULL)
    {
        String::trimblank(szBuf,2);
        if( !strstr(szBuf,modname)) {
            continue;
        }

        char *szSavePtr;
        char *token;
        char *szStr=szBuf;
        char szPid[256],szTmpCmd[256];
        memset(szPid,0,sizeof(szPid));
        memset(szTmpCmd,0,sizeof(szTmpCmd));

        int nCmdItems=7;
        int i;
        for(i=0;i<=nCmdItems;i++,szStr=NULL)
        {
            token=strtok_r(szStr," \t",&szSavePtr);
            if(!token)
                break;

            if (strcmp(token, modname) == 0) {
                return 1;
            }
        }
    }
    pclose(fpPipe);

    return 0;
}

std::string Utils::OS::GetFileOwner(const std::string& filepath) {
    const char *LSOF[] = {"/usr/bin/lsof", "/bin/lsof", "/usr/local/bin/lsof", "/usr/sbin/lsof"};
    int valid = 0;
    for (;valid<(int)(sizeof(LSOF)/sizeof(const char*)); ++valid) {
        if (Utils::Path::FileAvailed(LSOF[valid])) {
            break;
        }
    }

    if (valid >= (int)(sizeof(LSOF)/sizeof(const char*))) {
        return "";
    }

    char command[1024] = {0};
    sprintf(command, "%s %s", LSOF[valid], filepath.c_str());

    FILE *fp = NULL;
    if((fp=popen(command,"r"))==NULL) {
        return "";
    }

    string process;
    do {
        memset(command, 0, 1024);
        if (fgets(command, sizeof(command)-1, fp) == NULL) {
            pclose(fp);
            fp = NULL;
            break;
        }

        char * find = strchr(command, ' ');
        if (find) {
            if (0 == strncmp(command, "COMMAND", find-command)) {
                continue;
            }
            else {
                command[find-command] = '\0';
                process = command;
                break;
            }
        }
    }while(!feof(fp));

    if (fp) {
        pclose(fp);
    }
    return process;
}


bool Utils::OS::GetProcState(const char *lockfile) {
    bool state = false;
    if (Path::FileAvailed(lockfile)) {
        FILE *file = fopen(lockfile, "rb+");
        if (!file) {
            return false;
        }
//#if defined (COMMON_LINUX)
        if (flock(file->_fileno, LOCK_EX|LOCK_NB) != 0) {
            state = true;
        }
//#endif
        fclose(file);
    }
    return state;
}

bool Utils::OS::GetProcState(const std::string& folder, bool check_owner) {
    char temp[128];
    snprintf(temp, 127, "%s_%d.lock", GetProcessName().c_str(), getuid());

    string lockfile = Utils::Path::JoinPath(folder.c_str(), temp, NULL);
    bool lock = GetProcState(lockfile.c_str());
    if (lock && check_owner) {
        string owner = GetFileOwner(lockfile);
        string current = GetProcessName();

        if (owner == current) {
            return true;
        }
        else {
            unlink(lockfile.c_str());
            lock = false;
        }
    }
    return lock;
}

FILE* Utils::OS::LockProcState(const char *folder) {
    char temp[128];
    snprintf(temp, 127, "%s_%d.lock", GetProcessName().c_str(), getuid());

    std::string fs = Path::JoinPath(folder, temp, NULL);
    FILE *file = fopen(fs.c_str(), "wb+");
    if (file) {
//#if defined (COMMON_LINUX)
        if (flock(file->_fileno, LOCK_EX|LOCK_NB) == 0) {
            return file;
        }
//#endif
        fclose(file);
    }

    return NULL;
}

void Utils::OS::ReleaseProcState(FILE *lock) {
    if (lock) {
//#if defined (COMMON_LINUX)
        flock(lock->_fileno, LOCK_UN);
        fclose(lock);
//#endif
    }
}

int Utils::OS::CheckProcessState(const char *process) {
    FILE *fpPipe=NULL;
    char szProcess[512];
    pid_t pnPid[10];

    sprintf(szProcess,"ps -ef|grep %s", process);

    if((fpPipe=popen(szProcess,"r"))==NULL)
        return -1;

    int nTot=0;

    char szBuf[512];
    char szGrepFlag[512];
    sprintf(szGrepFlag,"grep %s", process);

    while(fgets(szBuf,sizeof(szBuf)-1,fpPipe)!=NULL)
    {
        String::trimblank(szBuf,2);
        if( !strstr(szBuf,process) || strstr(szBuf,szGrepFlag) )
            continue;

        // get filed two for pid

        char *szSavePtr;
        char *token;
        char *szStr=szBuf;
        char szPid[256],szTmpCmd[256];
        memset(szPid,0,sizeof(szPid));
        memset(szTmpCmd,0,sizeof(szTmpCmd));

        int nItemsPid=1;
        int nCmdItems=7;
        int i;
        for(i=0;i<=nCmdItems;i++,szStr=NULL)
        {
            token=strtok_r(szStr," \t",&szSavePtr);
            if(!token)
                break;
            if(i==nItemsPid)
            {
                strncpy(szPid,token,sizeof(szPid)-1);
            }
            else if(i==nCmdItems)
                strncpy(szTmpCmd,token,sizeof(szTmpCmd)-1);
        }
        if(szPid[0]=='\0' || szTmpCmd[0]=='\0')
            continue;

        char *endptr;
        long int pid=strtol(szPid,&endptr,10);
        if( (endptr && *endptr!=0)|| pid<=0)
            continue;
        // getcmd
        pnPid[nTot++]=pid;
        if(nTot >= 10)
            break;
    }
    pclose(fpPipe);

    return nTot;
}

int Utils::OS::KillProc(const char *app, int signal) {
    int state = Utils::OS::CheckProcessState(app);
    if (state > 0) {
        vector<int> pids;
        if (0 == Utils::OS::GetPid(app, pids)) {
            vector<int>::iterator iter;
            for (iter=pids.begin();iter!=pids.end(); ++iter) {
                int status = kill(*iter, signal);
                wait(&status);
            }
        }
    }
    return state;
}

int Utils::OS::KillProc(pid_t pid, int signal) {
    int status = kill(pid, signal);
    wait(&status);
    return 0;
}

std::string Utils::OS::GetProcessName() {
    char strProcessPath[1024] = {0};
    if(readlink("/proc/self/exe", strProcessPath,1024) <=0) {
        return "";
    }

    return string(strrchr(strProcessPath, '/') + 1);
}

uid_t Utils::OS::GetProcessUID(pid_t pid) {
    char buffer[128];
    sprintf(buffer, "/proc/%d/status", pid);
    ifstream ifs(buffer);
    if (ifs) {
        std::string line;
        while(std::getline(ifs, line)) {
            if (String::StartsWith(line, "Uid")) {
                int u1, u2, u3, u4;
                sscanf(line.c_str(), "Uid:\t%d\t%d\t%d\t%d", &u1, &u2, &u3, &u4);
                return u1;
            }
        }
    }
    return 0;
}

//int Utils::OS::GetPid(const char *process_path, char *pid) {
//    DIR *dir;
//    struct dirent *ptr;
//    FILE *fp;
//    char filepath[128];
//    char filetext[128];
//    int ret = -1;
//    dir = opendir("/proc");
//    if (NULL != dir) {
//        while(NULL != (ptr=readdir(dir))) {
//            if ((0==strcmp(ptr->d_name, ".")) || (0==strcmp(ptr->d_name,".."))) {
//                continue;
//            }

//            bool isdir = DT_DIR==ptr->d_type;
//            if (!isdir && DT_UNKNOWN==ptr->d_type) {
//                snprintf(filepath, 1023, "%s/%s", "/proc", ptr->d_name);
//                struct stat buf;
//                stat(filepath, &buf);
//                isdir = bool(buf.st_mode & S_IFDIR);
//            }


//            if (!isdir) {
//                continue;
//            }

//            sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);
//            fp = fopen(filepath, "r");
//            if (NULL != fp) {
//                memset(filetext,0,128);
//                fread(filetext, 1, 127, fp);
//                if (filetext == strstr(filetext, process_path)) {
//                    strcpy(pid, ptr->d_name);
//                    ret = 0;
//                    break;
//                }
//                fclose(fp);
//            }
//        }
//        closedir(dir);
//    }
//    return ret;
//}

int Utils::OS::GetPid(const char *process_path, vector<int>& pids) {
    DIR *dir;
    struct dirent *ptr;
    FILE *fp;
    char filepath[128];
    char filetext[128];

    dir = opendir("/proc");
    if (NULL != dir) {
        while(NULL != (ptr=readdir(dir))) {
            if ((0==strcmp(ptr->d_name, ".")) || (0==strcmp(ptr->d_name,".."))) {
                continue;
            }
            bool isdir = false;
#if defined (COMMON_LINUX)
            isdir = DT_DIR==ptr->d_type;
            if (!isdir && DT_UNKNOWN==ptr->d_type) {
                snprintf(filepath, 1023, "%s/%s", "/proc", ptr->d_name);
                struct stat buf;
                stat(filepath, &buf);
                isdir = bool(buf.st_mode & S_IFDIR);
            }
#else
            snprintf(filepath, 1023, "%s/%s", "/proc", ptr->d_name);
            struct stat buf;
            stat(filepath, &buf);
            isdir = bool(buf.st_mode & S_IFDIR);
#endif
            if (!isdir) {
                continue;
            }

            sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);
            fp = fopen(filepath, "r");
            if (NULL != fp) {
                memset(filetext,0,128);
                fread(filetext, 1, 127, fp);
                if (filetext == strstr(filetext, process_path)) {
                    pids.push_back(atoi(ptr->d_name));
                }
                fclose(fp);
            }
        }
        closedir(dir);
    }
    return 0;
}

int Utils::OS::GetPid(const char *process_path, std::vector<pair<int, string> >& pids) {
    DIR *dir;
    struct dirent *ptr;
    FILE *fp;
    char filepath[128];
    char filetext[128];

    dir = opendir("/proc");
    if (NULL != dir) {
        while(NULL != (ptr=readdir(dir))) {
            if ((0==strcmp(ptr->d_name, ".")) || (0==strcmp(ptr->d_name,".."))) {
                continue;
            }

            bool isdir = false;
#if defined (COMMON_LINUX)
            isdir = DT_DIR==ptr->d_type;
            if (!isdir && DT_UNKNOWN==ptr->d_type) {
                snprintf(filepath, 1023, "%s/%s", "/proc", ptr->d_name);
                struct stat buf;
                stat(filepath, &buf);
                isdir = bool(buf.st_mode & S_IFDIR);
            }
#else
            snprintf(filepath, 1023, "%s/%s", "/proc", ptr->d_name);
            struct stat buf;
            stat(filepath, &buf);
            isdir = bool(buf.st_mode & S_IFDIR);
#endif

            if (!isdir) {
                continue;
            }

            sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);
            fp = fopen(filepath, "r");
            if (NULL != fp) {
                memset(filetext,0,128);
                fread(filetext, 1, 127, fp);
                if (filetext == strstr(filetext, process_path)) {
                    pids.push_back(make_pair(atoi(ptr->d_name), filetext + strlen(process_path) + 1));
                }
                fclose(fp);
            }
        }
        closedir(dir);
    }
    return 0;
}

int Utils::OS::GetUsers(std::vector<string> users) {
    ifstream ifs("/etc/passwd");
    if (!ifs) {
        return -1;
    }

    std::string line;
    while(std::getline(ifs, line)) {
        std::vector<std::string> flags;
        Utils::String::Split(line, ":", &flags);
        if (strncmp(flags[5].c_str(), "/home", 5) == 0) {
            if (Utils::Path::PathExists(flags[5])) {
                users.push_back(flags[5].substr(6));
            }
        }
    }
    return users.size();
}

std::string Utils::OS::GetUserFromUID(uid_t uid) {
    struct passwd *pw_ptr;
    if((pw_ptr = getpwuid(uid))) {
        return pw_ptr->pw_name;
    }
    return "";
}

int Utils::OS::GetKernelVersions(pair<string, string>& version) {
    ifstream ifs("/proc/version");
    if (!ifs) {
        return -1;
    }

    std::string line;
    while(std::getline(ifs, line)) {
        std::vector<std::string> flags;
        Utils::String::Split(line, " ", &flags);
        if (flags[1] ==  "version") {
            version.first = flags[2].c_str();
            break;
        }
    }

    size_t verpos = line.find("#");
    if (verpos != string::npos) {
        version.second = line.substr(verpos);
    }
    return 0;
}

string Utils::OS::GetMachineArch() {
  struct utsname uts;
  if (uname(&uts) < 0) {
    return "";
  }
  return uts.machine;
}

string Utils::OS::GetKernelVersionString() {
    ifstream ifs("/proc/version");
    if (!ifs) {
        return "";
    }

    string version;
    std::string line;
    while(std::getline(ifs, line)) {
        std::vector<std::string> flags;
        Utils::String::Split(line, " ", &flags);
        if (flags[1] ==  "version") {
            version = flags[2].c_str();
            break;
        }
    }
    return version;
}

int Utils::OS::CheckModuleState(const string &module) {
    ifstream ifs("/proc/modules");
    if (!ifs) {
        return -1;
    }

    std::string line;
    while(std::getline(ifs, line)) {
        std::vector<std::string> flags;
        Utils::String::Split(line, " ", &flags);
        if (module == flags[0]) {
            return atoi(flags[2].c_str());
        }
    }
    return -1;
}

int Utils::OS::CopyFile(const char *source, const char *dest) {
    FILE *fp1 = fopen(source, "r");
    FILE *fp2 = fopen(dest, "w");

    char buff[1024] = {0};
    int count = 0;
    while((count = fread(buff, 1, 1024, fp1)) != 0) {
        fwrite(buff, 1, count, fp2);
    }

    fclose(fp1);
    fclose(fp2);

    return 0;
}

int Utils::OS::GetFileBits(const char *file) {
    // https://blog.csdn.net/duanbeibei/article/details/51615788
    FILE *f = fopen(file, "rb");
    if (f) {
        unsigned char data[2];
        fseek(f, 4, SEEK_SET);
        fread(data, 1, 2, f);

        int bits = -1;
        if (data[0] == 0x02 && data[1] == 0x1) {
            bits = 64;
        }
        else if (data[0] == 0x01 && data[1] == 0x1) {
            bits = 32;
        }
        fclose(f);
        return bits;
    }
    return -1;
}

bool Utils::OS::GetLinkEntry(const std::string& link, string& entry) {
    char buf[1024];
    ssize_t len;

    if((len = readlink(link.c_str(), buf, 1024 - 1)) != -1) {
        buf[len] = '\0';
        entry = buf;
    }
    return len>0;
}

int Utils::OS::GetLibraryFolder(std::vector<std::string>& folders) {
#if defined (COMMON_LINUX)
    string ldconfig = "/etc/ld.so.conf";
    ifstream ifs(ldconfig.c_str());
    string line;
    while(std::getline(ifs, line)) {
        string temp = String::TrimAll(line);
        if (String::StartsWith(temp, "#")) {
            continue;
        }
        else if (String::StartsWith(temp, "include")) {
            vector<string> ps;
            String::Split(temp, " ", &ps);
            if (ps.size() > 1) {
                string sub = String::TrimAll(ps[1]);
                string dpath, dfile;
                Path::SplitPath(sub, dpath, dfile);
                if (!Path::PathExists(dpath)) {
                    continue;
                }

                struct dirent *tmp = NULL;
                DIR *dir = opendir(dpath.c_str());
                while (NULL != (tmp = readdir(dir))) {
                    if (DT_REG == tmp->d_type) {
                        if (0 == fnmatch(dfile.c_str(), tmp->d_name,  FNM_PATHNAME|FNM_PERIOD)) {
                            string conf = Path::JoinPath(dpath.c_str(), tmp->d_name, NULL);
                            ifstream ifsub(conf.c_str());
                            string confline;
                            while(std::getline(ifsub, confline)) {
                                string vb = String::TrimAll(confline);
                                if (Path::PathExists(vb)) {
                                    folders.push_back(vb);
                                }
                            }
                            ifsub.close();
                        }
                    }
                }
                closedir(dir);
            }
        }
        else {
            if (!temp.empty()) {
                if (Path::PathExists(temp)) {
                    folders.push_back(temp);
                }
            }
        }
    }
    ifs.close();
#endif
    return 0;
}

int Utils::OS::GetCPUKernelCount() {
#if defined (COMMON_LINUX)
    return get_nprocs();
#else
    const char *command = "lsdev -Cc processor";

    FILE *fpPipe=NULL;
    if((fpPipe=popen(command, "r"))==NULL) {
        return -1;
    }

    int count = 0;
    char szBuf[512];
    while(fgets(szBuf,sizeof(szBuf)-1, fpPipe)!=NULL) {
        count++;
    }
    return count;
#endif

}

std::string Utils::OS::GetSoPath(const std::string& lib, int bits, std::vector<std::string>& ldconfig) {
#if defined (COMMON_LINUX)
    vector<string> alltype;
    alltype.push_back(lib);
    alltype.push_back(lib + ".?");
    for (int i=0; i<(int)alltype.size(); ++i) {
        vector<string>::iterator iter;
        for (iter=ldconfig.begin(); iter!=ldconfig.end(); ++iter) {
            struct dirent *tmp = NULL;
            DIR *dir = opendir(iter->c_str());
            while (NULL != (tmp = readdir(dir))) {
                if (0 == fnmatch(alltype[i].c_str(), tmp->d_name,  FNM_PATHNAME|FNM_PERIOD)) {
                    string conf = Path::JoinPath(iter->c_str(), tmp->d_name, NULL);
                    switch (tmp->d_type) {
                    case DT_LNK: {
                        string real;
                        while (GetLinkEntry(conf, real)) {
                            conf = Path::JoinPath(iter->c_str(), real.c_str(), NULL);
                        }
                    }
                    case DT_REG: {
                        int sb = GetFileBits(conf.c_str());
                        if (sb == bits) {
                            closedir(dir);
                            return conf;
                        }
                    }
                    }
                }
            }
            closedir(dir);
        }
    }
#endif
    return "";
}

std::string Utils::OS::GetCmdResult(const char *cmd)
{
    string result;
    char buf_ps[1024];
    char ps[1024]={0};
    FILE *ptr;
    strcpy(ps, cmd);
    if((ptr=popen(ps, "r"))!=NULL)
    {
        while(fgets(buf_ps, 1024, ptr)!=NULL)
        {
            result += buf_ps;
            if(result.length() > 1024){
                break;
            }
        }
        pclose(ptr);
        ptr = NULL;
    }
    else
    {
        printf("execute %s error\n", ps);
    }
    return result;
}

long Utils::OS::GetFileSize(const std::string& filepath) {
    FILE *file = fopen(filepath.c_str(), "rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fclose(file);
        return size;
    }
    return 0;
}

string Utils::OS::GetCPUName() {
    string cpumodel;
    ifstream ifs("/proc/cpuinfo");
    if (ifs) {
        std::string line;
        while(std::getline(ifs, line)) {
            if (String::StartsWith(line, "model name") ||
                    String::StartsWith(line, "cpu model")) {
                std::vector<std::string> flags;
                Utils::String::Split(line, ":", &flags);
                if (flags.size() == 2) {
                    cpumodel = String::TrimAll(flags[1]);
                    break;
                }
            }
        }
        ifs.close();
    }
    return cpumodel;
}

std::string Utils::OS::GetOSReleaseName() {
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

#endif


#ifdef COMMON_PATH_API
int Utils::Path::mkdirs(const std::string& s, mode_t mode) {
    size_t pre=0,pos;
    std::string dir;
    int mdret = 0;

    string target = s;
    if(target[target.size()-1]!='/'){
        // force trailing / so we can handle everything in loop
        target += "/";
    }
    if(target[0] == '/') {
        pre = 1;
    }

    while((pos=target.find_first_of('/',pre))!=std::string::npos) {
        dir=target.substr(0,pos++);
        if(dir.size()==0)
            continue;

        pre=pos;
 // if leading / first time is 0 length
        umask(0);
        if((mdret=::mkdir(dir.c_str(),mode)) && errno!=EEXIST) {
            return mdret;
        }
    }
    return mdret;
}

bool Utils::Path::FileAvailed(const std::string& filename) {
    struct stat status;
    if(stat(filename.c_str(), &status)==0 && !S_ISDIR(status.st_mode)) {
        return true;
    }
    return false;
}

bool Utils::Path::PathExists(const std::string& path) {
    struct stat status;
    if(stat(path.c_str(), &status)==0 && S_ISDIR(status.st_mode)) {
        return true;
    }
    return false;
}

std::string Utils::Path::JoinPath(const char* path, ...) {
    std::string ret = path;
    if (ret[ret.size()-1] == '/') {
        ret.erase(ret.size()-1);
    }

    va_list args;
    va_start(args, path);
    while(1) {
        char* one = va_arg(args, char*);
        if (one == NULL) {
            break;
        }

        if (strlen(one) == 0) {
            continue;
        }
        if (one[0] != '/') {
            ret += "/";
        }
        ret += one;
    }
    return ret;
}

bool Utils::Path::SplitPath(const std::string& filepath, std::string& path, std::string& file) {
    int pos = filepath.rfind('/');
    if (pos < 0) {
        return false;
    }

    path = filepath.substr(0, pos);
    file = filepath.substr(pos+1);
    return true;
}

bool Utils::Path::CheckPath(const string &path, mode_t mode) {
    if (!PathExists(path)) {
        mkdirs(path, mode);
        return chmod(path.c_str(), mode);
    }
    else {

        return chmod(path.c_str(), mode);
    }
}

bool Utils::Path::MatchPath(const char* source, const char* dest) {
    vector<string> vsrc, vdst;
    String::Split(source, "/", &vsrc);
    String::Split(dest, "/", &vdst);

    for (int i=0; i<(int)vdst.size(); ++i) {
        if (vdst[i] != vsrc[i]) {
            return false;
        }
    }
    return true;
}

int Utils::Path::GetDictDictionaryInfo(const string &path, std::vector<string> &info, bool showhidden) {
    struct dirent *tmp = NULL;
    char tempPath[1024] = {0};

    DIR *dir = opendir(path.c_str());
    if (NULL == dir) {
        return -1;
    }
    while (NULL != (tmp = readdir(dir))) {
        bool isdir = false;
#if defined (COMMON_LINUX)
        isdir = DT_DIR==tmp->d_type;
        if (!isdir && DT_UNKNOWN==tmp->d_type) {
#endif
            snprintf(tempPath, 1023, "%s/%s", path.c_str(), tmp->d_name);
            struct stat buf;
            stat(tempPath, &buf);
            isdir = bool(buf.st_mode & S_IFDIR);
#if defined (COMMON_LINUX)
        }
#endif
        if (isdir) {
            if (0 == strcmp(tmp->d_name, ".") ||
                    0 == strcmp(tmp->d_name, "..")) {
                continue;
            }

            if (tmp->d_name[0] == '.' && !showhidden) {
                continue;
            }
            info.push_back(tmp->d_name);
        }
    }
    closedir(dir);
    return 0;
}

int Utils::Path::TravelDictionary(const char *dic, int (*func)(const char* file, const char *name)) {
    struct dirent *tmp = NULL;
    char tempPath[1024] = {0};

    DIR *dir = opendir(dic);
    if (NULL == dir) {
        return -1;
    }
    while (NULL != (tmp = readdir(dir))) {
        bool isdir = false;
#if defined (COMMON_LINUX)
        isdir = DT_DIR==tmp->d_type;
        if (!isdir && DT_UNKNOWN==tmp->d_type)
#endif
        {
            snprintf(tempPath, 1023, "%s/%s", dic, tmp->d_name);
            struct stat buf;
            stat(tempPath, &buf);
            isdir = bool(buf.st_mode & S_IFDIR);
        }

        bool isfile = false;
#if defined (COMMON_LINUX)
        isfile = DT_REG==tmp->d_type;
        if (!isfile && DT_UNKNOWN==tmp->d_type)
#endif
        {
            snprintf(tempPath, 1023, "%s/%s", dic, tmp->d_name);
            struct stat buf;
            stat(tempPath, &buf);
            isfile = bool(buf.st_mode & S_IFREG);
        }

        if (isfile) {
            string file = Utils::Path::JoinPath(dic, tmp->d_name, NULL);
            if (func(file.c_str(), tmp->d_name) < 0) {
                return -1;
            }
        } else if (isdir) {
            if (0 == strcmp(tmp->d_name, ".") ||
                    0 == strcmp(tmp->d_name, "..")) {
                continue;
            }

            sprintf(tempPath, "%s/%s", dic, tmp->d_name);
            if (TravelDictionary(tempPath, func) < 0) {
                return -1;
            }
        }
    }
    closedir(dir);
    return 0;
}

bool Utils::Path::DeleteFolder(const char *dirname) {
    char chBuf[256];
    DIR * dir = NULL;
    struct dirent *ptr;

    dir = opendir(dirname);
    if(NULL == dir) {
        return false;
    }
    while((ptr = readdir(dir)) != NULL) {
        if(0 == strcmp(ptr->d_name, ".")) {
            continue;
        }
        if(0 == strcmp(ptr->d_name, "..")){
            continue;
        }
        snprintf(chBuf, 256, "%s/%s", dirname, ptr->d_name);

        struct stat buf;
        stat(chBuf, &buf);
        if(S_ISSOCK(buf.st_mode)) {
            unlink(chBuf);
        }
        else if(S_ISDIR(buf.st_mode)) {
            DeleteFolder(chBuf);
        }
        else {
            int err = unlink(chBuf);
//            if(0 != remove(chBuf)) {
//                return false;
//            }
        }
    }
    closedir(dir);
    return remove(dirname)==0;
}

bool Utils::Path::CopyFile(const std::string& source, const std::string& dest) {
    if (!FileAvailed(source)) {
        return false;
    }

    std::string path, file;
    if (!SplitPath(dest, path, file)) {
        return false;
    }

    CheckPath(path);

    FILE *in, *out;
    in = fopen(source.c_str(), "rb");
    if (NULL == in) {
        return false;
    }
    out = fopen(dest.c_str(), "wb");
    if (NULL == out) {
        fclose(in);
        return false;
    }

    char buffer[1024];
    int nRet = 0;
    bool success = true;
    while((nRet = fread(buffer, 1, 1024, in)) > 0) {
        if(fwrite(buffer, nRet, 1, out) != 1) {
            success = false;
            break;
        }
    }
    fclose(in);
    fclose(out);
    return success;
}

//std::string Utils::Path::GetInstallPath() {
//    std::string path = INSTALL_PATH;
//    if (!path.empty()) {
//        return path;
//    }

//    if (FileAvailed(RAV_CONFIG_FILE)) {
//        ConfigFile *config = ConfigFile::NEW(RAV_CONFIG_FILE);
//        if (config) {
//            path = config->mInstallPath;
//            delete config;
//        }
//    }
//    return path;
//}

std::string Utils::Path::GetPath(const std::string& path) {
    int pos = path.rfind("/");
    if (pos != -1) {
        return path.substr(0, pos);
    }
    return "";
}

std::string Utils::Path::GetFileName(const std::string& path) {
    int pos = path.rfind("/");
    if (pos != -1) {
        return path.substr(pos+1, std::string::npos);
    }
    return "";
}

std::string Utils::Path::GetDomainUrl(const std::string& url) {
    if (String::StartsWith(url, "https") || String::StartsWith(url, "http")) {
        int pos = url.find(":");
        if (pos != (int)string::npos) {
            pos = url.find("/", pos+3);
            if (pos != (int)string::npos) {
                return url.substr(0, pos);
            }
            return url;
        }
    }
    return "";
}
#endif


#ifdef COMMON_JSON_API
std::string Utils::JSON::GetJsonString(Json::Value value, const std::string& flag) {
    std::string ret = "";
    Json::Value sub = value[flag];
    if (sub.isString()) {
        return sub.asString();
    }
    else if (sub.isInt()) {
        char t[12];
        sprintf(t, "%d", sub.asInt());
        return t;
    }else if (sub.isArray()) {
        int size = sub.size();
        if (size == 0) {
            return "";
        }
        for(int i=0; i<size; i++) {
            ret += sub[(Json::Value::UInt)i].asString();
            if (i != (size -1) ) {
                ret += ",";
            }
        }
    }
    return ret;
}


int Utils::JSON::GetJsonInt(Json::Value value, const std::string& flag) {
    Json::Value sub = value[flag];
    if (sub.isString()) {
        std::string temp = sub.asString();
        int t = 0;
        if (String::StartsWith(temp, "0x")) {
            if (sscanf(temp.c_str(), "0x%x", &t) > 0) {
                return t;
            }
        }
        else {
            if (sscanf(temp.c_str(), "%d", &t) > 0) {
                return t;
            }
        }
    }
    else if (sub.isInt()) {
        return sub.asInt();
    }
    return -1;
}
#endif


#ifdef COMMON_LOG_API
//void Utils::LOG::PrintLog(const std::string& file, const std::string& log) {
//    std::string datetime = Date::FormatTimeStamp(Date::GetTimeStamp());

//    std::ofstream ofs;
//    ofs.open(file.c_str(), std::ios_base::in|std::ios_base::app);
//    ofs << "[" << datetime << "]\t" << log << std::endl;
//    ofs.close();
//}

void Utils::LOG::PrintLog(FILE *file, const char *fmt, ...) {
    if(NULL == file)
        return;

    time_t timeSecs = time(NULL);
    struct tm *timeInfo = localtime(&timeSecs);
    static char acTitle[1024] = { 0 };
    snprintf(acTitle, sizeof(acTitle), "[%d-%02d-%02d %02d:%02d:%02d]\t",
             timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday,
             timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);

    int iLen = strlen(acTitle);
    fwrite(acTitle, iLen, 1, file);

    memset(acTitle, 0, sizeof(acTitle));
    va_list args;
    va_start(args, fmt);
    vsnprintf(acTitle, sizeof(acTitle), fmt, args);
    va_end(args);
    iLen = strlen(acTitle);
    fwrite(acTitle, iLen, 1, file);
    fwrite("\n", 1, 1, file);
    fflush(file);
}

static string install_path_for_log;
void Utils::LOG::InitLog(const char *path) {
    install_path_for_log = path;
}

void Utils::LOG::PrintLog(int type, const char *fm, ...) {
    int buflen = 5120;
    char buf[buflen];
    int i = 0;
    memset(buf, 0, buflen);
    va_list args;
    va_start( args, fm );
    vsnprintf(buf, buflen, fm,args);
    va_end( args );

    FILE* logfile=NULL;

    if (install_path_for_log.size() == 0) {
        return;
    }

    std::string file;
    if (0 == type) {
        file = Utils::Path::JoinPath(install_path_for_log.c_str(), "RJJHGJCZ", "log.txt", NULL);
    }
    else if (1 == type) {
        file = Utils::Path::JoinPath(install_path_for_log.c_str(), "RJJHGLTX", "log.txt", NULL);
    }
    else if (2 == type) {
        file = Utils::Path::JoinPath(install_path_for_log.c_str(), "RJJHZDFY", "log.txt", NULL);
    }
    else if(3 == type) {
        file = Utils::Path::JoinPath(install_path_for_log.c_str(), "RJJHetc", "log.txt", NULL);
    }
    else if(4 == type) {
        file = "/var/log/rundebug/RAV/log.txt";
    }

    std::string path, name;
    Utils::Path::SplitPath(file, path, name);
    if(access(path.c_str(),0)!=0) {
        return;
    }

    char fname[256];
    char longtime[200];
    time_t t;
    memset(fname,0,sizeof(fname));
    time(&t);
    struct tm local = {0};
    localtime_r(&t, &local);
    sprintf(longtime,"%04d-%02d-%02d %02d:%02d:%02d <%d> <%u>", local.tm_year+1900,
            local.tm_mon+1, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec, getpid(), (int)(intptr_t)pthread_self());

    for(i= 0; i < 3; i++)
    {
        logfile = fopen(file.c_str(), "ar+");
        if(logfile)
        {
            int fd = fileno(logfile);
            fchmod(fd, 0666);
            break;
        }

        sleep(1);
    }
    if(logfile)
    {
        // 判断文件大小
        if (ftell(logfile) < 100*1024*1024)   // 5M
        {
            //日期时间
            const char* pTemp = longtime;
            fwrite(pTemp, 1, strlen(pTemp), logfile);
            fwrite(" ", 1, 1, logfile);
            //内容
            fwrite(buf, 1, strlen(buf), logfile);
            fwrite(" \r\n", 1, 3, logfile);
            fclose(logfile);
        }
        else
        {
            fclose(logfile);
            remove(fname);
        }
    }
}
#endif


#ifdef COMMON_ZIP_API
int Utils::Compress::ZipFile(const std::string& in, const::std::string& out) {
    CUNIXZIP zip;
    zip.gz_init(in.c_str(), out.c_str());
    return zip.gz_compress();
}

int Utils::Compress::UnzipFile(const std::string& in, const::std::string& out) {
    CUNIXZIP zip;
    zip.gz_init(in.c_str(), out.c_str(), true);
    return zip.gz_uncompress();
}

#endif

#ifdef COMMON_7Z_API
int Utils::SevenZip::Encode7z(ISeqOutStream *outStream, ISeqInStream *inStream, UInt64 fileSize) {
    CLzmaEncHandle enc;
    SRes res;
    CLzmaEncProps props;

    enc = LzmaEnc_Create(&g_Alloc);
    if (enc == 0) {
        return SZ_ERROR_MEM;
    }

    LzmaEncProps_Init(&props);
    res = LzmaEnc_SetProps(enc, &props);

    if (res == SZ_OK) {
        Byte header[LZMA_PROPS_SIZE + 8];
        size_t headerSize = LZMA_PROPS_SIZE;
        int i;

        res = LzmaEnc_WriteProperties(enc, header, &headerSize);
        for (i = 0; i < 8; i++) {
            header[headerSize++] = (Byte)(fileSize >> (8 * i));
        }

        if (outStream->Write(outStream, header, headerSize) != headerSize) {
            res = SZ_ERROR_WRITE;
        }
        else {
            if (res == SZ_OK) {
                res = LzmaEnc_Encode(enc, outStream, inStream, NULL, &g_Alloc, &g_Alloc);
            }
        }
    }

    LzmaEnc_Destroy(enc, &g_Alloc, &g_Alloc);
    return res;
}

int Decode2(CLzmaDec *state, ISeqOutStream *outStream, ISeqInStream *inStream, UInt64 unpackSize) {
    int thereIsSize = (unpackSize != (UInt64)(uint64_t)-1);
    unsigned char inBuf[IN_BUF_SIZE];
    unsigned char outBuf[OUT_BUF_SIZE];
    size_t inPos = 0, inSize = 0, outPos = 0;
    LzmaDec_Init(state);
    for (;;)
    {
        if (inPos == inSize) {
            inSize = IN_BUF_SIZE;
            RINOK(inStream->Read(inStream, inBuf, &inSize));
            inPos = 0;
        }
        {
            SRes res;
            SizeT inProcessed = inSize - inPos;
            SizeT outProcessed = OUT_BUF_SIZE - outPos;
            ELzmaFinishMode finishMode = LZMA_FINISH_ANY;
            ELzmaStatus status;
            if (thereIsSize && outProcessed > unpackSize) {
                outProcessed = (SizeT)unpackSize;
                finishMode = LZMA_FINISH_END;
            }

            res = LzmaDec_DecodeToBuf(state, outBuf + outPos, &outProcessed,
            inBuf + inPos, &inProcessed, finishMode, &status);
            inPos += inProcessed;
            outPos += outProcessed;
            unpackSize -= outProcessed;

            if (outStream) {
                if (outStream->Write(outStream, outBuf, outPos) != outPos) {
                    return SZ_ERROR_WRITE;
                }
            }

            outPos = 0;

            if (res != SZ_OK || (thereIsSize && unpackSize == 0)) {
                return res;
            }

            if (inProcessed == 0 && outProcessed == 0) {
                if (thereIsSize || status != LZMA_STATUS_FINISHED_WITH_MARK) {
                    return SZ_ERROR_DATA;
                }
                return res;
            }
        }
    }
}

int Utils::SevenZip::Decode7z(ISeqOutStream *outStream, ISeqInStream *inStream) {
    UInt64 unpackSize;
    int i;
    SRes res = 0;

    CLzmaDec state;

    /* header: 5 bytes of LZMA properties and 8 bytes of uncompressed size */
    unsigned char header[LZMA_PROPS_SIZE + 8];

    /* Read and parse header */

    RINOK(SeqInStream_Read(inStream, header, sizeof(header)));

    unpackSize = 0;
    for (i = 0; i < 8; i++)
        unpackSize += (UInt64)header[LZMA_PROPS_SIZE + i] << (i * 8);

    LzmaDec_Construct(&state);
    RINOK(LzmaDec_Allocate(&state, header, LZMA_PROPS_SIZE, &g_Alloc));
    res = Decode2(&state, outStream, inStream, unpackSize);
    LzmaDec_Free(&state, &g_Alloc);

    return res;
}

int Utils::SevenZip::lzma_compress(const std::string& in, const::std::string& out) {
    CFileSeqInStream inStream;
    CFileOutStream outStream;
    int res;

    FileSeqInStream_CreateVTable(&inStream);
    File_Construct(&inStream.file);

    FileOutStream_CreateVTable(&outStream);
    File_Construct(&outStream.file);

    if (InFile_Open(&inStream.file, in.c_str()) != 0)
        return -1;

    if (OutFile_Open(&outStream.file, out.c_str()) != 0)
        return -1;


    UInt64 fileSize;
    File_GetLength(&inStream.file, &fileSize);
    res = Encode7z(&outStream.vt, &inStream.vt, fileSize);

    File_Close(&outStream.file);
    File_Close(&inStream.file);

    return res;
}

int Utils::SevenZip::lzma_uncompress(const std::string& in, const::std::string& out) {
    CFileSeqInStream inStream;
    CFileOutStream outStream;

    int res;

    FileSeqInStream_CreateVTable(&inStream);
    File_Construct(&inStream.file);

    FileOutStream_CreateVTable(&outStream);
    File_Construct(&outStream.file);

    if (InFile_Open(&inStream.file, in.c_str()) != 0)
        return -1;

    if (OutFile_Open(&outStream.file, out.c_str()) != 0)
        return -1;


    UInt64 fileSize;
    File_GetLength(&inStream.file, &fileSize);
    res = Decode7z(&outStream.vt, &inStream.vt);

    File_Close(&outStream.file);
    File_Close(&inStream.file);

    return res;
}

#endif
