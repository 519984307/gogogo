#ifndef SCANENGINE_GLOBAL_H
#define SCANENGINE_GLOBAL_H

#include <string.h>

typedef enum {
    RAVUXENG_VIRUS_IGNORE = 0x0,
    RAVUXENG_VIRUS_CLEAN,
    RAVUXENG_VIRUS_DELETE,
    RAVUXENG_VIRUS_ASKUSER,
    RAVUXENG_VIRUS_STOPSCAN
}TVirusDealType;

typedef enum {
    RAVUXENG_KR_IGNORE = 0x0,
    RAVUXENG_KR_SKILL,
    RAVUXENG_KR_SDELETE,
    RAVUXENG_KR_EKILL,
    RAVUXENG_KR_EDELETE,
    RAVUXENG_KR_ECOMPOUND,
    RAVUXENG_KR_EPOST_TREAT,
    RAVUXENG_KR_EUNKONW
}T;

typedef enum {
    RAVUXENG_ASK_FOUNDVIRUS = 0x0,
    RAVUXENG_ASK_CLEANFAILED,
    RAVUXENG_ASK_BACKUPFAILED
}TVirusAskType;

class ScanEntry {
public:
    enum EntryType {
        ENTRY_EMPTY   = -1,
        ENTRY_PREPARED= 0,
        CLOUD_BLANK   = 1,
        CLOUD_WHITE,
        CLOUD_UNKNOWN,
        CLOUD_GRAY    = 9
    };
public:
    char *file;
    char *virus;
    char hash[32];
    /** cloud state */
    int  state;

private:
    int size;

public:
    inline void reset() {
        memset(file, 0, size);
        memset(hash, 0, 32);
        memset(virus, 0, size);
        state = ENTRY_EMPTY;
    }

    ScanEntry(int size) {
        this->size = size;
        file = new char[size];
        virus = new char[size];

        reset();
    }

    ~ScanEntry() {
        delete[] file;
        delete[] virus;
    }
    inline int limit() const {
        return size-1;
    }
    inline void consume() {
        state = ENTRY_EMPTY;
        file[0] = '\0';
        hash[0] = '\0';
    }
    inline bool valid() const {
        return (state==ENTRY_EMPTY)&&(file[0]=='\0');
    }
};

#endif // SCANENGINE_GLOBAL_H
