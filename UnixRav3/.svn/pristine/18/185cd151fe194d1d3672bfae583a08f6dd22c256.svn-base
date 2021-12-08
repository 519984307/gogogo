#include "logdatabasic.h"

#include <sys/time.h>

#define TIME         "time"
#define EVENTTYPE    "eventtype"
#define EVENTLEVEL   "eventlevel"
#define EVENTSOURCE  "eventsource"
#define CATEGORY     "category"
#define USERNAME     "username"
#define UNIQUEVALUE  "uniquevalue"
#define MACHINENAME  "machinename"
#define GUID         "guid"
#define DESCRIPTION  "description"
#define VERSION      "version"
#define SOURCE       "source"

LogDataBasic::LogDataBasic() : mCategory(0), mUniquevalue(0) {
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);

    char out[128] = {0};
    struct tm *local = localtime(&tv.tv_sec);
    strftime(out, 128, "%Y-%m-%d %H:%M:%S", local);
    mTime = out;

    mUniquevalue = tv.tv_sec;
    mEventtype = EVENTLOG_SUCCESS;
    mEventlevel = ACTION_TAKEN_IMMEDIATELY;
    mCategory = 0;
}

string LogDataBasic::getLogType() {
    return ENUM_VALUE(LogTypeContent, LOG_TYPE_BASIC_PLATFORM);
}

Json::Value LogDataBasic::toJSON() {
    Json::Value value;
    value[TIME]         = Json::Value(mTime);
    value[EVENTTYPE]    = Json::Value(mEventtype);
    value[EVENTLEVEL]   = Json::Value(mEventlevel);
    value[EVENTSOURCE]  = Json::Value(mEventsource);
    value[CATEGORY]     = Json::Value(mCategory);
    value[USERNAME]     = Json::Value(mUsername);
    value[UNIQUEVALUE]  = Json::Value(mUniquevalue);
    value[DESCRIPTION]  = Json::Value(mDescription);

    value[MACHINENAME]  = Json::Value(mMachineName);
    value[GUID]         = Json::Value(mGUID);
    value[VERSION]      = Json::Value(mVersion);
    value[SOURCE]       = Json::Value(mSource);

    return value;
}
