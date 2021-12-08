#include "mainengine.h"
#include "centerlistener.h"
#include "json/json.h"
#include "commonapi.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#define PORT    8090

MainEngine* MainEngine::NEW() {
    MainEngine *self = new MainEngine;
    if (self) {
        if (self->Construct()) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

int MainEngine::Construct() {
    mListener = CenterListener::NEW(this, 0, PORT);
    if (NULL == mListener) {
        return -1;
    }
    return 0;
}

int MainEngine::Start() {
    return mListener->Start();
}

int MainEngine::OnCommandObserver(int event, void *args, int cfd) {
    CommandBundle *bundle = (CommandBundle*)args;
    printf("CommandBundle:%d\n", bundle->mCommandID);
    switch (bundle->mCommandID) {
    case I_SEE_YOU: {
        if (bundle->mStringValue == "hello") {
            int value = 1;
            Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&value);
            value = 5;
            Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&value);
            Utils::OS::Write(cfd, 5, (unsigned char*)"world");
        }
        break;
    }
    case COMPILE_UNIXRAV: {
        string svn, source, build;
        vector<pair<string,string> > files;
        parse_command(bundle->mStringValue, svn, source, build, files);
        int err = update_svn(source, svn, true);
        if (0 == err) {
            reset_env(build);
            compile(source, build);
            vector<pair<string,string> > outs;
            check_relese(build, files, outs);

            string result;
            if (outs.size() > 0) {
                make_result(outs, result);
            }

            int value = 1;
            Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&value);

            value = outs.size();
            Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&value);

            vector<pair<string,string> >::iterator iter;
            for (iter=outs.begin();iter!=outs.end();++iter) {
                string path, file;
                Utils::Path::SplitPath(iter->first, path, file);
                int filesize = Utils::OS::GetFileSize(iter->first);
                Json::Value json;
                json["file"] = Json::Value(file);
                json["hash"] = Json::Value(iter->second);
                json["len"] = Json::Value(filesize);

                Json::FastWriter writer;
                string fvalue = writer.write(json);
                value = fvalue.size();
                Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&value);
                Utils::OS::Write(cfd, value, (unsigned char*)fvalue.c_str());

                FILE *f = fopen(iter->first.c_str(), "rb");
                if (f) {
                    char buffer[1024];
                    int readed = 0;
                    do {
                        int r = fread(buffer, 1, 1024, f);
                        Utils::OS::Write(cfd, r, (unsigned char*)buffer);
                        readed += r;
                    }while (readed<filesize);
                    fclose(f);
                }
            }
        }
        else {
            int value = 0;
            Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&value);

            char *message = "svn update error";
            value = strlen(message);
            Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&value);
            Utils::OS::Write(cfd, value, (unsigned char*)message);
        }

        break;
    }
    case COMPILE_OLD_RAV: {
        OldUnixRavParams params;
        process_old_unixrav(*bundle, &params);

        string dest = string("/tmp/") + Utils::String::itoa(Utils::Date::GetTimeStamp());
        if (receive_file(cfd, params.malsize, params.malware, dest)) {
            Utils::Path::CopyFile(dest, Utils::Path::JoinPath(params.buildir.c_str(), "virusdb", "malware.rmd", NULL));
        }
        unlink(dest.c_str());
        dest = string("/tmp/") + Utils::String::itoa(Utils::Date::GetTimeStamp());
        if (receive_file(cfd, params.cfgsize, params.vlstd, dest)) {
            Utils::Path::CopyFile(dest, Utils::Path::JoinPath(params.buildir.c_str(), "virusdb", "vlstd-linux.cfg", NULL));
        }
        unlink(dest.c_str());

        update_svn(params.buildir, "", false);

        dest = string("cdrom.") + params.version+"."+params.pid+".NOL.tgz";
        string destfile = Utils::Path::JoinPath(params.buildir.c_str(), dest.c_str(), NULL);

        if (Utils::Path::FileAvailed(destfile)) {
            unlink(destfile.c_str());
        }
        compile_unixrav(params.srcdir, params.buildir, params.version, params.pid);
        if (Utils::Path::FileAvailed(destfile)) {
            int result = 1;
            Utils::OS::Write(cfd, 4, (unsigned char*)&result);
            result = 2; //mean count
            Utils::OS::Write(cfd, 4, (unsigned char*)&result);

            send_back_unixrav(cfd, destfile);
            send_back_unixrav(cfd, Utils::Path::JoinPath(params.buildir.c_str(), "upload.zip", NULL));

            unlink(destfile.c_str());
            unlink(Utils::Path::JoinPath(params.buildir.c_str(), "upload.zip", NULL).c_str());
        }
        break;
    }
    }
    return 0;
}

