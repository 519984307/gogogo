#include <iostream>
#include <string>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

#include "ravquafile.h"

using std::string;
using std::cout;
using std::endl;

string g_quafolder;

void help_info() {
    cout << "Format: restore -[d|h]" << endl;
    cout << "d: set the quatine dest path" << endl;
    cout << "h: help info" << endl;
 }

int parse_args(char argc, char *argv[]) {
    int opt = 0;
    while ((opt = getopt(argc, argv, "d:h")) != -1) {
        switch (opt) {
        case 'd':
            g_quafolder = optarg;
            return 0;
            break;
        default:
            help_info();
            break;
        }
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if (parse_args(argc, argv)) {
        return -1;
    }
    if (g_quafolder.empty()) {
        cout << "qua path is empty, check it";
        return -1;
    }
    if (access(g_quafolder.c_str(), R_OK)) {
        cout << "qua path not exist, check it";
    }
    RavQuaFile *qua = RavQuaFile::NEW(g_quafolder);
    if (!qua) {
        return -1;
    }
    int count  = qua->GetIndexCount();
    if (count < 0) {
        cout << "no qua file";
        return -1;
    } else {
        cout << "will restore " << count << " files" << endl;
    }
    
    long fileNum = 0;
    char tempPath[1024] = {0};
    DIR *dir = opendir(g_quafolder.c_str());
    if (NULL == dir) {
        cout << "error open dir";
        return -1;
    }

    struct dirent *tmp = NULL;
    while (NULL != (tmp = readdir(dir))) {
        if (0 == strcmp(tmp->d_name, ".") ||
                 0 == strcmp(tmp->d_name, "..")) {
            continue;
        }
        bool isfile = DT_REG==tmp->d_type;
        if (!isfile) 
            continue;
           
        sprintf(tempPath, "%s/%s", g_quafolder.c_str(), tmp->d_name);
        if (access(tempPath, R_OK)) {
            continue;
        }

        cout << "file " << tmp->d_name << endl;
        qua->RestoreFile(RAVQUA_ACTION_RESTOREDEL, tmp->d_name);
    }
    closedir(dir);
    delete qua;
    cout << "restore finished" << endl;
    return 0;
} 

