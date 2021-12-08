#ifndef ABSTRACTLOGDATAMODEL_H
#define ABSTRACTLOGDATAMODEL_H

#include <string>
#include <json/json.h>

/**
 * All model class the parent
 * @brief The AbstractLogDataModel class
 */

using namespace std;
class AbstractLogDataModel
{
public:
    virtual string getLogType()     = 0;
    virtual Json::Value toJSON()    = 0;
};

#endif // ABSTRACTLOGDATAMODEL_H
