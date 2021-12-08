#ifndef MESSAGEPACKAGE_H
#define MESSAGEPACKAGE_H

#include "messagepartbody.h"

#define MODEL_MESSAGE_TAG   "msg"

using namespace std;

template<class T>
class MessagePackage
{
public:
    MessagePackage(MessagePartBody<T> *part) : mPart(part) {
        mPart = part;
    }

    Json::Value toJSON() {
        Json::Value part;
        if (NULL != mPart) {
            part.append(mPart->toJSON());
        }

        Json::Value msg;
        msg[MODEL_MESSAGE_TAG] = part;
        return msg;
    }


    std::string serialize() {
        Json::FastWriter writer;
        return writer.write(toJSON());
    }

public:
    MessagePartBody<T> *mPart;
};

class MessageBundle {
public:
    MessageBundle(MessagePartBundle *bundle) {
        mPartList.push_back(bundle);
    }

    Json::Value toJSON() {
        Json::Value value;

        std::list<MessagePartBundle*>::iterator iter;
        for (iter=mPartList.begin(); iter!=mPartList.end(); ++iter) {
            value.append((*iter)->toJSON());
        }

        Json::Value json;
        json[MODEL_MESSAGE_TAG] = value;
        return json;
    }

    std::string serialize() {
        Json::FastWriter writer;
        return writer.write(toJSON());
    }

    void operator += (MessageBundle bundle) {
        mPartList.merge(bundle.mPartList);
    }
public:
    list<MessagePartBundle*>   mPartList;
};

#endif // MESSAGEPACKAGE_H
