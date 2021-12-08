#ifndef CLIENTINFO_H
#define CLIENTINFO_H

using namespace std;

#include <string>

class ClientInfo
{
public:
    int GetMacAddr(string& out);
    int GetLocalIP(string &outIP);
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

private:
    int AvaildGUID(const char * guid);
    std::string GetOSReleaseName();

private:
    string mIP;
    string mMac;
    string mEthName;
    string mHostName;
    string mCPUArch;
    string mUser;
    string mRoot;
};

#endif // CLIENTINFO_H
