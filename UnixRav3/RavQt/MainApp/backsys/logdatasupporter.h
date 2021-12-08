#ifndef LOGDATASUPPORTER_H
#define LOGDATASUPPORTER_H

#include "application.h"
#include "resources.h"
#include "dialog.h"
#include "settingview.h"
#include "commonapi.h"
#include "Entry/logevententry.h"
#include "Entry/virusinfoentry.h"
#include "Entry/quarantinelog.h"
#include "Entry/whitelistentry.h"

#include <string>
#include <string.h>

class LogEventEntry;
class ScanLogDataSupporter : public LogDataBridge<LogEventEntry, std::string>
{
public:
    ScanLogDataSupporter(Application *app) : LogDataBridge<LogEventEntry, std::string>(app) {
        LoadData();
    }

public:
    void Content(int pos, std::string& data) {
        char temp[70];
        memset(temp, ' ', 69);
        temp[69] = '\0';

        char abuf[32] = {0};
        int index = 0;
        std::list<LogEventEntry>::iterator iter;
        for (iter=mDataArray.begin(); iter!=mDataArray.end(); ++iter, ++index) {
            if (index == pos) {
                sprintf(abuf, "%d", iter->mID);
                memcpy(temp+0, abuf, strlen(abuf)>3?3:strlen(abuf));
                memcpy(temp+3, Utils::Date::FormatTimeStamp(iter->mStartTime).c_str(), 19);

                if (iter->GetScanType() == LogEventEntry::REMOTE_SCAN) {
                    memcpy(temp+25, REMOTE_COMMAND_NAME, strlen(REMOTE_COMMAND_NAME)>6?6:strlen(REMOTE_COMMAND_NAME));
                }
                else  if (iter->GetScanType() == LogEventEntry::MANUAL_SCAN) {
                    memcpy(temp+25, USER_MANUALLY_NAME, strlen(REMOTE_COMMAND_NAME)>6?6:strlen(REMOTE_COMMAND_NAME));
                }
                else if (iter->GetScanType() == LogEventEntry::TIMER_SCAN) {
                    memcpy(temp+25, SCHEDULE_SCAN_NAME, strlen(REMOTE_COMMAND_NAME)>6?6:strlen(REMOTE_COMMAND_NAME));
                }

                switch (iter->GetDiskType()) {
                case LogEventEntry::ALL_DISK:
                    memcpy(temp+34, FULL_SCAN_TYPE, strlen(FULL_SCAN_TYPE)>8?8:strlen(FULL_SCAN_TYPE));
                    break;
                case LogEventEntry::QUICK_DISK:
                    memcpy(temp+34, QUICK_SCAN_TYPE, strlen(QUICK_SCAN_TYPE)>8?8:strlen(QUICK_SCAN_TYPE));
                    break;
                case LogEventEntry::USER_DISK:
                    memcpy(temp+34, CUSTOM_SCAN_TYPE, strlen(CUSTOM_SCAN_TYPE)>8?8:strlen(CUSTOM_SCAN_TYPE));
                    break;
                default:
                    break;
                }

                sprintf(abuf, "%d", iter->mScanCount);
                memcpy(temp+50, abuf, strlen(abuf)>3?3:strlen(abuf));
                sprintf(abuf, "%d", iter->mVirusCount);
                memcpy(temp+57, abuf, strlen(abuf)>3?3:strlen(abuf));
                sprintf(abuf, "%d", iter->mKilledCount);
                memcpy(temp+64, abuf, strlen(abuf)>3?3:strlen(abuf));

                data = temp;
            }
        }
    }

    void LoadData(int page=0) {
        mApplication->GetScanLogs(mDataArray);
    }
};

class VirusLogDataSupporter : public LogDataBridge<VirusInfoEntry, std::string>
{
public:
    VirusLogDataSupporter(Application *app) : LogDataBridge<VirusInfoEntry, std::string>(app) {
        LoadData();
    }

public:
    void Content(int pos, std::string& data) {
        char temp[70];
        memset(temp, ' ', 69);
        temp[69] = '\0';

        char abuf[32] = {0};
        int index = 0;
        std::list<VirusInfoEntry>::iterator iter;
        for (iter=mDataArray.begin(); iter!=mDataArray.end(); ++iter, ++index) {
            if (index == pos) {
                sprintf(abuf, "%d", iter->mID);
                memcpy(temp+0, abuf, 1);
                memcpy(temp+3, Utils::Date::FormatTimeStamp(iter->mTime).c_str(), 19);
                memcpy(temp+23, iter->mOriginPath.c_str(), iter->mOriginPath.size()>17?17:strlen(abuf));
                memcpy(temp+41, iter->mVirusName.c_str(), iter->mVirusName.size()>17?17:strlen(abuf));

                switch (iter->mDealState) {
                case 0:
                    memcpy(temp+59, VIRUS_DEAL_DEFAULT, strlen(VIRUS_DEAL_DEFAULT)>6?6:strlen(VIRUS_DEAL_DEFAULT));
                    break;
                case 1:
                    memcpy(temp+59, VIRUS_DEAL_IGNORE, strlen(VIRUS_DEAL_IGNORE)>6?6:strlen(VIRUS_DEAL_IGNORE));
                    break;
                case 2:
                    memcpy(temp+59, VIRUS_DEAL_CLEAN, strlen(VIRUS_DEAL_CLEAN)>6?6:strlen(VIRUS_DEAL_CLEAN));
                    break;
                case 3:
                    memcpy(temp+59, VIRUS_DEAL_REMOVE, strlen(VIRUS_DEAL_REMOVE)>6?6:strlen(VIRUS_DEAL_REMOVE));
                    break;
                default:
                    break;
                }

                sprintf(abuf, "%d", iter->mScanLogID);
                memcpy(temp+66, abuf, strlen(abuf)>3?3:strlen(abuf));

                data = temp;
            }
        }
        data = temp;
    }

