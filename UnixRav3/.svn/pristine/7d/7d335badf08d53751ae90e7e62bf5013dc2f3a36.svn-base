#include "baseentry.h"
#include "json/json.h"

bool Serializable::Initialize(const std::string& data) {
    Json::Reader reader;
    Json::Value json;
    if (reader.parse(data, json)) {
        InitializeJson(json);
        return true;
    }
    return false;
}

std::string Serializable::Externlize() {
    Json::Value json;
    ExternlizeJson(json);
    Json::FastWriter writer;
    return writer.write(json);
}
