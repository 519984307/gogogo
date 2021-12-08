//#include "updateapp.h"
//#include "upgrade.h"
//#include "commonapi.h"
//#include "commonlocker.h"
//#include "baseconfig.h"
//#include "models/policy/commandmodel.h"

//#include <string>
//#include <stdlib.h>
//#include <unistd.h>
//#include <stdio.h>

//UpdateApplication::UpdateApplication() {
//    mUpdateEngine = NULL;
//    mCommandModel = NULL;
//    mLocker = NULL;

//    mNeedRestart = false;
//}

//UpdateApplication::~UpdateApplication() {
//    if (mUpdateEngine != NULL) {
//        delete mUpdateEngine;
//        mUpdateEngine = NULL;
//    }
//    if (mCommandModel) {
//        delete mCommandModel;
//    }
//    if (mLocker) {
//        delete mLocker;
//    }
//}

//int UpdateApplication::Contruct(const std::string& installdir, const std::string& ownerpath) {
//    mInstallFolder = installdir;
//    mUpdateEngine = UpdateEngine::NEW(installdir, ownerpath);
//    if (NULL == mUpdateEngine) {
//        return -1;
//    }

//    return 0;
//}

//int UpdateApplication::AppUpdate(const std::string& version) {
//    if (NULL == mUpdateEngine) {
//        return -1;
//    }

//    mUpdateEngine->SetObserver(this);
//    return mUpdateEngine->Start(version);
//}

//int UpdateApplication::VirusUpdate(const std::string& version) {
////    if (NULL == mUpdateEngine) {
////        return -1;
////    }
////    mUpdateEngine->SetObserver(this);
////    return mUpdateEngine->StartUpdateVirus(version);
//}

//int UpdateApplication::StartUpdate(CommandModel *model) {
//    SetConnectParams(mUpdateEngine->GetUpdateConfig());
//    if (model) {
//        if (mCommandModel) {
//            delete mCommandModel;
//        }
//        mCommandModel = new CommandModel;
//        *mCommandModel = *model;
//    }

//    if (mLocker) {
//        delete mLocker;
//    }
//    mLocker = CondLocker::NEW();

//    return pthread_create(&mUpdateThread, NULL, UpdateThread, this);
//}

//int UpdateApplication::StopUpdate() {

//    return 0;
//}

//void* UpdateApplication::UpdateThread(void *args) {
//    UpdateApplication *app = (UpdateApplication*)args;
//    app->WorkFunc();
//    return NULL;
//}

//void UpdateApplication::WorkFunc() {
//    VirusUpdate(GetVirusVersion());
//    mLocker->Wait();

//    if (mUpdateEngine->GetUpdateConfig().mUpdateContent >= UPDATE_VIRUS_DB) {
//        AppUpdate(GetAppVersion());
//        mLocker->Wait();
//    }

//    UpdateCallback(UPDATE_EVENT_STATUE, UPDATE_COMMAND_FINISHED, mCommandModel);
//    if (mNeedRestart) {
//        std::string update = Utils::Path::JoinPath(mInstallFolder.c_str(),
//                                                   DEPLOYED_BIN_FOLDER,
//                                                   UPDATE_PROCESS_ENTRY, NULL);

//        pid_t pid;
//        if ((pid=fork()) ==0) {
//            if(execl(update.c_str(), UPDATE_PROCESS_ENTRY, "-r", "-v", mAppNewVersion.c_str(), NULL)<0) {
//                UpdateCallback(UpdateObserver::UPDATE_EVENT_REPLACE, UpdateObserver::UPDATE_ERROR, NULL);
//            }
//        }
//    }
//}

//int UpdateApplication::OnUpdateEvent(int event, int type, void *object) {
//    UpdateCallback((UpdateObserver::UpdateEvent)event, (UpdateObserver::UpdateType)type, object);

//    // for app update, when download finished, then replace file
//    if (event == UpdateObserver::UPDATE_EVENT_DOWNLOAD
//            && type == UpdateObserver::UPDATE_DOWNLOAD_SUCCESS) {
//        mAppNewVersion = *(std::string*)object;
//        mNeedRestart = true;
//    }

//    switch (type) {
//    case UPDATE_VIRUS_SUCCESS:
//    case UPDATE_NEWEST:
//    case UPDATE_DOWNLOAD_SUCCESS:
//    case UPDATE_ERROR: {
//        mLocker->Signal();
//        break;
//    }
//    default:
//        break;
//    };

//    return 0;
//}




