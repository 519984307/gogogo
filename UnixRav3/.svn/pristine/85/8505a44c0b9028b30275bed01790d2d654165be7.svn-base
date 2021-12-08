#include "logdataspamphone.h"
#include "../modeldefine.h"

#define PHONE    "phone"
#define DATE     "date"
#define REASON   "reason"

LogDataSpamPhone::LogDataSpamPhone()
{

}
string LogDataSpamPhone::getLogType() {
    return ENUM_VALUE(LogTypeContent, LOG_TYPE_SPAM_PHONE);
}

Json::Value LogDataSpamPhone::toJSON() {
    Json::Value value;

    value[PHONE] = Json::Value(mPhone);
    value[DATE] = Json::Value(mDate);
    value[REASON] = Json::Value(mReason);

    return value;
}
