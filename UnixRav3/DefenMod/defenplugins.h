#ifndef DEFENPLUGINS_H
#define DEFENPLUGINS_H

#include <string>
#include <list>
#include <sys/types.h>

using namespace std;

class DefenDataContainer;;
class DefenPlugins
{
public:
    typedef enum {
        DEFEN_IGNORE    = 0,
        DEFEN_FILE_SCAN,
        DEFEN_PROC_EXTENS,
        DEFEN_PROC_DENY,
        
    }JudgeResult;
public:
    static DefenPlugins* NEW(DefenDataContainer *setting);
    ~DefenPlugins();

private:
    DefenPlugins();
    int Construct(DefenDataContainer *setting);

    inline bool Trusted(int type, const string& file);
    inline bool DocOpener(const string& name);

public:
    /** type define in defenmmodproxy.h DEFEN_TYPE */
    JudgeResult Judge(int type, const string& filepath, void *data, string& out);

    int LoadConfig(uid_t current=0);

private:
    int GetCommonFolder(uid_t uid, list<string>& path);

private:
    DefenDataContainer *mSettings;
    string mUserName;

    string mHomeConfigPathRegex;
};

#endif // DEFENPLUGINS_H
