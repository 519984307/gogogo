#ifndef SCAN_CACHE_
#define SCAN_CACHE_

#include <string>
#include <map>
#include <vector>
#include <stdlib.h>
#include <string.h>

using namespace std;

class MutexLocker;
class ScanCache {
public:
    static ScanCache* NEW(const string& cacheFolder);
    ~ScanCache();

    /**
     *  扫描文件
     *  true: 可疑文件，需进一步确定， false已缓冲文件，白的
     */
    bool ScanFile(const string& file);
    void CacheFile(const string& file);
    void Finished();

private:
    ScanCache();
    int Construct(const string& cacheFolder);

    int LoadCache(const string& file);
    int StoreCache(const string& file);

    inline time_t GetFileChangedTime(const string& file);

    string GetCacheFile(const string& file);

    int GetFileHash(const char* file, unsigned int hash[4]);

private:
    MutexLocker *mCacheMutex;
    string       mCacheFolder;

    struct _block {
        unsigned int hash[4];
        bool operator==(const struct _block& a) const {
            return hash[0] == a.hash[0]
                    && hash[1] == a.hash[1]
                    && hash[2] == a.hash[2]
                    && hash[3] == a.hash[3];
        }
        bool operator<(const struct _block& a) const {
            if (hash[0] < a.hash[1]) {
                return true;
            }
            else if (hash[1] < a.hash[1]) {
                return true;
            }
            else if (hash[2] < a.hash[2]) {
                return true;
            }
            else if (hash[3] < a.hash[3]) {
                return true;
            }
            return false;
        }
        void reset() {
            memset(hash, 0, sizeof(unsigned int)*4);
        }
    };
    typedef struct _block block;
    map<unsigned int, vector<pair<block, unsigned int> > >    mHashPool;

    /** 存放未进入HASH 的文件CACHE */
    map<string, pair<block, unsigned int> >   mCachePool;

    block mCacheBlock;

private:
    enum {
        UNLOAD,
        LOADING,
        LOADED,
        SWAP,
        UNLOADING,
    }mCacheStatus;;
    pthread_t   mCacheThread;

    string      mCurrentCacheFile;
    time_t      mNowStamp;
    static void* CacheWork(void *params);
    void WorkFunc();
};

#endif //SCAN_CACHE_
