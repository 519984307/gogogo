#include "apiservice.h"
#include "httprequest.h"
#include "communicator_global.h"
#include "commonapi.h"
#include "baseconfig.h"

#include "headerfield.h"
#include "models/messagepackage.h"

#include <pthread.h>

APIService::APIService(string baseUrl, string eid, string guid,
                       string pstamp, string cstamp,
                       string sguid, string version, string systype) {

    mHeaderModel = new HttpRequestHeaderModel(eid, guid, pstamp, cstamp, sguid, version, systype);
    mObserver = NULL;
    mUrl = baseUrl;

    pthread_mutex_init(&mRequestMutex, NULL);
	pthread_mutex_init(&m_dlock, NULL);
}

APIService::~APIService() {
    SECURITY_FREE(mHeaderModel);

    pthread_mutex_destroy(&mRequestMutex);
	pthread_mutex_destroy(&m_dlock);
}

string APIService::generalHeader(std::list<int> commands) {
    string info;
    if (mHeaderModel->makeHeader(info, commands)) {
        return string("i:") + info + "\r\nv:1.0";
    }
    return "";
}

int APIService::httpRequest(int method, const string& url, const list<int>& commands, const string& body, APICallback *callback) {
    if (url.empty()) {//url: server's url
        return 0;
    }
    int ret = 0;
    string headerRespond, bodyRespond, connInfo;
    pthread_mutex_lock(&mRequestMutex);
    {
        string header = generalHeader(commands);//i:{info}\r\nv:1.0
        if (0 == method) {//method=HTTP_GET
            ret = http_get(url, header, headerRespond, bodyRespond, connInfo);
        }
        else if (1 == method) {//HTTP_POST
            std::string enc = Utils::Hash::RC4Encrypt(body);
            ret = http_post(url, header, enc, headerRespond, bodyRespond, connInfo);
        }
        
        LOG_GLTX("body request is: (%d),%d,%s\n", ret, method, body.c_str());
        if (0 == ret) {
			ParserConnInfo(connInfo);
            if (!headerRespond.empty()) {
                parseHeader(headerRespond, callback);
                parseBody(bodyRespond, callback);
            }
            else {
                LOG_GLTX("Body respond with empty header :%s\n", bodyRespond.c_str());
                if (callback) {
                    callback->onRequestCallback(APICallback::HEAD, APICallback::HEAD_ERROR, "");
                }
            }
        }
    }
    pthread_mutex_unlock(&mRequestMutex);
    return ret;
}

bool APIService::parseHeader(const string& header, APICallback *callback) {
    int result = mHeaderModel->parseHeader(header);
    if (result > 0) {
        if (result&HttpRequestHeaderModel::SGUID_INIT) {
            LOG_GLTX("mTespan HttpRequestHeaderModel::SGUID_INIT");
            if (callback) {
                callback->onRequestCallback(APICallback::HEAD, APICallback::SGUID_INIT, mHeaderModel->getSGUID());
            }
        }

        if (result&HttpRequestHeaderModel::TESPAN_CHANGE) {
            LOG_GLTX("mTespan HttpRequestHeaderModel::TESPAN_CHANGE");
            if (callback) {
                callback->onRequestCallback(APICallback::HEAD, APICallback::TESPAN_CHANGE, mHeaderModel->getTespan());
            }
        }

        if (result&HttpRequestHeaderModel::IS_TOMORROW) {
            LOG_GLTX("mTespan HttpRequestHeaderModel::IS_TOMORROW");
            if (callback) {
                callback->onRequestCallback(APICallback::HEAD, APICallback::DATE_CHANGED, "");
            }
        }

        if (result&HttpRequestHeaderModel::EID_CHANGE) {
            LOG_GLTX("mTespan HttpRequestHeaderModel::EID_CHANGE");
            if (callback) {
                callback->onRequestCallback(APICallback::HEAD, APICallback::EID_CHANGED, mHeaderModel->GetEID());
            }
        }

        return true;
    }
    else if (result < 0){
        if (callback) {
            callback->onRequestCallback(APICallback::HEAD, APICallback::HEAD_ERROR, mHeaderModel->getTespan());
        }
    }

    return false;
}

bool APIService::parseBody(const string& body, APICallback *callback) {
    if (body.empty()) {
        LOG_GLTX("body respond is: \n\n");
        return true;
    } 

    std::string dec;
    const char *errorflag = "error";
    if (body.compare(0, 5, errorflag) == 0) {
        dec = body;
    }
    else {
        dec = Utils::Hash::RC4Decrypt(body);
    }
    LOG_GLTX("body respond is: %s\n\n", dec.c_str());

    if (NULL != callback && !dec.empty()) {
        return callback->onRequestCallback(APICallback::BODY, APICallback::BODY_ENTRY, dec) == 0;
    }
    return true;
}

int APIService::heartBeat() {
    list<int> commands;
    return httpRequest(HTTP_GET, mUrl, commands, "", mObserver);
}

int APIService::reportData(list<int> commands, const string& body) {
    return httpRequest(HTTP_POST, mUrl, commands, body, mObserver);
}

int APIService::reportData(Command command, const string& body) {
    list<int> commands;
    commands.push_back((int)command);

    LOG_GLTX("%.200s \n", body.c_str());

    return httpRequest(HTTP_POST, mUrl, commands, body, mObserver);
}

int APIService::GetServerTime() const {
    if (mHeaderModel) {
        return mHeaderModel->getServerTime();
    }
    return 0;
}

void APIService::ParserConnInfo(const std::string & conn)
{
	if(m_local_ip != conn)
	{
		LOG_GLTX(">> get conn info: %s", conn.c_str());
	}

	pthread_mutex_lock(&m_dlock);
	m_local_ip = conn;
	pthread_mutex_unlock(&m_dlock);
}

int  APIService::GetLocalIp(std::string & ip) 
{
	pthread_mutex_lock(&m_dlock);
	ip = m_local_ip;
	pthread_mutex_unlock(&m_dlock);
	return 0;
}
