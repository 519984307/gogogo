/**
  * 
  * */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <iostream>
#include <cstdarg>
#include <fstream>

#include "commonapi.h"
#include "crypt/md5.h"

using namespace std;

string autoupdate = "";
string replacefile = "";

int parse_args(char argc, char *argv[]) {
    if (argc < 3) {
        cout << "command need package dir!\n" << endl;
        return -1;
    }

    int opt = 0;
    while ((opt = getopt(argc, argv, "d:a:h")) != -1) {
        switch (opt) {
        case 'd':
            replacefile = optarg;
            break;
        case 'a':
            autoupdate = optarg;
            break;
        default:
            break;
        }
    }
    return 0;
}

typedef struct {
    char magic[4];
    int  count;
    int  offset;
    int  dataoff;
}HEADER;

typedef struct  {
    unsigned int zipsize;
    unsigned int filesize;
    char         filename[128];
    char         filepath[128];
    char         hash[32];
}EXTRACT_FILE_INFO;

std::string GetFileMD5(const std::string& path) {
    unsigned char data[1024];
    FILE *file = fopen(path.c_str(), "rb");
    if (NULL == file) {
        return "";
    }

    MD5 md5;

    while (1) {
        int readed = fread(data, 1, 1024, file);
        if (readed <= 0) {
            fclose(file);
            break;
        }

        md5.update(data, readed);
    }

    md5.finalize();
    return md5.hexdigest();
}

int main(int argc, char *argv[]) {
    // example
    // autorepair.bin  /tmp/dest.bin  /tmp/autoupdate.bin /home/build bin/RAVGLTX0
//    parse_args(argc, argv);
    if (argc < 3) {
        cout << "params error" << endl;
    }

    string dest = argv[1];
    string source = argv[2];
    string root = argv[3];
    vector<string> files;
    for (int i=4; i< argc; i++) {
        files.push_back(argv[i]);
    }

    vector<EXTRACT_FILE_INFO> fileInfos;

    Utils::Path::CopyFile(source, dest);
    FILE *f = fopen(dest.c_str(), "ab+");
    if (f) {
        fseek(f, 0, SEEK_END);
        long start = ftell(f);
        long zipfile = 0;

        vector<string>::iterator fileptr;
        for (fileptr=files.begin();fileptr!=files.end();++fileptr) {
            string zip = Utils::Path::JoinPath(root.c_str(), fileptr->c_str(), NULL);
            if (!Utils::Path::FileAvailed(zip)) {
                cout << zip << " not found file" << endl;
                return -1;
            }

            long origin_size = Utils::OS::GetFileSize(zip);

            string hash = GetFileMD5(zip);

            if (0 == Utils::Compress::ZipFile(zip, "/tmp/ravgltx0")) {
                FILE *f0 = fopen("/tmp/ravgltx0", "rb");
                if (f0) {
                    fseek(f0, 0, SEEK_END);
                    zipfile = ftell(f0);

                    fseek(f0, 0, SEEK_SET);

                    char buffer[1024];
                    do {
                        int readed = fread(buffer, 1, 1024, f0);
                        if (readed > 0) {
                            fwrite(buffer, 1, readed, f);
                        }
                        else {
                            break;
                        }
                    }
                    while(1);
                    fclose(f0);
                }
                unlink("/tmp/ravgltx0");

                EXTRACT_FILE_INFO info;
                memset(&info, 0, sizeof(EXTRACT_FILE_INFO));

                info.zipsize = zipfile;
                info.filesize = origin_size;

                string name, path;
                Utils::Path::SplitPath(*fileptr, path, name);

                strcpy(info.filename, name.c_str());
                strcpy(info.filepath, fileptr->c_str());

                memcpy(info.hash, hash.c_str(), 32);

                fileInfos.push_back(info);
            }
        }

        int headoff = ftell(f);

        vector<EXTRACT_FILE_INFO>::iterator iter;
        for (iter=fileInfos.begin(); iter!=fileInfos.end(); ++iter) {
            fwrite(&(*iter), 1, sizeof(EXTRACT_FILE_INFO), f);
        }

        HEADER header;
        memset(&header, 0, sizeof(HEADER));

        memcpy(header.magic, "RING", 4);
        header.count = fileInfos.size();
        header.dataoff = start;
        header.offset = headoff;

        fwrite(&header, 1, sizeof(HEADER), f);
        fclose(f);

        chmod(dest.c_str(), 0755);
    }
    return 0;
}