    void LoadData(int page=0) {
        mApplication->GetVirusLogs(mDataArray);
    }
};

class BaseLogDataSupporter : public LogDataBridge<LogEventEntry, std::string>
{
public:
    BaseLogDataSupporter(Application *app) : LogDataBridge<LogEventEntry, std::string>(app) {
        LoadData();
    }

public:
    void Content(int pos, std::string& data) {
        char temp[70];
        memset(temp, ' ', 69);
        temp[69] = '\0';

        memcpy(temp+0, "0", 1);
        memcpy(temp+3, "2008-10-01 05:04:00", 19);
        memcpy(temp+23, "SCAN   ", 6);
        memcpy(temp+32, "REMOTE SCAN DATA", 13);
        memcpy(temp+46, "CENTER REMOTE", 13);
        memcpy(temp+60, "FINISHED", 8);

        data = temp;
    }

    void LoadData(int page=0) {

    }
};

class QuarantineDataSupporter : public LogDataBridge<QuarantineLog, std::string>
{
public:
    QuarantineDataSupporter(Application *app) : LogDataBridge<QuarantineLog, std::string>(app) {
        LoadData();
    }

public:
    void Content(int pos, std::string& data) {
        char temp[70];
        memset(temp, ' ', 69);
        temp[69] = '\0';

        char abuf[32] = {0};
        int index = 0;
        std::list<QuarantineLog>::iterator iter;
        for (iter=mDataArray.begin(); iter!=mDataArray.end(); ++iter, ++index) {
            if (index == pos) {
                sprintf(abuf, "%d", iter->mID);
                memcpy(temp+0, abuf, 1);
                memcpy(temp+3, Utils::Date::FormatTimeStamp(iter->mDate).c_str(), 19);

                memcpy(temp+23, iter->mSource.c_str(), iter->mSource.size()>28?28:iter->mSource.size());
                memcpy(temp+53, iter->mTarget.c_str(), iter->mTarget.size()>13?13:iter->mTarget.size());

                data = temp;
            }
        }

        data = temp;
    }

    void LoadData(int page=0) {
        mApplication->GetQuaLogs(mDataArray);
    }
};

class WhiteListDataSupporter : public LogDataBridge<WhiteListEntry, std::string>
{
public:
    WhiteListDataSupporter(Application *app) : LogDataBridge<WhiteListEntry, std::string>(app) {
        LoadData();
    }

public:
    void Content(int pos, std::string& data) {
        WhiteListEntry entry;
        if (GetData(pos, entry)) {
            char temp[70];
            memset(temp, ' ', 69);
            temp[69] = '\0';

            char seq[12] = {0};
            sprintf(seq, "%ld", entry.mID);
            memcpy(temp+0, seq, 1);
            memcpy(temp+10, entry.mWhitePath.c_str(), 40);

            data = temp;
        }
    }

    int Count() {
        return mDataArray.size();
    }

    void LoadData(int page=0) {
        mDataArray.clear();

        mApplication->GetWhiteList(mDataArray);

        WhiteListEntry entry;
        entry.mWhitePath = STRING_ADD_WHITE_LIST;
        mDataArray.push_back(entry);
    }

    void SelectData(int pos) {
        if (pos < (int)mDataArray.size()-1) {
            return;
        }

        std::string whitedir;
        SingleInputView *view = new SingleInputView(whitedir, 5,5,40,4);
        Dialog::Builder build;
        Dialog* dialog = build.SetTitle(STRING_ADD_WHITE_LIST)
                         .SetLayout(view)
                         .SetPositiveButton(STRING_BUTTON_YES)
                         .SetNegativeButton(STRING_BUTTON_NO)
                         .Build();
        if (dialog->DoModel() == Dialog::DIALOG_OK) {
            if (view->GetContent().empty()) {
                return;
            }

            WhiteListEntry entry;
            entry.mWhitePath = view->GetContent();
            if (0 == mApplication->AddWhiteList(entry)) {
                mDataArray.push_front(entry);
            }
        }
        delete dialog;
    }

    void DeleteData(int pos) {
        if (pos == (int)mDataArray.size() - 1) {
            return;
        }
        return LogDataBridge::DeleteData(pos);
    }
};

#endif // LOGDATASUPPORTER_H
