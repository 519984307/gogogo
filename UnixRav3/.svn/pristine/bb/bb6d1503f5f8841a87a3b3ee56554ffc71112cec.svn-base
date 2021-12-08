#include "updater.h"
#include "tailreader.h"

#include <unistd.h>
#include <getopt.h>
#include <iostream>

extern char *optarg;
extern int optind, opterr, optopt;

using namespace std;

void PomptArgs(string proc)
{
    cout << "Using:" << endl;
    cout << "\t" << proc << " -s install security machine" << endl;
    cout << "\t" << proc << " -h help information" << endl;
}

int main(int argc, char* argv[]) 
{
    int opt = 0;
    bool islpserver = false;

    while ((opt = getopt(argc, argv, "sh")) != -1) {
        switch(opt) {
        case 's':
            islpserver = true;
            break;
        case 'h':
            PomptArgs(argv[0]);
            return 0;
            break;
        }
    }
    TailReader treader;
    Updater updater;   
    if ( treader.ParseFile() == 0 ) {
        updater.SetTailInfo(treader.type_, treader.center_addr_);
    }
    updater.InitInfo(islpserver);
    if (updater.DownloadInstallFiles() != 0) {
        cout << "Download install files fail" << endl;
        return -1;
    }  
    updater.GetServerType();
    updater.BackupData();                                                                                                                  
    if (updater.InstallNewFiles() != 0) {
        cout << "Install new files fail" << endl;
        return -1;
    }
    updater.RestoreBackupedData();
    updater.StartDaemons();
    updater.BackupConfigFile();
    updater.UninstallOldFiles();
    updater.RestoreConfigFile();
    return 0;
}
