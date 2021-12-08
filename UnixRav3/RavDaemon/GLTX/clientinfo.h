#ifndef CLIENTINFO_H
#define CLIENTINFO_H

using namespace std;

#include <string>
#include <map>

class ClientInfo
{
public:
    int GetMacAddr(string& out, const string& ip);
    int GetLocalIP(string &outIP, const string& centerip);
    int GetDiskSize(string& outSize);
    int GetMemorySize(string& outSize);
    int GetOSName(string& outName);
    int GetClientName(string& outName);
    int GetClientArch(string& outArch);
    int GetCurrentUser(string &outUser);
    int GetUserRoot(string &outRoot);
    int GetBiosUUID(string &bios);
    int GetDiskUUID(string &diskuuid);
    int GetSystem(string& system);

    string GetDescription();
    void   SetLocalIP(const char * szIp);
    bool   CheckLocalMac(const char * szMac);

private:
    int AvaildGUID(const char * guid);
    std::string GetOSReleaseName();

    int InitEthInfo();
    string GetMac(const string& eth);

private:
    string mHostName;
    string mCPUArch;
    string mUser;
    string mRoot;

public:
    static map<string, string> mIPMacs;
    static string IP;
    static string MAC;
};


#endif // CLIENTINFO_H
