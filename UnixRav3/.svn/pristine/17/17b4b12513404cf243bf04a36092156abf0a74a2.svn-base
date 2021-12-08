#include "engineobserver.h"

#include <string>

class SimpleJudger : public EngineObserver {
public:
    virtual ~SimpleJudger() {}

public:
    int OnVirusEvent(EngineEvent event, EngineType type, void *data);

public:
    std::string mVirusName;
};
