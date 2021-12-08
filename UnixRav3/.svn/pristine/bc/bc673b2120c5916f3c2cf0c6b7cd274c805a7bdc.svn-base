#ifndef ENGINEOBSERVER_H
#define ENGINEOBSERVER_H

class EngineObserver {
public:
    typedef enum {
        COUNT_PATH,
        SCAN_FILE,
        VIRUS_JUDGE,
        VIRUS_UPDATE,
        REPORT,
        CLOUD_VIRUS,
    }EngineEvent;
    typedef enum {
        COUNT_FINISH,
        FILE_PATH,
        SCAN_START,
        SCAN_FINISH,
        JUDGE_START,
        JUDGE_END,
        JUDGE_OUT,
        JUDGE_DANGE,
        JUDGE_EXIT,
        JUDGE_CACHE,
        JUDGE_CLOUD,
        CACHE_FILE,
        ALL_FINISHED,
        RP_DOWNLOAD,
        RP_COMBINE,
        SCAN_SUMMARY,
        CLOUD_CHECK,
        CLOUD_POST,
        CLOUD_STATE,
    }EngineType;
    virtual int OnVirusEvent(EngineEvent event, EngineType type, void *data) = 0;
};

#endif // ENGINEOBSERVER_H
