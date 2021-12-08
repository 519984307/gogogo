#include "authorizationmodel.h"
#include "crypt/base64.h"
#include "communicator_global.h"

#include <string.h>


AuthorizationModel* AuthorizationModel::NEW(const std::string& guid, int auth, int tespan) {
    AuthorizationModel *self = new AuthorizationModel;
    if (self) {
        int err = self->Construct(guid, auth, tespan);
        if (err) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

AuthorizationModel::AuthorizationModel() :mAuthorization(0) {

}

int AuthorizationModel::Construct(const std::string& guid, int auth, int tespan) {
    mGUID = guid;
    mAuthorization = auth;
    mTespan = tespan;
    return 0;
}

bool AuthorizationModel::validate() {
    return true;
}
