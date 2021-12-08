#ifndef CONTENTFACTORY_H
#define CONTENTFACTORY_H

#include <string>
#include <list>

#include "json/json.h"

class ContentFactory
{
public:
    static int make(const std::string& content, std::map<int, std::list<void*> >& datas);

private:
    int parseContent(const std::string& content, std::map<int, std::list<void*> >& datas);

    int parsePolicy(Json::Value value, std::list<void*>& data);
    int parseCommond(Json::Value value, std::list<void*>& data);
    int parseAuth(Json::Value value, std::list<void*>& data, int tespan=300);
};

#endif // CONTENTFACTORY_H
