#include "commandmodel.h"
#include "commonapi.h"
#include "communicator_global.h"

#define PRODUCTID    "productid"
#define ID           "id"
#define TYPE         "type"
#define CMDTYPE      "cmdtype"
#define CMDVER       "cmdver"
#define CMDID        "cmdid"
#define CMDCONTENTBUF  "cmdcontentbuf"

CommandModel* CommandModel::NEW(Json::Value json, bool decrypted) {
    CommandModel *self = new CommandModel;
    if (self) {
        int err = self->Contruct(json, decrypted);
        if (err) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

CommandModel::CommandModel() : mType(0), mCmdId(0) {}

int CommandModel::Contruct(Json::Value json, bool decrypted) {
    mContentCrypted = !decrypted;

    try {
        mProductId = json[PRODUCTID].asString();
        mId = json[ID].asString();
        mType = Utils::JSON::GetJsonInt(json, TYPE);
        mCmdType = (CommandType)Utils::JSON::GetJsonInt(json, CMDTYPE);
        mCmdVer = json[CMDVER].asString();
        mCmdId = Utils::JSON::GetJsonInt(json, CMDID);

        std::string cmd = json[CMDCONTENTBUF].asString();
        if (!cmd.empty()) {
            if (decrypted) {
                mCmdContentBuf = Utils::Hash::Base64Decode(cmd);
            }
            else {
                mCmdContentBuf = cmd;
            }
        }
    }
    catch(...) {
        return -1;
    }
    return 0;
}

std::string CommandModel::GetMessageContent() {
    if (mContentCrypted) {
        return Utils::Hash::Base64Decode(mCmdContentBuf);
    }
    return mCmdContentBuf;
}

bool CommandModel::validate() {
    switch (mCmdType){
        case NOTIFICATION:
            return mProductId == PRODUCT_GUID;
//                    ConstDefined.AVAILED_GUID(mProductId);
        case REPORT_LOCATION:
            return mProductId == PRODUCT_GUID;
        default:
            break;
    }
    return false;
}

void CommandModel::operator =(const CommandModel& model) {
    mProductId = model.mProductId;
    mId = model.mId;
    mType = model.mType;
    mCmdType = model.mCmdType;
    mCmdVer = model.mCmdVer;
    mCmdId = model.mCmdId;
    mCmdContentBuf = model.mCmdContentBuf;
}

std::string CommandModel::JSONString() {
    Json::Value json;
    json[PRODUCTID] = Json::Value(mProductId);
    json[ID] = Json::Value(mId);
    json[TYPE] = Json::Value(mType);
    json[CMDTYPE] = Json::Value(mCmdType);
    json[CMDVER] = Json::Value(mCmdVer);
    json[CMDCONTENTBUF] = Json::Value(mCmdContentBuf);
    json[CMDID] = Json::Value(mCmdId);
    Json::FastWriter writer;
    return writer.write(json);
}
