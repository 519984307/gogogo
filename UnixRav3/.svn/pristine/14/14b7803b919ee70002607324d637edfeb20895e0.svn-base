#include "logdataspammsg.h"
#include "../modeldefine.h"

#define CONTENT  "content"
#define PHONE    "phone"
#define DATE     "date"
#define REASON   "reason"

LogDataSpamMsg::LogDataSpamMsg()
{

}

string LogDataSpamMsg::getLogType() {
    return ENUM_VALUE(LogTypeContent, LOG_TYPE_SPAM_MESSAGE);
}

Json::Value LogDataSpamMsg::toJSON() {
    Json::Value value;

    value[CONTENT] = Json::Value(mContent);
    value[PHONE] = Json::Value(mPhone);
    value[DATE] = Json::Value(mDate);
    value[REASON] = Json::Value(mReason);

    return value;
}
