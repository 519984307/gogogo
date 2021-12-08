#ifndef _H_TAILREADER_H_
#define _H_TAILREADER_H_

#include <string>

using namespace std;

class TailReader{
public:
    TailReader();
    ~TailReader();

    int ParseFile();

protected:
    int ParseJson(const string buffer);
    string GetOwnerPathName();

public:
    bool have_tail_;
    int  json_length_;
    string json_string_;

    int type_;
    string center_addr_;
};



#endif
