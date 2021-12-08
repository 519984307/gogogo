#include "communicatebundle.h"
#include "json/json.h"
#include "commonapi.h"

bool CommunicateBundle::Internalize(const char *data, int len) {
    const_cast<char*>(data)[len] = '\0';
    std::string content = std::string(data);

#ifdef COMMON_JSON_API
    Json::Reader reader;
    Json::Value json;
    if (!reader.parse(content, json)) {
        return false;
    }

    if (json.isMember("flag")) {
        Json::Value value = json["flag"];
        mFlag = static_cast<enum _CommunicateType>(value.asInt());
    }

    if (json.isMember("title")) {
        Json::Value value = json["title"];
        mTitle = value.asString();
    }

    if (json.isMember("type")) {
        Json::Value value = json["type"];
        mType = static_cast<enum _NotifyType>(value.asInt());
    }

    if (json.isMember("content")) {
        Json::Value value = json["content"];
        mContent = value.asString();
    }

    if (json.isMember("left")) {
        Json::Value value = json["left"];
        mButtonTitle1 = value.asString();
    }

    if (json.isMember("right")) {
        Json::Value value = json["right"];
        mButtonTitle2 = value.asString();
    }
#endif
    return true;
}

std::string CommunicateBundle::Externalize() {
#ifdef COMMON_JSON_API
    Json::Value json;
    json["flag"] = Json::Value(mFlag);
    json["type"] = Json::Value(mType);
    if (!mTitle.empty()) {
        json["title"] = Json::Value(mTitle);
    }
    if (!mContent.empty()) {
        json["content"] = Json::Value(mContent);
    }
    if (!mButtonTitle1.empty()) {
        json["left"] = Json::Value(mButtonTitle1);
    }
    if (!mButtonTitle2.empty()) {
        json["right"] = Json::Value(mButtonTitle2);
    }

    Json::FastWriter writer;
    return writer.write(json);
#else
    return "";
#endif
}

int CommunicateBundle::Externalize(char **data, int *len) {
#ifdef COMMON_JSON_API
    Json::Value json;
    json["flag"] = Json::Value(mFlag);
    json["type"] = Json::Value(mType);
    if (!mTitle.empty()) {
        json["title"] = Json::Value(mTitle);
    }
    if (!mContent.empty()) {
        json["content"] = Json::Value(mContent);
    }
    if (!mButtonTitle1.empty()) {
        json["left"] = Json::Value(mButtonTitle1);
    }
    if (!mButtonTitle2.empty()) {
        json["right"] = Json::Value(mButtonTitle2);
    }

    Json::FastWriter writer;
    std::string ret = writer.write(json);
    *len = static_cast<int>(ret.size());
    *data = new char[ret.size()];
    memcpy(*data, ret.c_str(), static_cast<size_t>(*len));
#endif
    return 0;
}

bool CommunicateBundle::Externalize(int fd) {
    if (mTempBuffer.empty()) {
#ifdef COMMON_JSON_API
        Json::Value json;
        json["flag"] = Json::Value(mFlag);
        json["type"] = Json::Value(mType);
        if (!mTitle.empty()) {
            json["title"] = Json::Value(mTitle);
        }
        if (!mContent.empty()) {
            json["content"] = Json::Value(mContent);
        }
        if (!mButtonTitle1.empty()) {
            json["left"] = Json::Value(mButtonTitle1);
        }
        if (!mButtonTitle2.empty()) {
            json["right"] = Json::Value(mButtonTitle2);
        }

        Json::FastWriter writer;
        mTempBuffer = writer.write(json);
#endif
    }

    return Utils::OS::Write(
                fd,
                static_cast<int>(mTempBuffer.size()),
                reinterpret_cast<unsigned char*>(const_cast<char*>(mTempBuffer.c_str()))
            ) == static_cast<int>(mTempBuffer.size());
 }

int CommunicateBundle::CalculateSize() {
    mTempBuffer = "";

#ifdef COMMON_JSON_API
    Json::Value json;
    json["flag"] = Json::Value(mFlag);
    json["type"] = Json::Value(mType);
    if (!mTitle.empty()) {
        json["title"] = Json::Value(mTitle);
    }
    if (!mContent.empty()) {
        json["content"] = Json::Value(mContent);
    }
    if (!mButtonTitle1.empty()) {
        json["left"] = Json::Value(mButtonTitle1);
    }
    if (!mButtonTitle2.empty()) {
        json["right"] = Json::Value(mButtonTitle2);
    }

    Json::FastWriter writer;
    mTempBuffer = writer.write(json);
#endif

    return static_cast<int>(mTempBuffer.size());
}


bool CommandBundle::Internalize(const char *data, int len) {
    if (len < CalculateSize()) {
        return false;
    }

    Reset();
    memcpy(&mCommandID, data, sizeof(int));
    memcpy(&mIntegerValue, data+sizeof(int), sizeof(int));
    if (len > 8) {
        mStringValue = std::string(data+sizeof(int)*2, static_cast<size_t>(len)-sizeof(int)*2);
    }
    return true;
}

int  CommandBundle::Externalize(char **data, int *len) {
    *len = sizeof(int)*2 + mStringValue.size();
    *data = new char[*len];
    memcpy(*data, &mCommandID, sizeof(int));
    memcpy((*data)+sizeof(int), &mIntegerValue, sizeof(int));
    memcpy((*data)+sizeof(int)*2, mStringValue.c_str(), mStringValue.size());
    return 0;
}

bool CommandBundle::Externalize(int fd) {
    if (sizeof(int) != Utils::OS::Write(fd, sizeof(int), reinterpret_cast<unsigned char*>(&mCommandID))) {
        return false;
    }
    if (sizeof(int) != Utils::OS::Write(fd, sizeof(int), reinterpret_cast<unsigned char*>(&mIntegerValue))) {
        return false;
    }
    if (!mStringValue.empty()) {
        int reallen = CalculateSize()-8;
        return reallen == Utils::OS::Write(fd,
                             reallen,
                             reinterpret_cast<unsigned char*>(const_cast<char*>(mStringValue.c_str())));
    }

    return true;
}

int  CommandBundle::CalculateSize() {
    return static_cast<int>(sizeof(int) + sizeof(int) + mStringValue.size());
}
