#ifndef BASEENTRY_H
#define BASEENTRY_H

#include <string>
#include <json/json.h>

class CppSQLite3Query;
class BaseEntry
{
public:
    BaseEntry() {
        mID = 0;
    }

public:
    long mID;

protected:
    virtual std::string CreateTable() const = 0;
    virtual std::string Select() const = 0;
    virtual std::string Insert() const = 0;
    virtual std::string Update() const = 0;
    virtual std::string Delete() const = 0;
    virtual void Mapping(CppSQLite3Query *query) = 0;
};

class Serializable {
public:
    bool Initialize(const std::string& data);
    std::string Externlize();

public:
    virtual void InitializeJson(Json::Value& json) = 0;
    virtual void ExternlizeJson(Json::Value& json) = 0;
};

template<typename T>
class LockField {
public:
    bool    mLocked;
    T       mValue;

    LockField<T>() {
        mLocked = false;
    }
};


#endif // BASEENTRY_H
