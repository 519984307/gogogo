#ifndef LOGDATALOCATION_H
#define LOGDATALOCATION_H

#include "abstractlogdatamodel.h"

class LogDataLocation
{
public:
    LogDataLocation();

private:
    string getLogType();
    Json::Value toJSON();

public:
    double mLng;
    double mLat;
};

#endif // LOGDATALOCATION_H
