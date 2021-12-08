#ifndef MESSAGEPARTBODY_H
#define MESSAGEPARTBODY_H

#include <map>
#include <list>
#include <string>

#include "modeldefine.h"
#include "models/contentmodel.h"
#include "json/json.h"

#define MODEL_COMMAND_TAG   "cmsgtype"
#define MODEL_CONTENT_TAG   "content"

using namespace std;

template<class T>
class MessagePartBody
{
public:
    MessagePartBody(Command command, ContentModel<T> *child) {
        mCommandType = command;
        mContentMap.insert(pair<string,ContentModel<T>*>(child->getTypeFlag(), child));
    }

    MessagePartBody(Json::Value json) {

    }


    Json::Value toJSON() {
        Json::Value json;
        json[MODEL_COMMAND_TAG] = Json::Value(mCommandType);

        Json::Value contentJSON;
        typename map<string, ContentModel<T>* >::iterator iter;
        for (iter=mContentMap.begin(); iter!=mContentMap.end(); ++iter) {
            ContentModel<T> *content = iter->second;
            contentJSON[iter->first] = content->toJSON();
        }
        json[MODEL_CONTENT_TAG] = contentJSON;
        return json;
    }



public:
    Command mCommandType;
    map<string, ContentModel<T>*>   mContentMap;
};

class MessagePartBundle {
public:
    MessagePartBundle(Command command, ContentBundle *bundle) {
        mCommandType = command;
        mContent  = bundle;
    }

    Json::Value toJSON() {
        Json::Value json;
        json[MODEL_COMMAND_TAG] = Json::Value(mCommandType);
        json[MODEL_CONTENT_TAG] = mContent->toJSON();
        return json;
    }

public:
    Command mCommandType;
    ContentBundle*   mContent;
};

//template<class T, class K>
//class MessagePartBundle {
//public:
//    MessagePartBundle(Command command, ContentModel<T> *son, ContentModel<K> *daughter) {
//        mCommandType = command;
//        mSonContent = son;
//        mDaughterContent = daughter;
//    }

//    Json::Value toJSON() {
//        Json::Value json;
//        json[MODEL_COMMAND_TAG] = Json::Value(mCommandType);

//        Json::Value contentJSON;
//        if(NULL != mSonContent) {
//            contentJSON[mSonContent->getTypeFlag()] = mSonContent->toJSON();
//        }
//        if (NULL != mDaughterContent) {
//            contentJSON[mDaughterContent->getTypeFlag()] = mDaughterContent->toJSON();
//        }

//        json[MODEL_CONTENT_TAG] = contentJSON;
//        return json;
//    }

//public:
//    Command mCommandType;
//    ContentModel<T> *mSonContent;
//    ContentModel<K> *mDaughterContent;
//};

#endif // MESSAGEPARTBODY_H
