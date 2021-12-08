#ifndef OBSERVER_H
#define OBSERVER_H

#include <stdio.h>
#include <list>
#include <string>

class DefenObserver {
public:
    typedef enum {
        DEFEN_PROC_START,
        DEFEN_FILE_WRITE,
        DEFEN_FILE_ACCESS,
    }DEFEN_TYPE;
public:
    virtual int onDefenEvent(DEFEN_TYPE event, const char *file, void *data=NULL) = 0;

    virtual std::list<std::string> &GetFolderMonitorList() = 0;

};

#endif // OBSERVER_H
