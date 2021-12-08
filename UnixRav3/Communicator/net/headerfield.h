#ifndef HTTPREQUESTHEADERMODEL_H
#define HTTPREQUESTHEADERMODEL_H

#include <string>
#include <list>

#include "json/json.h"

using namespace std;

class HttpRequestHeaderModel
{
public:
    HttpRequestHeaderModel(string eid,
                           string guid,
                           string pstamp,
                           string cstamp,
                           string sguid,
                           string version,
                           string systype);

    HttpRequestHeaderModel(Json::Value json);

    int refresh(Json::Value json);
    int refresh(const string& vinfo);

    bool verify(const string& verify);

    bool makeHeader(string& header, std::list<int> commands);
    int  parseHeader(const string& header);

    bool operator ==(const HttpRequestHeaderModel& model);
    bool operator +=(const HttpRequestHeaderModel& model);

    string getSGUID() const {
        return mSGuid;
    }

    string getTespan() const;
    enum {
        SGUID_INIT      = 1,
        TESPAN_CHANGE   = 2,
        IS_TOMORROW     = 4,
        EID_CHANGE      = 8,
    };

    long getServerTime() const;
    string GetEID() const {
        return mEid;
    }

private:
    Json::Value makeHeader(std::list<int> commands);

    bool DateChanged(time_t last, time_t current);

private:
    /** 企业标识 */
    std::string mEid;
    /** 客户端生成的唯一标识 */
    std::string mGuid;
    /** 策略时间戳 */
    std::string mPStamp;
    /** 命令流水号 */
    std::string mCStamp;
    /** 客户端请求服务器的心跳周期，单位毫秒 */
    int mTespan;
    /** 服务器生成的客户端唯一标识 */
    std::string mSGuid;
    /** 客户端版本号 */
    std::string mVersion;
    /** 日志上报类型，1按心跳2立即3连续4全部(立即+连续)5不上报，  默认为4 */
    int mLrtype;
    /** 日志上报大小，0为按客户端限制  非0按服务器端限制 ，单位K，  默认为0 */
    int mLrsize;
    /** 操作系统类型为字符串由客户端上传 */
    std::string mSysType;
    /** 通讯协议版本号 */
    std::string mProtecolVersion;
    /** 验证信息，服务器下发 */
    std::string mVeriryInfo;
    /** server time */
    long mServerTime;
};

#endif // HTTPREQUESTHEADERMODEL_H
