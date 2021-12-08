#ifndef UPGRADE_MAKER_
#define UPGRADE_MAKER_

#include <string>
#include <map>

using namespace std;

class upgrademaker {
public:
    upgrademaker(const string& id, const string& type, const string& version, const string& folder, const string& out);
    int make();

private:
    int traval_dir(const char *path);
    int make_upgrade(const string& file);
    string get_file_md5(const string& file);
    int general_xml(const string& file);

    inline long get_file_size(const string& file);

private:
    string mSource;
    string mDest;

    const string mVersion;
    const string mType;
    const string mID;

    string m7zPath;
    string mMd5Path;
    string mRpkPath;
    string mXmlPath;
    string mSrcPath;

private:
    struct file_entry {
        string path;
        string md5;
        string crc32;
        string size;
        string    norestart;
    };
    map<string, file_entry> mHashPool;
    map<string, file_entry> mKernelPool;
};
#endif //UPGRADE_MAKER_