int MainEngine::send_back_unixrav(int cfd, const string& filename) {
    string path, file;
    Utils::Path::SplitPath(filename, path, file);
    int filesize = Utils::OS::GetFileSize(filename);
    Json::Value json;
    json["file"] = Json::Value(file);
    json["hash"] = Json::Value(Utils::Hash::GetFileMD5(filename));
    json["len"] = Json::Value(int(Utils::OS::GetFileSize(filename)));

    Json::FastWriter writer;
    string fvalue = writer.write(json);
    int value = fvalue.size();
    Utils::OS::Write(cfd, sizeof(int), (unsigned char*)&value);
    Utils::OS::Write(cfd, value, (unsigned char*)fvalue.c_str());

    FILE *f = fopen(filename.c_str(), "rb");
    if (f) {
        char buffer[1024];
        int readed = 0;
        do {
            int r = fread(buffer, 1, 1024, f);
            Utils::OS::Write(cfd, r, (unsigned char*)buffer);
            readed += r;
        }while (readed<filesize);
        fclose(f);
    }
    return 0;
}

int MainEngine::process_old_unixrav(const CommandBundle& bundle, OldUnixRavParams* params) {
    Json::Value json;
    Json::Reader reader;
    if (reader.parse(bundle.mStringValue, json)) {
        params->srcdir = json["source"].asString();
        params->buildir = json["build"].asString();
        params->malware = json["malware"].asString();
        params->vlstd = json["vlstd"].asString();
        params->malsize = json["malsize"].asInt();
        params->cfgsize = json["cfgsize"].asInt();
        params->version = json["version"].asString();
        params->pid = json["pid"].asString();
        return 0;
    }
    return -1;
}

bool MainEngine::receive_file(int sock, int size, const string& hash, const string& dest) {
    FILE *file = fopen(dest.c_str(), "wb+");
    if (file) {
        unsigned char buffer[8096];
        int readlen = 0;
        do {
            int len = Utils::OS::Read(sock, (size-readlen)>8096?8096:(size-readlen), buffer);
            if (len > 0) {
                fwrite(buffer, 1, len, file);
                readlen += len;
            }
            else {
                break;
            }
        } while(readlen < size);
        fclose(file);
        printf("close the file and hash is %s\n", Utils::Hash::GetFileMD5(dest).c_str());

        return Utils::Hash::GetFileMD5(dest) == hash;
    }
//    ofstream file(dest, ios::binary|ios::out);
//    if (file) {
//        unsigned char buffer[8096];
//        int readlen = 0;
//        do {
//            int len = Utils::OS::Read(sock, 8096, buffer);
//            if (len > 0) {
//                file.write((char*)buffer, len);
//                readlen += len;
//            }
//            else {
//                break;
//            }
//            printf("receive data %d -- %d -- %d\n", readlen, size, len);
//        } while(readlen < size);
//        file.close();

//        printf("close the file and hash is %s\n", Utils::Hash::GetFileMD5(dest).c_str());

//        return Utils::Hash::GetFileMD5(dest) == hash;
//    }
    return false;
}

