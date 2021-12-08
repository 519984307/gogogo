#include "logepstate.h"

#define KEY          "key"
#define VALUE        "value"

LogEPState::LogEPState()
{
    mEventtype = EVENTLOG_SUCCESS;
    mEventlevel = ACTION_TAKEN_IMMEDIATELY;
    mCategory = 0;
}

LogEPState::LogEPState(const string& eventSource,
                       const string& username,
                       const string& key,
                       const string& value) {
    mEventtype = EVENTLOG_SUCCESS;
    mEventlevel = ACTION_TAKEN_IMMEDIATELY;
    mCategory = 0;

    mEventsource = eventSource;
    mUsername = username;

    mKey = key;
    mValue = value;
}

Json::Value LogEPState::toJSON() {
    Json::Value value = LogDataBasic::toJSON();
    if (!mKey.empty()) {
        value[KEY] = Json::Value(mKey);
    }
    if (!mValue.empty()) {
        value[VALUE] = Json::Value(mValue);
    }
    return value;
}
