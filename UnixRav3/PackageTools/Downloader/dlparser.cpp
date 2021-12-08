#include "dlparser.h"
#include "commonapi.h"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <sys/utsname.h>
#include <string>

using namespace std;
using namespace Utils;

DlParser::DlParser()
{
}

DlParser::~DlParser()
{
    UnInit();
}

void DlParser::UnInit() 
{
    vector<RpkFile*>::iterator it = mDlFiles.begin();
    while (it != mDlFiles.end()) {
        RpkFile * rf = *it;
        delete rf;
        rf = NULL;
        it++;
    }
    mDlFiles.clear();
}

int DlParser::GetParamsFromJson(const char *file)
{
    string sdata;
    int readed = 0;
    FILE *fp = fopen(file, "r");
    if(fp == NULL) {
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    char *buf = new char[size*2];
    fseek(fp, 0, SEEK_SET);
    while (fgets(buf, 1023, fp)) {
        sdata += buf;
        buf[0] = 0;
    }
    fclose(fp);

    string sorgdata = Hash::Base64Decode(sdata);
    //printf("xxx GetParamsFromJson json=%s\n", sorgdata.c_str());
    Json::Value json;
    Json::Reader reader;
    if(reader.parse(sorgdata, json)){
        mDlUrl = JSON::GetJsonString(json, "dl_url");
        mDlRpk = JSON::GetJsonString(json, "dl_rpk");
        mInstallPath = JSON::GetJsonString(json, "install_path");
        mInstallFinish = JSON::GetJsonString(json, "install_finish");
    }
    //printf("dl_url:%s  dl_rpk:%s  install_path:%s  install_finish:%s\n", mDlUrl.c_str(), mDlRpk.c_str(), mInstallPath.c_str(), mInstallFinish.c_str());
    delete[] buf;

    return 0;
}

string DlParser::GetVersionFromXml(string &data)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    string ret = "";
    doc = xmlParseMemory(data.c_str(), data.length());
    if (doc == NULL)
        return ret;
    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        xmlFreeDoc(doc);
        return ret;
    }
    if (xmlStrcmp(cur->name, BAD_CAST"RISING")) {
        xmlFreeDoc(doc);
        return ret;
    }
    xmlNodePtr child = cur->xmlChildrenNode;
    while (child != NULL) {
        if (!xmlStrcmp(child->name, BAD_CAST"PRODUCT")) {
            xmlChar * version = xmlGetProp(child, BAD_CAST"VERSION");
            ret = reinterpret_cast<char*>(version);
            break;
        }
        child = child->next;
    }
    xmlFreeDoc(doc);
    return ret;
}

string DlParser::GetVDBVersionFromXml(string &data)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    string ret = "";
    doc = xmlParseMemory(data.c_str(), data.length());
    if (doc == NULL)
        return ret;
    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        xmlFreeDoc(doc);
        return ret;
    }
    if (xmlStrcmp(cur->name, BAD_CAST"rpklist")) {
        xmlFreeDoc(doc);
        return ret;
    }
    xmlChar * vversion = xmlGetProp(cur, BAD_CAST"virver");
    ret = reinterpret_cast<char*>(vversion);
    xmlFreeDoc(doc);
    return ret;
}

int DlParser::GetRpkFilesInfoFromXml(string &data, bool using_json_file) {
    xmlDocPtr doc;
    xmlNodePtr cur;
    doc = xmlParseMemory(data.c_str(), data.length());
    if (doc == NULL){
        return -1;
    }
    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        xmlFreeDoc(doc);
        return -1;
    }
    if (xmlStrcmp(cur->name, BAD_CAST"rpklist")) {
        xmlFreeDoc(doc);
        return -1;
    }
    xmlChar *updatetime = xmlGetProp(cur, BAD_CAST"updatetime");
    mUpdateTime = reinterpret_cast<char*>(updatetime);
    xmlNodePtr child = cur->xmlChildrenNode;
    while (child != NULL) {
        if (!xmlStrcmp(child->name, BAD_CAST"item")) {
            xmlChar *xname = xmlGetProp(child, BAD_CAST"name");
            if (xname == NULL) {
                child = child->next;
                continue;
            }
            string sname = reinterpret_cast<char*>(xname);
            if (sname.length() == 0) {
                continue;
            }
            if (using_json_file ? (mDlRpk.find(String::ToUpper(sname)) != string::npos) : true) {
                RpkFile *rfile = new RpkFile;
                if (rfile == NULL){
                    xmlFreeDoc(doc);
                    return -1;
                }
                rfile->name = sname;
                xmlChar *xsize = xmlGetProp(child, BAD_CAST"size");
                rfile->size = atoi(reinterpret_cast<char*>(xsize));
                xmlChar *xmd5 = xmlGetProp(child, BAD_CAST"md5");
                rfile->md5 = reinterpret_cast<char*>(xmd5);
                mDlFiles.push_back(rfile);
                printf("Download file info: %s  %d  %s\n", rfile->name.c_str(), rfile->size, rfile->md5.c_str());
            }
        }
        child = child->next;
    }
    xmlFreeDoc(doc);
    return 0;
}

string DlParser::GetMachineType() {
    struct utsname utsn;
    if (uname(&utsn) == 0) {
        if (Utils::String::StartsWith(utsn.machine, "mips")) {
            return string("mips64");
        }
        return utsn.machine;
    }
    return "";
}


