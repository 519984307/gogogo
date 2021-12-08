#include <cstdio>
#include <string>
#include <iostream>
#include <cstdarg>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <map>
#include <list>
#include <utility>

#include "md5.h"
#include "unzip.h"
#include "unixzip.h"

using namespace std;

// define the function pointer
typedef int (*FileOpreater)(const string& path, const string& file);
const char *FileType[] = {
    "NETWORK",
    "CHAR",
    "COMM",
};

const int LEN_64 = 64;
const int MIN_BUFF = 256;

#define UPLOAD_FOLDER   "upload"
#define CENTER_MD5_FILE "clientmd5.list"
#define VER_FILE_LIST   "versionnew.list"

struct _section_ver {
  char secname[64];
  char version[48];
  char date[16];
  int  count;
  int  secsize;
};
typedef struct _section_ver SectionVersion;

struct _file_ver
{
  char filename[MIN_BUFF];
  char version[LEN_64];
  char download[MIN_BUFF];
};
typedef struct _file_ver FileVersion;

// global vaient
string gPackageDir;
string gVersion;
FILE   *gCenterFile = NULL;

map<string, list<pair<string,string> > > gFileInfo;

std::string JoinPath(const char* path, ...) {
    // the function come from CommonAPI/commonapi.h
    std::string ret = path;
    if (ret[ret.size()-1] == '/') {
        ret.erase(ret.size()-1);
    }

    va_list args;
    va_start(args, path);
    while(1) {
        char* one = va_arg(args, char*);
        if (one == NULL) {
            break;
        }

        if (strlen(one) == 0) {
            continue;
        }
        if (one[0] != '/') {
            ret += "/";
        }
        ret += one;
    }
    return ret;
}

int parse_args(char argc, char *argv[]) {
    if (argc < 3) {
        cout << "command need package dir!\n" << endl;
        return -1;
    }

    int opt = 0;
    while ((opt = getopt(argc, argv, "d:v:h")) != -1) {
        switch (opt) {
        case 'd':
            gPackageDir = optarg;
            break;
        case 'v':
            gVersion = optarg;
            break;
        case 'h': {
            cout << "Format: UpgradeMaker installdir" << endl;
            break;
        }

        default:
            break;
        }
    }
    return 0;
}

int travl_package(const string& pkgdir, FileOpreater pFunc) {
    char tempPath[1024] = {0};
    DIR *dir = opendir(pkgdir.c_str());
    if (NULL == dir) {
        return 0;
    }

    int error = 0;
    struct dirent *tmp = NULL;
    while (NULL != (tmp = readdir(dir))) {
        if (DT_REG == tmp->d_type) {
            if (NULL != pFunc) {
                if ((*pFunc)(pkgdir, tmp->d_name)) {
                    error = -1;
                    break;
                }
            }
        } else if (DT_DIR == tmp->d_type) {
            if (0 == strcmp(tmp->d_name, ".") ||
                    0 == strcmp(tmp->d_name, "..")) {
                continue;
            }
            else if (pkgdir ==  "/")
                sprintf(tempPath, "/%s", tmp->d_name);
            else
                sprintf(tempPath, "%s/%s", pkgdir.c_str(), tmp->d_name);

            if (travl_package(tempPath, pFunc)) {
                error = -1;
                break;
            }
        }
    }
    closedir(dir);
    return error;
}

string general_filename(const string& filepath, const string& file, bool addtoinfo) {
    const char *format = "LINUXKERNEL.%d.INTEL.%s.DEFAULT";

    // what the fuck?
    char category[128];
    if (!file.empty()) {
        sprintf(category, format, 26, "COMM");
    }

    string zipfile;
    if (addtoinfo) {
        zipfile = file + "." + category + ".zip";
        pair<string, string> filepair = make_pair(filepath, zipfile);
        map<string, list<pair<string, string> > >::iterator iter = gFileInfo.find(category);
        if (iter == gFileInfo.end()) {
            list<pair<string, string> > categories;
            categories.push_back(filepair);
            gFileInfo.insert(pair<string, list<pair<string, string> > >(category, categories));
        }
        else {
            iter->second.push_back(filepair);
        }
    }
    else {
        zipfile = file + ".zip";
    }
    return zipfile;
}

/**
 * @brief general_file
 * @param upload_to : upload file path
 * @param path : the file real path
 * @param file : the file name
 * @param filename : the sub path name
 * @param addtoinfo : if it add the gFileInfo
 * @return
 */
