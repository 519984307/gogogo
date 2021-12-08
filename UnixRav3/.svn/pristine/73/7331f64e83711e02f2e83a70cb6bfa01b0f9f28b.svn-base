#include "tailreader.h"
#include "commonapi.h"

#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <fstream>

using namespace Utils::JSON;


TailReader::TailReader()
{
    have_tail_ = false;
    json_length_ = 0;

    type_ = -1;
    center_addr_ = "";
}

TailReader::~TailReader()
{

}

int TailReader::ParseFile()
{
    char stmp[120];
    int nret = -1;
    string file_name = GetOwnerPathName();
    ifstream fp(file_name.c_str());
    fp.seekg(0, ios::end);
    long size = fp.tellg();
    fp.seekg(size-4, ios::beg);
    fp.read(stmp, 4);
    if (stmp[0] == 'R' && stmp[1] == 'I' && stmp[2] == 'N' && stmp[3] == 'G') {
        int len = 0;
        fp.seekg(size-8, ios::beg);
        fp.read(stmp, 4);
        memcpy(&len, stmp, 4);
        len = ntohl(len);
        printf("file size is:%ld  len:%d\n", size, len);
        if (len < size) {
            fp.seekg(size-8-len, ios::beg);
            fp.read(stmp, len);
            stmp[len] = 0;
            printf("Json:%s\n", stmp);    
            if (ParseJson(stmp) == 0) {
                have_tail_ = true;
                nret = 0;
            }
        }
    }else {
        printf("File have no valid tail, size:%ld\n", size);
    }
    fp.close();
    return nret;
}

int TailReader::ParseJson(const string buffer)
{
    Json::Value json;
    Json::Reader reader;
    if (reader.parse(buffer, json)) {
        type_ = GetJsonInt(json, "type");
        center_addr_ = GetJsonString(json, "center");
        printf("Find tail info: type=%d center=%s\n", type_, center_addr_.c_str());
    }else{
        printf("Find tail info fail\n");
        return -1;
    }
    return 0;
}

string TailReader::GetOwnerPathName()
{
    char path[1024];
    int cnt = readlink("/proc/self/exe", path, 1024);
    if(cnt < 0|| cnt >= 1024)
    {
        return NULL;
    }
    path[cnt] = 0;
    string s_path(path);
    //printf("TailReader::GetOwnerPathName: cnt=%d len=%d\n", cnt, s_path.length());
    return s_path;
}


