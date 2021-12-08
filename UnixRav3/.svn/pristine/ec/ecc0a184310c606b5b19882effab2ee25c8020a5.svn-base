#include "simplejudger.h"
#include "multivirusengine.h"

int SimpleJudger::OnVirusEvent(EngineEvent /*event*/, EngineType type, void *data) {
    if (JUDGE_DANGE == type) {
        std::pair<std::string, std::string>* virus = (std::pair<std::string, std::string>*)data;
        mVirusName = virus->second;
    }
    return 0;
}