int MainEngine::parse_command(const string& command, string& svn, string& source, string& build, vector<pair<string,string> >&files) {
    Json::Value json;
    Json::Reader reader;
    if (reader.parse(command, json)) {
        svn = Utils::JSON::GetJsonString(json, "svn");
        Json::Value srcjson = json["source"];
        source = Utils::JSON::GetJsonString(srcjson, "source");
        build = Utils::JSON::GetJsonString(srcjson, "build");

        Json::Value fjson = json["file"];
        Json::Value::Members member = fjson.getMemberNames();
        std::vector<std::string>::iterator iter;
        for (iter=member.begin(); iter!=member.end(); ++iter) {
            Json::Value filejson = fjson[*iter];
            for (int i=0; i<filejson.size(); ++i) {
                Json::Value fijson = filejson[i];
                string path = Utils::JSON::GetJsonString(fijson, "path");
                string hash = Utils::JSON::GetJsonString(fijson, "hash");

                files.push_back(make_pair(path, hash));
            }
        }
        return 0;
    }
    return -1;
}

std::string MainEngine::GetCmdResult(const char *cmd)
{
    string result;
    char buf_ps[1024];
    char ps[1024]={0};
    FILE *ptr;
    strcpy(ps, cmd);
    if((ptr=popen(ps, "r"))!=NULL)
    {
        while(fgets(buf_ps, 1024, ptr)!=NULL)
        {
            result = buf_ps;
            if(result.length() > 1024){
                break;
            }
        }
        pclose(ptr);
        ptr = NULL;
    }
    else
    {
        printf("execute %s error\n", ps);
    }
    return result;
}

int MainEngine::GetResultInfo(const string& s, vector<int>& result) {
    int i = 0, value=0;
    int found = 0;
    for (i = 0; i <= s.size() - 1;i++) {
        if (s[i] >= '0'&&s[i] <= '9') {
            found = 1;
            value = value * 10 + s[i] - 48;
        }
        else {
            if (found) {
                result.push_back(value);
            }
            found = 0;
        }
    }
    return 0;
}


int MainEngine::compile_unixrav(const string& source, const string& build, const string& version, const string& pid) {
    string cmd = "cd " + source + " && ./buildrav.sh " + version + " " + pid + " " + " NOL " + build;

    std::cout << cmd << std::endl;

    string result = GetCmdResult(cmd.c_str());

    string dest = build + "/" + string("cdrom.") + version+"."+pid+".NOL.tgz";
    if (Utils::Path::FileAvailed(dest)) {
        cmd = string("cd ") + build + "/upload && 7za a -tzip ../upload.zip ./*";
        GetCmdResult(cmd.c_str());
        return 0;
    }
    return -1;
}


int MainEngine::update_svn(const string& path, const string& svn, bool checksvn) {
    string cmd = "cd " + path + " && svn up";
    string result = GetCmdResult(cmd.c_str());
    vector<int> versions;
    GetResultInfo(result, versions);
    if (versions.size() == 1 && checksvn) {
        if (versions[0] == atoi(svn.c_str())) {
            return 0;
        }
    }
    return -1;
}

int MainEngine::reset_env(const string& build) {
    return 0;
}
int MainEngine::compile(const string& source, const string& build) {
    string cmd = "cd " + build + " && cmake -DCMAKE_BUILD_TYPE=Release " + source + " && make";
    string result = GetCmdResult(cmd.c_str());
    vector<int> versions;
    GetResultInfo(result, versions);
    if (versions.size() == 1) {
        if (versions[0] == 100) {
            return 0;
        }
    }
    return -1;
}

int MainEngine::check_relese(const string& build, vector<pair<string,string> >& files, vector<pair<string,string> >& outs) {
    vector<pair<string,string> >::iterator iter;
    for (iter=files.begin(); iter!=files.end(); ++iter) {
        string f = Utils::Path::JoinPath(build.c_str(), iter->first.c_str(), NULL);
        string hash = Utils::Hash::GetFileMD5(f);
        if (hash != iter->second) {
            outs.push_back(make_pair(f, hash));
        }
    }
    return 0;
}

int MainEngine::make_result(vector<pair<string,string> >&files, string& result) {
    return 0;
}
int MainEngine::send_back(const string& result, vector<pair<string,string> >&files) {
    return 0;
}