string general_file(const string& upload_to, const string& path, const string& file, const string& filename, bool addtoinfo=true) {
    const string origin  = JoinPath(path.c_str(), file.c_str(), NULL);

    string zipfile = general_filename(filename, file, addtoinfo);
    string out = JoinPath(upload_to.c_str(), zipfile.c_str(), NULL);

//    UnZip ziper;
//    if (0 == ziper.CreateNewZip(out.c_str(), origin.c_str())) {
//        return zipfile;
//    }

    CUNIXZIP zipper;
    if (zipper.gz_init(origin.c_str(), out.c_str(), false, 0)) {
        return "";
    }

    if (zipper.gz_compress()) {
        return "";
    }

    return zipfile;
}

string general_md5(const string& zipfile) {
    // the function come from CommonAPI/commonapi.h
    unsigned char data[1024];
    FILE *file = fopen(zipfile.c_str(), "rb");
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

int make_upload_file(const string& path, const string& file) {
    string subfolder = path.substr(gPackageDir.length()+1+gVersion.length()+1);
    string filename = JoinPath(subfolder.c_str(), file.c_str(), NULL);
    string uploadfolder = JoinPath(gPackageDir.c_str(), UPLOAD_FOLDER, NULL);

    string zipfile = general_file(uploadfolder, path, file, filename);
    if (zipfile.empty()) {
        return -1;
    }

    string hash = general_md5(JoinPath(path.c_str(), file.c_str(), NULL));
    if (hash.empty()) {
        return -1;
    }
    string content = filename + "=" + hash;

    if (gCenterFile) {
        fputs(content.c_str(), gCenterFile);
        fputs("\n", gCenterFile);
        fflush(gCenterFile);
    }

    return 0;
}

int make_versionlist(const string& file) {
    FILE *fd = fopen(file.c_str(), "wb+");
    if (NULL == fd) {
        return -1;
    }

    time_t curtime=time(0);
    struct tm *sttm=localtime(&curtime);
    char date[16];
    sprintf(date, "%4d-%02d-%02d",1900+sttm->tm_year,sttm->tm_mon+1, sttm->tm_mday);

    map<string, list<pair<string,string> > >::iterator iter;
    for (iter=gFileInfo.begin(); iter!=gFileInfo.end(); ++iter) {
        SectionVersion section;
        memset(&section, 0, sizeof(SectionVersion));
        strcpy(section.secname, iter->first.c_str());
        strcpy(section.version, gVersion.c_str());
        strcpy(section.date, date);
        section.count = iter->second.size();
        section.secsize = section.count * sizeof(FileVersion);

        fwrite(&section, sizeof(SectionVersion), 1, fd);

        list<pair<string,string> >::iterator it;
        for (it=iter->second.begin(); it!=iter->second.end(); ++it) {
            FileVersion filev;
            memset(&filev, 0, sizeof(FileVersion));
            strcpy(filev.filename, it->first.c_str());
            strcpy(filev.version, gVersion.c_str());
            strcpy(filev.download, it->second.c_str());

            fwrite(&filev, sizeof(FileVersion), 1, fd);
        }
    }
    fclose(fd);
}

#if 1
int main(int argc, char *argv[]) {
    if (parse_args(argc, argv)) {
        return -1;
    }

    gFileInfo.clear();
    string uploadfolder = JoinPath(gPackageDir.c_str(), UPLOAD_FOLDER, NULL);
    mkdir(uploadfolder.c_str(), 0755);

    string centermd5 = JoinPath(uploadfolder.c_str(), CENTER_MD5_FILE, NULL);
    gCenterFile = fopen(centermd5.c_str(), "w+");
    if (NULL == gCenterFile) {
        cout << "create the center.md5 file error!\n" << endl;
    }
    string sourcedir = JoinPath(gPackageDir.c_str(), gVersion.c_str(), NULL);
    if (travl_package(sourcedir, make_upload_file)) {
        cout << "travl dir error!\n" << endl;
    }

    if (gCenterFile) {
        fclose(gCenterFile);
    }

    string centerzip = general_file(uploadfolder, uploadfolder, CENTER_MD5_FILE, CENTER_MD5_FILE, false);
    if (!centerzip.empty()) {
        unlink(JoinPath(uploadfolder.c_str(), CENTER_MD5_FILE, NULL).c_str());
    }

    string versionlist = JoinPath(uploadfolder.c_str(), VER_FILE_LIST, NULL);
    make_versionlist(versionlist);

    cout << "make upload success\n" << endl;
    return 0;
}
#endif
