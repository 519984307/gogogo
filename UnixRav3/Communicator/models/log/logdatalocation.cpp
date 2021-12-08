#include "logdatalocation.h"
#include "../modeldefine.h"

#include <stdio.h>

#define  LNG  "lng"
#define  LAT  "lat"

LogDataLocation::LogDataLocation() {

}

string LogDataLocation::getLogType() {
    return ENUM_VALUE(LogTypeContent, LOG_TYPE_LOC_REPORT);
}

Json::Value LogDataLocation::toJSON() {
    Json::Value value;

    char temp[32] = {0};
    sprintf(temp, "%f", mLng);
    value[LNG]    = Json::Value(temp);

    sprintf(temp, "%f", mLat);
    value[LAT]    = Json::Value(temp);

    return value;
}
