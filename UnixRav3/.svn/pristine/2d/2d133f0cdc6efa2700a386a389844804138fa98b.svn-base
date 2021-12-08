#ifndef COMPILE_TOOL_H
#define COMPILE_TOOL_H

#include "centerlistener.h"

using namespace std;

class MainEngine : public CenterCommandObserver {
public:
    static MainEngine* NEW();
    int Construct();

    MainEngine() {
        mListener = NULL;
    }
    ~MainEngine() {
        if (NULL == mListener) {
            delete mListener;
            mListener = NULL;
        }
    }

    int Start();

    typedef enum _command {
        I_SEE_YOU = 0,
        COMPILE_UNIXRAV,
        COMPILE_OLD_RAV,
    }CommandType;

protected:
    // from CenterCommandObserver
    int OnCommandObserver(int event, void *args, int cfd);

public:
    int parse_command(const string& command, string& svn, string& source, string& build, vector<pair<string,string> >&files);
    int update_svn(const string& path, const string& svn, bool checksvn);
    int reset_env(const string& build);
    int compile(const string& source,  const string& build);
    int check_relese(const string& build, vector<pair<string,string> >& files, vector<pair<string,string> >& outs);
    int make_result(vector<pair<string,string> >&files, string& result);
    int send_back(const string& result, vector<pair<string,string> >&files);

    std::string GetCmdResult(const char *cmd);
    int GetResultInfo(const string& info, vector<int>& result);

    //
    struct OldUnixRavParams {
        string srcdir;
        string buildir;
        string malware;
        string vlstd;
        int    malsize;
        int    cfgsize;
        string version;
        string pid;
    };

    int process_old_unixrav(const CommandBundle& bundle, OldUnixRavParams* params);
    bool receive_file(int sock, int size, const string& hash, const string& dest);
    int send_back_unixrav(int cfd, const string& filename);
    int compile_unixrav(const string& source, const string& build, const string& version, const string& pid);


public:
    CenterListener *mListener;
};

#endif // COMPILE_TOOL_H
