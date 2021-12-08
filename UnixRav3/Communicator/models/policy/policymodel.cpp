#include "policymodel.h"
#include "crypt/base64.h"
#include "communicator_global.h"
#include "commonapi.h"

#include <string.h>


#define POLICYPRODUCTID  "policyproductid"
#define GROUPTYPE        "grouptype"
#define POLICYTYPE       "policytype"
#define POLICYVER        "policyver"
#define POLICYMAINCONTENT  "policymaincontent"

PolicyModel* PolicyModel::NEW(Json::Value value, bool crypted) {
    PolicyModel *self = new PolicyModel;
    if (self) {
        int err = self->Construct(value, crypted);
        if (err) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

PolicyModel::PolicyModel() :mGroupType(0), mPolicyType(0) {

}

int PolicyModel::Construct(Json::Value value, bool crypted) {
    mContentCrypted = crypted;

    try {
        mPolicyProductID = Utils::JSON::GetJsonString(value, POLICYPRODUCTID);
        mGroupType = Utils::JSON::GetJsonInt(value, GROUPTYPE);
        mPolicyType = Utils::JSON::GetJsonInt(value, POLICYTYPE);
        mPolicyVer = Utils::JSON::GetJsonString(value, POLICYVER);
        std::string cmd = Utils::JSON::GetJsonString(value, POLICYMAINCONTENT);
        if (!cmd.empty()) {
            if (crypted) {
                mPolicyMainContent = base64_decode(cmd);
            }
            else {
                mPolicyMainContent = cmd;
            }
        }
    }
    catch (...) {
        return -1;
    }
    return 0;
}

bool PolicyModel::validate() {
    if (mPolicyProductID == PRODUCT_GUID) {
        return true;
    }

    bool result = false;
    AVAILED_GUID(mPolicyProductID.c_str(), result);
    return result;
}

void PolicyModel::InitializeJson(Json::Value& json) {
    mPolicyProductID = json[POLICYPRODUCTID].asString();
    mGroupType = json[GROUPTYPE].asInt();
    mPolicyType = json[POLICYTYPE].asInt();
    mPolicyVer = json[POLICYVER].asString();
    mPolicyMainContent = json[POLICYMAINCONTENT].asString();
}

void PolicyModel::ExternlizeJson(Json::Value& json) {
    json[POLICYPRODUCTID] = Json::Value(mPolicyProductID);
    json[GROUPTYPE] = Json::Value(mGroupType);
    json[POLICYTYPE] = Json::Value(mPolicyType);
    json[POLICYVER] = Json::Value(mPolicyVer);
    json[POLICYMAINCONTENT] = Json::Value(mPolicyMainContent);
}

std::string PolicyModel::JSONString() {
    Json::Value json;
    ExternlizeJson(json);

    Json::FastWriter writer;
    return writer.write(json);
}

std::string PolicyModel::GetPolicyContent() {
    if (!mContentCrypted) {
         return base64_decode(mPolicyMainContent);
    }
    return mPolicyMainContent;
}
