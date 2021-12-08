#ifndef CONTENTMODEL_H
#define CONTENTMODEL_H

#include <list>

#include "modeldefine.h"
#include "json/json.h"

#include "models/log/logdatavirus.h"

#include <stdio.h>

template<class T>
class ContentModel
{
public:
    ContentModel(ContentType type, std::list<T*> children) {
        mType = type;
        mSingleFlag = false;
        mListContent.insert(std::pair<std::string, std::list<T*> >(getTypeFlag(), children));
    }

    ContentModel(ContentType type, T* child) {
        mType = type;
        mSingleFlag = true;
        mSingleMap.insert(std::pair<std::string, T*>(getTypeFlag(), child));
    }

    Json::Value toJSON() {
        Json::Value value;

        typename std::map<std::string, std::list<T*> >::iterator iter;
        for (iter=mListContent.begin(); iter!=mListContent.end(); ++iter) {
            typename std::list<T*>::iterator sp;
            for (sp=iter->second.begin(); sp!=iter->second.end(); ++sp) {
                Json::Value son = (*sp)->toJSON();
                value.append(son);
            }
        }

        typename std::map<std::string, T*>::iterator sp;
        for (sp=mSingleMap.begin(); sp!=mSingleMap.end(); ++sp) {
            Json::Value son = sp->second->toJSON();
            value = son;
        }
        return value;
    }

    bool validate() {
        return true;
    }

    std::string getTypeFlag() {
        return ENUM_VALUE(ContentTypeDetail, mType);
    }

    void operator += (const ContentModel<T>& content) {
        if (mType != content.mType) {
            return;
        }

        do {
            typename std::map<std::string, std::list<T*> >::iterator iter;
            for (iter=content.mListContent.begin(); iter!=content.mListContent.end(); ++iter) {
                typename std::map<std::string, std::list<T*> >::iterator fp;
                fp = mListContent.find(iter->first);
                if (fp != mListContent.end()) {
                    // find it, combine it
                    fp->second.merge(iter->second);
                }
                else {
                    mListContent.insert(std::pair<std::string, std::list<T*> >(iter->first, iter->second));
                }
            }
        }while(0);

        do {
            typename std::map<std::string, T*>::iterator iter;
            for (iter=content.mSingleMap.begin(); iter!=content.mSingleMap.end(); ++iter) {
                typename std::map<std::string, T*>::iterator fp;
                fp = mSingleMap.find(iter->first);
                if (fp == mListContent.end()) {
                    mSingleMap.insert(std::pair<std::string, T*>(iter->first, iter->second));
                }
            }
        }while(0);
    }

public:
    ContentType mType;

    /**
     * 单个的标志
     * 以这个标志确定在单个数据的时据不显示数组
     */
    bool mSingleFlag;
    std::map<std::string, T*> mSingleMap;
    std::map<std::string, std::list<T*> > mListContent;
};

class ContentBundle {
public:
    ContentBundle(ContentType type, Json::Value value, bool single=false) {
        mListContent.insert(std::pair<ContentType, Json::Value>(type, value));
        if (single) {
            mListFlag.insert(std::pair<ContentType, bool>(type, true));
        }
    }

    void operator += (ContentBundle bundle) {
        std::map<ContentType, Json::Value>::iterator iter;
        for (iter=bundle.mListContent.begin(); iter!=bundle.mListContent.end(); ++iter) {
            mListContent.insert(std::pair<ContentType, Json::Value>(iter->first, iter->second));
        }

        std::map<ContentType, bool>::iterator sp;
        for (sp=bundle.mListFlag.begin(); sp!=bundle.mListFlag.end(); ++sp) {
            mListFlag.insert(std::pair<ContentType, bool>(sp->first, sp->second));
        }
    }

public:
    std::string getTypeFlag(int type) {
        return ENUM_VALUE(ContentTypeDetail, type);
    }

    Json::Value toJSON() {
        Json::Value value;
        std::map<ContentType, Json::Value>::iterator iter;
        for (iter=mListContent.begin(); iter!=mListContent.end(); ++iter) {
            std::map<ContentType, bool>::iterator sp = mListFlag.find(iter->first);
             value[getTypeFlag(iter->first)] = iter->second;

//            if (sp != mListFlag.end()) {

//            }
//            else {
//                value[getTypeFlag(iter->first)].append(iter->second);
//            }
        }

        return value;
    }
public:
    std::map<ContentType, Json::Value> mListContent;
    std::map<ContentType, bool> mListFlag;
};


#endif // CONTENTMODEL_H
