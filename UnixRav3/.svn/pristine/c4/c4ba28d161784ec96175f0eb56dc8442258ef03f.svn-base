#ifndef APISERVICE_H
#define APISERVICE_H

using namespace std;

#include <string>
#include <list>

#include "models/modeldefine.h"

class APICallback {
public:
    typedef enum {
        HEAD,
        BODY
    }CALLBACK_EVENT;
    typedef enum {
        BODY_ENTRY = 0,
        SGUID_INIT,
        TESPAN_CHANGE,
        DATE_CHANGED,
        EID_CHANGED,
        HEAD_ERROR
    }CALLBACK_TYPE;
    virtual int onRequestCallback(CALLBACK_EVENT event, CALLBACK_TYPE TYPE, const string& data) = 0;
};

class HttpRequestHeaderModel;
class APIService
{
protected:
    APIService(string baseUrl,
               string eid,
               string guid,
               string pstamp,
               string cstamp,
               string sguid,
               string version,
               string systype);

public:
    ~APIService();
    void setObserver(APICallback *callback) {
        mObserver = callback;
    }

    int heartBeat();
    int reportData(list<int> commands, const string& body);
    int reportData(Command command, const string& body);

    int GetServerTime() const;
	int GetLocalIp(std::string & ip);

private:
    string generalHeader(std::list<int> commands);
    int httpRequest(int method, const string& url, const list<int>& commands, const string& body, APICallback *callback);

    bool parseHeader(const string& header, APICallback *callback);
    bool parseBody(const string& body, APICallback *callback);
	void ParserConnInfo(const string & conn);


private:
    enum {
        HTTP_GET = 0x0,
        HTTP_POST
    };
private:
    HttpRequestHeaderModel  *mHeaderModel;
    APICallback             *mObserver;

    pthread_mutex_t mRequestMutex, m_dlock;

    std::string mUrl, m_local_ip;

public:
    class Builder {
    public:
        string 	mBaseUrl;
        /** 企业标识 */
        string mEid;
        /** 客户端生成的唯一标识 */
        string mGuid;
        /** 策略时间戳 */
        string mPStamp;
        /** 命令流水号 */
        string mCStamp;
        /** 客户端请求服务器的心跳周期，单位毫秒 */
        long mTespan;
        /** 服务器生成的客户端唯一标识 */
        string mSGuid;
        /** 客户端版本号 */
        string mVersion;
        /** 日志上报类型，1按心跳2立即3连续4全部(立即+连续)5不上报，  默认为4 */
        int mLrsize;
        /** 操作系统类型为字符串由客户端上传 */
        string mSysType;
        /** 通讯协议版本号 */
        string mProtecolVersion;
        /** 验证信息，服务器下发 */
        string mVeriryInfo;

    public:
        Builder() {
            mTespan = 0;
            mLrsize = 0;
        }

        Builder& baseUrl(string url) {
            mBaseUrl = url;
            return *this;
        }

        Builder& eid(string eid) {
            mEid = eid;
            return *this;
        }

        Builder& guid(string guid) {
            mGuid = guid;
            return *this;
        }

        Builder& pStamp(string pStamp) {
            mPStamp = pStamp;
            return *this;
        }

        Builder& cStamp(string cstamp) {
            mCStamp = cstamp;
            return *this;
        }

        Builder& tespan(long tespan) {
            mTespan = tespan;
            return *this;
        }

        Builder& sguid(string sguid) {
            mSGuid = sguid;
            return *this;
        }

        Builder& version(string version) {
            mVersion = version;
            return *this;
        }

        Builder& system(string systype) {
            mSysType = systype;
            return *this;
        }

        APIService* build() {
            if (mBaseUrl.empty()) {
                throw string("base must not be null");
            }
            if (mEid.empty()) {
                throw string("eid must not be null");
            }
            if (mVersion.empty()) {
                throw string("version must not be null");
            }
            if (mGuid.empty()) {
                throw string("version must not be null");
            }
            return new APIService(mBaseUrl, mEid, mGuid, mPStamp, mCStamp, mSGuid, mVersion, mSysType);
        }
    };
};

#endif // APISERVICE_H
