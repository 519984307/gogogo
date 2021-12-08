#include "headerfield.h"
#include "commonapi.h"
#include "communicator_global.h"
#include "baseconfig.h"

#include <stdio.h>

#define EID     "eid"
#define GUID    "guid"
#define SGUID   "sguid"
#define VERSION "ver"
#define PSTAMP  "pstamp"
#define CSTAMP  "cstamp"
#define TESPAN  "tespan"
#define LRSIZE  "lrsize"
#define LRTYPE  "lrtype"
#define SYSTYPE "systype"
#define VINFO   "vinfo"
#define STIME   "stime"


HttpRequestHeaderModel::HttpRequestHeaderModel(string eid,
                                               string guid,
                                               string pstamp,
                                               string cstamp,
                                               string sguid,
                                               string version,
                                               string systype)
    :mLrsize(0), mLrtype(0),mTespan(0)
{
    mEid = eid;
    mGuid = guid;
    mPStamp = pstamp;
    mCStamp = cstamp;
    mSGuid = sguid;
    mVersion = version;
    mSysType = systype;

    mLrsize = 0;
    mLrtype = 0;
    mProtecolVersion = "1.0";
    mServerTime = 0;
}

HttpRequestHeaderModel::HttpRequestHeaderModel(Json::Value json) {
    refresh(json);
}

string HttpRequestHeaderModel::getTespan() const {
   char temp[8] = {0};
   sprintf(temp, "%d", mTespan);
   return temp;
}

long HttpRequestHeaderModel::getServerTime() const {
    return mServerTime;
}

bool HttpRequestHeaderModel::DateChanged(time_t last, time_t current) {
    if (current > 0) {
        struct tm *nowTime = localtime(&last);
        int nowYear = nowTime->tm_year;
        int nowMonth = nowTime->tm_mon;
        int nowDay = nowTime->tm_mday;

        struct tm *authTime = localtime(&current);
        if (nowYear == authTime->tm_year &&
                nowMonth == authTime->tm_mon &&
                nowDay == authTime->tm_mday) {
            // has authorized
           return false;
        }
    }
    return true;
}

int HttpRequestHeaderModel::refresh(Json::Value value) {
    mVersion= Utils::JSON::GetJsonString(value, VERSION);
    mPStamp = Utils::JSON::GetJsonString(value, PSTAMP);
    mCStamp = Utils::JSON::GetJsonString(value, CSTAMP);
    mLrsize = Utils::JSON::GetJsonInt(value, LRSIZE);
    mLrtype = Utils::JSON::GetJsonInt(value, LRTYPE);
    mVeriryInfo  = Utils::JSON::GetJsonString(value, VINFO);

    int ret = 0;

    long stime = Utils::JSON::GetJsonInt(value, STIME);
    if (DateChanged(mServerTime, stime)) {
        ret |= IS_TOMORROW;
    }
    mServerTime = stime;

    string sguid  = Utils::JSON::GetJsonString(value, SGUID);
    if (sguid != mSGuid) {
        ret |= SGUID_INIT;
        mSGuid = sguid;
    }

    int tespan = Utils::JSON::GetJsonInt(value, TESPAN);
    if (tespan != mTespan) {
        ret |= TESPAN_CHANGE;
        mTespan = tespan;
        LOG_GLTX("mTespan = tespan: %d", tespan);
    }

    string eid = Utils::JSON::GetJsonString(value, EID);
    if (eid != mEid) {
        ret |= EID_CHANGE;
        mEid = eid;
    }

    return ret;
}

int HttpRequestHeaderModel::refresh(const string& vinfo) {
    if (vinfo.empty()) {
        return -1;
    }
    Json::Reader reader;
    Json::Value json;

    if (reader.parse(vinfo, json)) {
        return refresh(json);
    }

    return -1;
}

bool HttpRequestHeaderModel::verify(const string& verify) {
    if (verify.empty()) {
        return false;
    }

    string decrypt = Utils::Hash::ESMDecrypt(verify);
    if (decrypt.empty()) {
        return false;
    }

    Json::Reader reader;
    Json::Value json;
    if (reader.parse(verify, json)) {
        HttpRequestHeaderModel model(json);
        return *this == model;
    }
    return false;
}

bool HttpRequestHeaderModel::operator ==(const HttpRequestHeaderModel& m) {
    return m.mEid == this->mEid
            && m.mGuid == this->mGuid
            && m.mSGuid == this->mSGuid
            && m.mSysType == this->mSysType;
}

bool  HttpRequestHeaderModel::operator +=(const HttpRequestHeaderModel& model) {
    mSGuid = model.mSGuid;
    mPStamp = model.mPStamp;
    mCStamp = model.mCStamp;
    mTespan = model.mTespan;
    mLrsize = model.mLrsize;
    mLrtype = model.mLrtype;
    mVeriryInfo = model.mVeriryInfo;
    return true;
}

Json::Value HttpRequestHeaderModel::makeHeader(std::list<int> commands) {
    Json::Value params;

    params[EID] = Json::Value(mEid);
    params[GUID] = Json::Value(mGuid);
    params[SGUID] = Json::Value(mSGuid);
    params[VERSION] = Json::Value(mVersion);
    params[PSTAMP] = Json::Value(mPStamp);
    params[CSTAMP] = Json::Value(mCStamp);
    params[TESPAN] = Json::Value(mTespan);
    params[LRSIZE] = Json::Value(mLrsize);
    params[LRTYPE] = Json::Value(mLrtype);
    params[SYSTYPE] = Json::Value(mSysType);
    params[VINFO] = Json::Value(mVeriryInfo);

    Json::Value commandStringArray;
    list<int>::iterator iter;
    for (iter=commands.begin(); iter!=commands.end(); ++iter) {
        commandStringArray.append(*iter);
    }

    if (commandStringArray.size() > 0) {
        params["cmsgtypelist"] = commandStringArray;
    }
    return params;
}

bool HttpRequestHeaderModel::makeHeader(string& header, std::list<int> commands) {
    Json::Value params = makeHeader(commands);

    Json::FastWriter writer;
    string result = writer.write(params);
    if (!result.empty()) {

        LOG_GLTX("header request is: %s", result.c_str());

        header = Utils::Hash::ESMEncrypt(result);
        return header.size()>0;
    }
    return false;
}

int HttpRequestHeaderModel::parseHeader(const string& header) {
    if (header.empty()) {
        return -1;
    }

    string decrypt = Utils::Hash::ESMDecrypt(header);

    LOG_GLTX("header respond is: %s\n", decrypt.c_str());

    return refresh(decrypt);
}

