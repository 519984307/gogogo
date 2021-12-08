#ifndef FINISHAPP_H
#define FINISHAPP_H

#include <pthread.h>
#include <string>
#include <sys/types.h>

class AppConfig;
class FinishApplication
{
public:
    FinishApplication(AppConfig *config, const std::string& ownerpath, const std::string& version, uid_t uid);
    virtual ~FinishApplication();

    int Start();

private:
    typedef int (FinishApplication::*UpdateFunc)(const std::string&, const std::string&);

private:
    bool CheckUpdateValid(const std::string& updatedir);
    bool BackupFile(const std::string& file);
    bool ReplaceFile(const std::string& source, const std::string& dest);

    bool FileCanReplaced(const std::string& file);

    int TravalDir(const std::string& folder, UpdateFunc func);

    int UpdateFile(const std::string& dir, const std::string& file);
    int RestoreFile(const std::string& dir, const std::string& file);

    int KillProc(const char *app, int signal);

    int KillAllProc();
    int StartServer();

protected:
    virtual void OnUpdateSuccess() = 0;
    virtual void OnUpdateFailed() = 0;

private:
    static void* ThreadWork(void *args);
    int WorkFunc();

    pthread_t   mWorkThread;

protected:
    AppConfig   *mAppConfig;
    uid_t       mUid;

    std::string mOwnerPath;
    std::string mVersion;
};

#endif // FINISHAPP_H
