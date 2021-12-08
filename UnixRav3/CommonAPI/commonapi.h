#ifndef COMMONAPI_H
#define COMMONAPI_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>

#ifdef COMMON_JSON_API
    #include "json/json.h"
#endif

#ifdef COMMON_7Z_API
    #include "lzma/7zTypes.h"
#endif

class CommandBundle;

namespace Utils {
#ifdef COMMON_STRING_API
    namespace String {
        std::string TrimLeft(std::string &s);
        std::string TrimRight(std::string &s);
        std::string TrimAll(std::string &s);

        void Split(const std::string& s, const std::string& delim, std::vector< std::string >* ret);
        bool StartsWith(const std::string& source, const std::string& fix);
        bool EndsWith(const std::string& source, const std::string& fix);
        void itoa(int i, std::string& a);
        const std::string itoa(int i);
        int trimblank(char *szStr,int nMode);
        std::string ToLower(std::string str);
        std::string ToUpper(std::string str);

        std::string RandomString(int len=32);

        int CompareVersion(const std::string& source, const std::string& dest);
    }
#endif

#ifdef COMMON_HASH_API
    namespace Hash {
        std::string GetMD5(const std::string& s);
        std::string GetFileMD5(const std::string& path);

        std::string UrlEncode(const std::string& str);
        std::string UrlDecode(const std::string& str);

        std::string ESMEncrypt(const std::string& s);
        std::string ESMDecrypt(const std::string& s);

        std::string Base64Encode(const std::string& s);
        std::string Base64Decode(const std::string& s);

        std::string RC4Encrypt(const std::string& data);
        std::string RC4Decrypt(const std::string& data);

        unsigned int FileCRC32(const std::string& file);
        unsigned int StrCRC32(const std::string& data);

        std::string RSEntrypt(const std::string& data);
        std::string RSDecrypt(const std::string& data);
    }
#endif

#ifdef COMMON_OS_API
    namespace OS {
        #define START_PROC(x, y, ...)   do { \
                                            pid_t pid; \
                                            if ((pid=fork()) == 0) {\
                                                execl(x, y, __VA_ARGS__); \
                                            }\
                                        }while(0);

        int Read(int fd, int len, unsigned char* data);
        int Write(int fd, int len, unsigned char *data);
        int SendLocal(const char *domain, const unsigned char *data, int len, int *out);
        int SendLocal(const short port, const unsigned char *data, int len);

        /**
         * @brief CheckProcessState
         *  检测进程是否启动，利用了ps方法，检测是否有进程运行
         *  在专用机中由于屏蔽了ps命令，所以此函数不应在专用机中使用
         * @param process
         * @return
         */
        int CheckProcessState(const char *process);
        bool GetProcState(const char *lockfile);
        /** 获取进程的状态，以及是否检查其是自己的产生的，因为进程被拉起的时候，其所有者会其父进程 */
        bool GetProcState(const std::string& folder, bool check_owner);
        std::string GetFileOwner(const std::string& filepath);
        FILE* LockProcState(const char *folder);
        void ReleaseProcState(FILE *locker);

        int CheckModState(const char *modname);
        int KillProc(const char *process, int signal);
        int KillProc(pid_t pid, int signal);
//        int GetPid(const char *process_path, char *pid);
        int GetPid(const char *process_path, std::vector<int>& pids);
        int GetPid(const char *process_path, std::vector<std::pair<int, std::string> >& pids);
        uid_t GetProcessUID(pid_t pid);
        std::string GetProcessName();
        int GetUsers(std::vector<std::string> users);
        int GetKernelVersions(std::pair<std::string, std::string>& version);
        std::string GetKernelVersionString();
        int CheckModuleState(const std::string& module); 
        int CopyFile(const char *source, const char *dest);
        std::string GetUserFromUID(uid_t uid);
        /** 判断一个库文件是32还是64位的 */
        int GetFileBits(const char *lib);
        int GetCPUKernelCount();
        std::string GetMachineArch();

        bool GetLinkEntry(const std::string& link, std::string& entry);
        int GetLibraryFolder(std::vector<std::string>& folders);
        std::string GetSoPath(const std::string& lib, int bits, std::vector<std::string>& ldconfig);
        std::string GetCmdResult(const char *cmd);
        long GetFileSize(const std::string& filepath);
        std::string GetCPUName();
        std::string GetOSReleaseName();
    }

    namespace Date {
        long GetTimeStamp();
        std::string FormatTimeStamp(time_t timeStamp);
        bool NowInTimeArea(const std::string& start, const std::string& end);
    }
#endif

#ifdef COMMON_PATH_API
    namespace Path {
        std::string GetPath(const std::string& path);
        std::string GetFileName(const std::string& path);
        std::string JoinPath(const char*, ...);
        int mkdirs(const std::string& s, mode_t mode=0755);
        bool FileAvailed(const std::string& filename);
        bool PathExists(const std::string& path);
        bool CopyFile(const std::string& source, const std::string& dest);
        bool SplitPath(const std::string& filepath, std::string& path, std::string& file);
        bool DeleteFolder(const char *dirname);
        bool CheckPath(const std::string& path, mode_t mode=0755);
        int TravelDictionary(const char *dic, int (*func)(const char* file, const char *name));
        bool MatchPath(const char *source, const char* dest);
        std::string GetDomainUrl(const std::string& url);
        int GetDictDictionaryInfo(const std::string& path, std::vector<std::string>& info, bool showhidden=false);        
    }
#endif

#ifdef COMMON_JSON_API
    namespace  JSON {
        std::string GetJsonString(Json::Value value, const std::string& flag);
        int GetJsonInt(Json::Value value, const std::string& flag);
    }
#endif

#ifdef COMMON_LOG_API
    namespace LOG {
//        void PrintLog(const std::string& file, const std::string& log);
        void PrintLog(FILE *file, const char *fmt, ...);
        void InitLog(const char *path);
        void PrintLog(int type, const char *fm, ...);
    }
#endif

#ifdef COMMON_ZIP_API
    namespace Compress {
        int ZipFile(const std::string& in, const::std::string& out);
        int UnzipFile(const std::string& in, const::std::string& out);
    }
#endif

#ifdef COMMON_7Z_API
    namespace SevenZip {
        int Encode7z(ISeqOutStream *outStream, ISeqInStream *inStream, UInt64 fileSize);
        int Decode7z(ISeqOutStream *outStream, ISeqInStream *inStream);
        int lzma_compress(const std::string& in, const::std::string& out);
        int lzma_uncompress(const std::string& in, const::std::string& out);
    }
#endif
}



#endif // COMMONAPI_H
