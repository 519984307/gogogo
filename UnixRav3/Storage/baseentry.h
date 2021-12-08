#ifndef BASEENTRY_H
#define BASEENTRY_H

#include <string>

class BaseEntry
{
public:
    long mID;

protected:
    virtual std::string CreateTable() const = 0;


};

#endif // BASEENTRY_H
