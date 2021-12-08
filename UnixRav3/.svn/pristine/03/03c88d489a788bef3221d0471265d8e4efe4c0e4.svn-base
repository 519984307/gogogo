#include "urlruleentry.h"
#include "storage_global.h"
#include "CPPSQLite/cppsqlite3.h"

string UrlRuleEntry::CreateTable() const {
    return "CREATE TABLE IF NOT EXISTS UrlRuleTable( \
                            ID INTEGER PRIMARY KEY AUTOINCREMENT,\
                            CATEGORY INTEGER, \
                            TYPE INTEGER, \
                            PATH TEXT, \
                            SERIAL TEXT, \
                            CONT INTEGER, \
                            ALERT INTEGER, \
                            REDIR TEXT, \
                            VALID TEXT);";
}

string UrlRuleEntry::Select() const {
    char sql[128] = {0};
    sprintf(sql, "select * from UrlRuleTable");
    return string(sql);
}
string UrlRuleEntry::Insert() const {
    char sql[512] = {0};
    sprintf(sql, "insert into UrlRuleTable(CATEGORY, TYPE, PATH, SERIAL, CONT, ALERT, REDIR, VALID) values(%d, %d, '%s', '%s', %d, %d, '%s', '%s')",
            mCategory, mType, mPath.c_str(), mSerial.c_str(), mControl, mAlert, mRedirect.c_str(), mValidTime.c_str());
    return string(sql);
}

string UrlRuleEntry::Update() const {
    char sql[512] = {0};
    sprintf(sql, "update UrlRuleTable set PATH='%s',TYPE=%d,CATEGORY=%d,SERIAL='%s',CONT=%d,ALERT=%d,REDIR='%s',VALID='%s' where ID=%ld",
            mPath.c_str(), mType,mCategory, mSerial.c_str(), mControl, mAlert,mRedirect.c_str(),mValidTime.c_str(), mID);
    return string(sql);
}
string UrlRuleEntry::Delete() const {
    char sql[128] = {0};
    sprintf(sql, "delete from UrlRuleTable where ID=%ld", mID);
    return string(sql);
}

void UrlRuleEntry::Mapping(CppSQLite3Query *query) {
    mID = query->getIntField(0, 0);
    mCategory = (RULE_CATEGORY)query->getIntField(1, 0);
    mType = (RULE_TYPE)query->getIntField(2, 0);
    mPath = query->getStringField(3, NULL);
    mSerial = query->getStringField(4, NULL);
    mControl = query->getIntField(5, 0);
    mAlert = query->getIntField(6, 0);
    mRedirect = query->getStringField(7, 0);
    mValidTime = query->getStringField(8, 0);
}

