#include "contentfactory.h"
#include "modeldefine.h"
#include "commonapi.h"
#include "models/policy/policymodel.h"
#include "models/policy/commandmodel.h"
#include "models/policy/authorizationmodel.h"

#include <stdlib.h>
#include <unistd.h>

int ContentFactory::make(const std::string& content, std::map<int, std::list<void*> >& datas) {
    if (content.empty()) {
        return -1;
    }

    ContentFactory *self = new ContentFactory;
    if (self) {
        int err = 0;
        try {
            err = self->parseContent(content, datas);
        }
        catch(...) {
            err = -1;
        }

        delete self;
        return err;
    }
    return -1;
}

int ContentFactory::parseContent(const std::string& content, std::map<int, std::list<void*> >& datas) {
    Json::Reader reader;
    Json::Value json;
    if (!reader.parse(content, json)) {
        return -1;
    }

    if (json.isMember("msg")) {
        Json::Value contentArray = json["msg"];
        for (uint i=0; i<contentArray.size();i++) {
            Json::Value content = contentArray[i];
            int type = Utils::JSON::GetJsonInt(content, "cmsgtype");
            Json::Value msgcontent = content["content"];
            if (msgcontent.isInt()) {
                int result = msgcontent.asInt();
                if (result == 0) {
                    continue;
                }
            }
            else {
                std::list<void*> result;
                switch (type) {
                case COMMAND_POLICY: {
                    if (parsePolicy(msgcontent, result)) {
                        continue;
                    }
                    break;
                }
                case COMMAND_COMMAND: {
                    if (parseCommond(msgcontent, result)) {
                        continue;
                    }
                    break;
                }
                case COMMAND_REQUEST_AUTH: {
                    int tespan = Utils::JSON::GetJsonInt(content, "tespan");
                    if (parseAuth(msgcontent, result, tespan))
                    break;
                }
                default:
                    break;
                }

                if (result.size() > 0) {
                    datas.insert(std::pair<int, std::list<void*> >(type, result));
                }
            }
        }
    }
    return 0;
}

int ContentFactory::parsePolicy(Json::Value value, std::list<void*>& data) {
    if (value.isMember("policycontent")) {
        Json::Value policyArray = value["policycontent"];
        for (uint i=0; i<policyArray.size(); i++) {
            PolicyModel *model = PolicyModel::NEW(policyArray[i], false);
            if (model) {
                data.push_back(model);
            }
        }
    }
    return 0;
}

int ContentFactory::parseCommond(Json::Value value, std::list<void*>& data) {
    if (value.isMember("cmdcontent")) {
        Json::Value commandArray = value["cmdcontent"];
        for (uint i=0; i<commandArray.size(); i++) {
            CommandModel *model = CommandModel::NEW(commandArray[i], false);
            if (model) {
                data.push_back(model);
            }
        }
    }
    return 0;
}

int ContentFactory::parseAuth(Json::Value value, std::list<void*>& data, int tespan) {
    Json::Value::Members member;
    std::vector<std::string>::iterator iter;

	if(value.isObject()) member = value.getMemberNames();
    for (iter=member.begin(); iter!=member.end(); ++iter) {
        AuthorizationModel *model = AuthorizationModel::NEW(*iter, value[*iter].asInt(), tespan);
        if (model) {
            data.push_back(model);
        }
    }
    return 0;
}


