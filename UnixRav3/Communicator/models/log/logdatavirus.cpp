#include "logdatavirus.h"
#include "commonapi.h"
#include "../modeldefine.h"
#include <strings.h>

#define TASKID       "taskid"
#define TASKNAME     "taskname"
#define APPID        "appid"
#define FINDTIME     "findtime"
#define FILEPATH     "filepath"
#define VIRUSID      "virusid"
#define ENGID        "engid"
#define VIRUSCLASS   "virusclass"
#define VIRUSNAME    "virusname"
#define TREATMETHOD  "treatmethod"
#define STATE        "state"
#define PARENTID     "parentid"
#define UNIQUEVALUE  "uniquevalue"

LogDataVirus::LogDataVirus(LogDataScanEvent::LogScanType type,
                           LogDataScanEvent::CommandSender sender,
                           const string& eventSource,
                           const string& username,
                           const string& mname,
                           const string& guid,
                           const string& version,
                           const string& source) {
    mEventtype = EVENTLOG_INFORMATION_TYPE;
    mEventlevel = INFORMATIONAL_MESSAGE;
    mCategory = 0;

    mEventsource = eventSource;
    mUsername = username;
    mMachineName = mname;
    mGUID = guid;
    mVersion = version;
    mSource = source;

    mTaskid = Utils::Date::GetTimeStamp() + rand()%1000000 + rand()%100000;
    mUniquevalue = mTaskid;
    mAppid = sender;

    if (type == LogDataScanEvent::QUICK_SCAN) {
        mTaskname = "quickscan";
    }
    else if (type == LogDataScanEvent::ALL_SCAN) {
        mTaskname = "allscan";
    }
    else if (type == LogDataScanEvent::CUSTOM_SCAN) {
        mTaskname = "customscan";
    }
    else if (type == LogDataScanEvent::TIME_SCAN) {
        mTaskname = "allscan";
    }
    else if (type == LogDataScanEvent::FILEMON_SCAN) {
        mTaskname = "filemon";
    }

    mVirusid = 0;
    mTreatmethod = 0;
    mState = 0;

    // FIXME: why???
    mParentid = 13;
    mEngid = 12;
}

string LogDataVirus::getLogType() {
    return ENUM_VALUE(LogTypeContent, LOG_TYPE_VIRUS_RECORD);
}

Json::Value LogDataVirus::toJSON() {
    Json::Value value = LogDataBasic::toJSON();

    value[TASKID] = Json::Value(mTaskid);
    value[TASKNAME] = Json::Value(mTaskname);
    value[APPID] = Json::Value(mAppid);
    value[FINDTIME] = Json::Value(mFindtime);
    value[FILEPATH] = Json::Value(mFilepath);
    value[VIRUSID] = Json::Value(mVirusid);
    value[ENGID] = Json::Value(mEngid);
    value[VIRUSCLASS] = Json::Value(mVirusclass);
    value[VIRUSNAME] = Json::Value(mVirusname);
    value[TREATMETHOD] = Json::Value(mTreatmethod);
    value[STATE] = Json::Value(mState);
    value[PARENTID] = Json::Value(mParentid);
    value[UNIQUEVALUE] = Json::Value(mUniquevalue);

    return value;
}


static const std::string en_rs_virus_type[] = {"Suspicious File", "Virus", "Worm","Rootkit",
                                               "Adware", "Trojan", "Backdoor", "Spyware"};
//{ 0, "可疑" }, { 1, "病毒" }, { 2, "蠕虫" }, { 3, "rookit" }, { 4, "广告" }, { 5, "木马" }, { 6, "后门" }, { 7, "壳" }
//8 unknow
static int FindVirusType(const string& virus)
{
    int i;
    int i_size = sizeof(en_rs_virus_type)/sizeof(std::string);
    std::string vtype = virus.substr(0, virus.find("."));
    for(i=0; i < i_size; i++){
        if(strcasecmp(vtype.c_str(), en_rs_virus_type[i].c_str()) == 0){
            return i;
        }
    }

    return 0;
}

void LogDataVirus::SetVirusData(long foundtime, const string &filepath, int virusid, const string &virusname, int deal, int state) {
    mFindtime = Utils::Date::FormatTimeStamp(foundtime);
    mFilepath = filepath;
    mVirusid = virusid;
    mVirusname = virusname;
    // TODO
    mVirusclass = FindVirusType(virusname);
    mTreatmethod = deal;
    mState = state;
}
