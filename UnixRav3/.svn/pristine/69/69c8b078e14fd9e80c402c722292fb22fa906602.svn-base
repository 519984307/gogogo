#include "pluginentry.h"
#include "commonapi.h"

std::string PublicScanData::JSON() {
    Json::Value json;

    std::list<std::string>::iterator iter;
    for (iter=files.begin();iter!=files.end();++iter) {
        json["scan_files"] = Json::Value(*iter);
    }
    json["scan_memory"] = Json::Value(memory);

    Json::FastWriter writer;
    return writer.write(json);
}

bool PublicScanData::Internalize(const unsigned char *data, int len) {
    std::string content = std::string((char*)data);

    Json::Reader reader;
    Json::Value json;
    if (!reader.parse(content, json)) {
        return false;
    }

    if (json.isMember("scan_files")) {
        Json::Value contentArray = json["scan_files"];
        for (uint i=0; i<contentArray.size();i++) {
            Json::Value content = contentArray[i];
            files.push_back(content.asString());
        }
    }

    if (json.isMember("scan_memory")) {
        Json::Value memoryValue = json["scan_memory"];
        memory = memoryValue.asBool();
    }
    return true;
}

bool PublicScanData::Internalize(int fd) {
    unsigned char buffer[2048];
    int readed = Utils::OS::Read(fd, 2048, buffer);
    return Internalize(buffer, readed);
}

int PublicScanData::CalculateSize() {
    return 0;
}

std::string PublicScanResult::JSON() {
    Json::Value ret;

    std::list<struct PublicScanResult::ScanResult>::iterator iter;
    for (iter=results.begin(); iter!=results.end(); ++iter) {
        Json::Value json;
        json["filename"]    = Json::Value(iter->file);
        json["md5"]         = Json::Value(iter->hash);
        json["virus"]       = Json::Value(iter->hits);
        json["virus_type"]  = Json::Value(iter->type);
        json["virus_name"]  = Json::Value(iter->virus);

        ret["scan_results"] = Json::Value(json);
    }

    Json::FastWriter writer;
    return writer.write(ret);
}

int PublicScanResult::CalculateSize() {
    return 0;
}


bool PublicScanResult::Externalize(int fd) {
    std::string json = JSON();
    return Utils::OS::Write(fd, json.size(), (unsigned char*)json.c_str()) == json.size();
}
