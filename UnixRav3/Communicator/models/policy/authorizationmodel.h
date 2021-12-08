#ifndef AUTHORIZATIONMODEL_H
#define AUTHORIZATIONMODEL_H

#include <string>

#include "json/json.h"

class AuthorizationModel
{
public:
    static AuthorizationModel* NEW(const std::string& guid, int auth, int tespan);

private:
    AuthorizationModel();
    int Construct(const std::string& guid, int auth, int tespan);

public:
    bool validate();

public:
    std::string   mGUID;
    int           mAuthorization;
    int           mTespan;
};

#endif // AUTHORIZATIONMODEL_H
