#ifndef UPDATEOBSERVER_H
#define UPDATEOBSERVER_H

class UpdateObserver {
public:
    typedef enum _UpdateEvnet {
        UPDATE_EVENT_APP,
        UPDATE_EVENT_VIRUS,
    }UpdateEvent;

    typedef enum _UpdateType {
        UPDATE_ERROR    = -1,
        UDDATE_GET_NEW_VERSION,
        UPDATE_GET_FILE_LIST,
        UPDATE_DOWNLOAD_FILE,
        UPDATE_DOWNLOAD_SUCCESS,
        UPDATA_VIRUS_RP,
        UPDATE_VIRUS_COMBINE,
        UPDATE_VIRUS_SUCCESS,
        UDPATE_VIRUS_FAILED,
        UPDATE_NEWEST,
        UPDATE_COMMAND_FINISHED,
        UPDATE_OFFLINE_VLIB,
    }UpdateType;


    /**
     * @brief OnUpdateEvent
     * @param event
     * @param type
     *          UPDATE_GET_FILE_LIST: object is int, will download count
     *          UPDATE_DOWNLOAD_FILE: object is char[], download file name
     *          UDDATE_GET_NEW_VERSION: object is string, version name
     *          UPDATE_DOWNLOAD_SUCCESS: object is string, current version
     * @param object
     * @return
     */
    virtual int OnUpdateEvent(int event, int type, void *object) = 0;
};

#endif // UPDATEOBSERVER_H
