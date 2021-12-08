#include "settingview.h"
#include "editview.h"
#include "textview.h"
#include "resources.h"
#include "optionview.h"

#include "Entry/commonentry.h"
#include "scanconfig.h"
#include "../Upgrade/Common/updateconfig.h"
#include "../BaseController/baseconfig.h"
#include "Entry/commonentry.h"


SettingView::SettingView(int x, int y, int width, int height):View(x,y,width,height)  {
    Layout();
}

void SettingView::Refresh() {
    for (int i=0; i<(int)mOptions.size(); ++i) {
        mOptions[i]->SetFocus(i==mFocusItem);
        mOptions[i]->Draw();
    }
}

int SettingView::ProcessEvent(int event) {
    switch (event) {
    case KEY_DOWN:
        if (++mFocusItem > (int)mOptions.size()) {
            mFocusItem = 0;
        }
        break;
    case KEY_UP:
        if (--mFocusItem < 0) {
            mFocusItem = mOptions.size()-1;
        }
        break;
    case 13:
    case 10:
        if (mFocusItem>=0 && mFocusItem<mOptions.size()) {
            return mOptions[mFocusItem]->ProcessEvent(event);
        }
    default:
        break;
    }
    Refresh();
    return 0;
}

DefenceSettingView::DefenceSettingView(ActiveDefenConfig& config, int x, int y, int width, int height) : SettingView(x,y,width,height), mConfig(config) {
    Layout(config);
}

void DefenceSettingView::Layout(ActiveDefenConfig& config) {
    std::vector<std::string> processOption;
    processOption.push_back(STRING_SET_CLOSE);
    processOption.push_back(STIRNG_SET_OPEN);
    mProcessSetView = new OptionView(STRING_PROCESS_DEFENCE_SET, processOption, (int&)config.mProcDefenState.mValue, 3, 3, 40, 1);
    mProcessSetView->SetMode(OptionView::RADIO);
    AddView(mProcessSetView);

    std::vector<std::string> fileOption;
    fileOption.push_back(STRING_SET_CLOSE);
    fileOption.push_back(STIRNG_SET_OPEN);
    mFileDefenceView = new OptionView(STRING_FILE_DEFENCE_SET, fileOption, (int&)config.mFileDefenState.mValue, 3, 4, 40, 1);
    mFileDefenceView->SetMode(OptionView::RADIO);
    AddView(mFileDefenceView);

    std::vector<std::string> pluginOption;
    pluginOption.push_back(STRING_SET_CLOSE);
    pluginOption.push_back(STIRNG_SET_OPEN);
    mPluginSetView = new OptionView(STRING_PLUGIN_DEFENCE_SET, pluginOption, (int&)config.mPluginDefenState.mValue, 3, 5, 40, 1);
    mPluginSetView->SetMode(OptionView::RADIO);
    AddView(mPluginSetView);

    std::vector<std::string> browserOption;
    browserOption.push_back(STRING_SET_CLOSE);
    browserOption.push_back(STIRNG_SET_OPEN);
    mBrowserBlockView = new OptionView(STRING_BROWSER_DEFENCE_SET, browserOption, (int&)config.mBrowserDefenState, 3,6, 40, 1);
    mBrowserBlockView->SetMode(OptionView::RADIO);
    AddView(mBrowserBlockView);

    std::vector<std::string> netfilterOption;
    netfilterOption.push_back(STRING_SET_CLOSE);
    netfilterOption.push_back(STIRNG_SET_OPEN);
    mNetfilteSetView = new OptionView(STRING_NETFILTER_DEFENCE_SET, netfilterOption, (int&)config.mNetBlockState.mValue, 3,7,40, 1);
    mNetfilteSetView->SetMode(OptionView::RADIO);
    AddView(mNetfilteSetView);

    mOptions.push_back(mProcessSetView);
    mOptions.push_back(mFileDefenceView);
    mOptions.push_back(mPluginSetView);
    mOptions.push_back(mBrowserBlockView);
    mOptions.push_back(mNetfilteSetView);

    mProcessSetView->SetFocus(true);
}


ScanSettingView::ScanSettingView(AntiVirusSetting& config, int x, int y, int width, int height) : SettingView(x,y,width,height), mConfig(config) {
    mVirusEngineType = 0;
    mScanType = 0;
    mFindVirusType = 0;
    mCleanFailType = 0;

    if (config.mVirusScan.mScanCommonVirus.mValue) {
        mVirusEngineType |= 1;
    }
    if (config.mVirusScan.mScanIntelligent.mValue) {
        mVirusEngineType |= 1<<1;
    }
    if (config.mVirusScan.mScanCompressFile.mValue) {
        mVirusEngineType |= 1<<2;
    }
    if (config.mVirusScan.mFileType.mValue == ALL_FILE) {
        mScanType = 0;
    }
    else if (config.mVirusScan.mFileType.mValue == EXE_DOC) {
        mScanType = 1;
    }

    Layout(config);
}

ScanSettingView::~ScanSettingView() {
    mConfig.mVirusScan.mScanCommonVirus.mValue = (mVirusEngineType & 0x1);
    mConfig.mVirusScan.mScanIntelligent.mValue = (mVirusEngineType & 0x2);
    mConfig.mVirusScan.mScanCompressFile.mValue = (mVirusEngineType & 0x4);

    if (mScanType == 0) {
        mConfig.mVirusScan.mFileType.mValue = ALL_FILE;
    }
    else if (mScanType == 1) {
        mConfig.mVirusScan.mFileType.mValue = EXE_DOC;
    }
}

void ScanSettingView::Layout(AntiVirusSetting& config) {
    std::vector<std::string> scanTypeOption;
    scanTypeOption.push_back(STRING_SCAN_ALL_FILE);
    scanTypeOption.push_back(STRING_SCAN_PRG_DOC);
    mScanTypeView = new OptionView(STRING_SETTING_FILE_TYPE, scanTypeOption, (int&)mScanType, 3, 3, 40, 1);
    mScanTypeView->SetMode(OptionView::RADIO);
    AddView(mScanTypeView);

    std::vector<std::string> scanEngineOption;
    scanEngineOption.push_back(STRING_SCAN_POPULATE);
    scanEngineOption.push_back(STRING_SCAN_AI);
    scanEngineOption.push_back(STRING_SCAN_ZIP);
    mScanEngineView = new OptionView(STRING_SCAN_ENGINE, scanEngineOption, (int&)mVirusEngineType, 3, 4, 40, 1);
    mScanEngineView->SetMode(OptionView::MULTI);
    AddView(mScanEngineView);

    std::vector<std::string> findVirusOption;
    findVirusOption.push_back(STRING_VIRUS_AUTO_DEAL);
    findVirusOption.push_back(STRING_VIRUS_MANUAL_DEAL);
    findVirusOption.push_back(STRING_VIRUS_IGNORE);
    mFindVirusView = new OptionView(STRING_VIRUS_DEAL_TYPE, findVirusOption, (int&)config.mVirusScan.mVirusDeal.mValue, 3, 5, 40, 1);
    mFindVirusView->SetMode(OptionView::RADIO);
    AddView(mFindVirusView);

    std::vector<std::string> cleanFaildOption;
    cleanFaildOption.push_back(STRING_VIRUS_AUTO_DEAL);
    cleanFaildOption.push_back(STRING_VIRUS_MANUAL_DEAL);
    cleanFaildOption.push_back(STRING_VIRUS_IGNORE);
    mCleanFailedView = new OptionView(STRING_CLEAN_FAILED, cleanFaildOption, (int&)config.mVirusScan.mCleanFailed.mValue, 3,6, 40, 1);
    mCleanFailedView->SetMode(OptionView::RADIO);
    AddView(mCleanFailedView);

    mOptions.push_back(mScanTypeView);
    mOptions.push_back(mScanEngineView);
    mOptions.push_back(mFindVirusView);
    mOptions.push_back(mCleanFailedView);

    mScanTypeView->SetFocus(true);
}


VirusSettingView::VirusSettingView(AntiVirusSetting& config,int x, int y, int width, int height)
    : SettingView(x,y,width,height), mConfig(config) {
    mVirusBackupType = 0;
    mVirusBackupType = config.mVirusBackup.mVirusBackup.mValue;
    Layout(config);
}

VirusSettingView::~VirusSettingView() {
    mConfig.mVirusBackup.mVirusBackup.mValue = mVirusBackupType;
}

void VirusSettingView::Layout(AntiVirusSetting& config) {
    std::vector<std::string> backupFile;

    backupFile.push_back(STRING_VIRUS_NO_BACKUP);
    backupFile.push_back(STRING_VIRUS_BACKUP_FILE);

    mBackupView = new OptionView(STRING_VIRUS_BACKUP, backupFile, (int&)mVirusBackupType, 3, 3, 40, 1);
    mBackupView->SetMode(OptionView::RADIO);
    AddView(mBackupView);

    std::vector<std::string> options;
    options.push_back(STRING_QUERY_ME);
    options.push_back(STRING_DELTE_FILE);
    options.push_back(STRING_VIRUS_IGNORE);
    mLargeFileView = new OptionView(STRING_VIRUS_LARGE_FILE,  options, (int&)config.mVirusBackup.mLargeFile.mValue, 3, 4, 40, 1);
    mLargeFileView->SetMode(OptionView::RADIO);
    AddView(mLargeFileView);

    std::vector<std::string> option2;
    option2.push_back(STRING_VIRUS_OVERRIDE_OLD);
    option2.push_back(STRING_VIRUS_SPACE_INCREAM);
    mSpaceLessView = new OptionView(STRING_VIRUS_SPACE_LESS, option2, (int&)config.mVirusBackup.mSpaceLess.mValue, 3, 5, 40, 1);
    mSpaceLessView->SetMode(OptionView::RADIO);
    AddView(mSpaceLessView);

    std::vector<std::string> option3;
    option3.push_back(STRING_QUERY_ME);
    option3.push_back(STRING_DELTE_FILE);
    option3.push_back(STRING_VIRUS_IGNORE);
    mBackupFailedView = new OptionView(STRING_BACKUP_FAILED, option3, (int&)config.mVirusBackup.mBackupFailed.mValue, 3, 6, 40, 1);
    mBackupFailedView->SetMode(OptionView::RADIO);
    AddView(mBackupFailedView);

    mOptions.push_back(mBackupView);
    mOptions.push_back(mLargeFileView);
    mOptions.push_back(mSpaceLessView);
    mOptions.push_back(mBackupFailedView);

    mBackupView->SetFocus(true);
}

AdminPasswdView::AdminPasswdView(int x, int y, int width, int height) : SettingView(x,y,width,height) {
    Layout();

    mHandler = Handler::NEW(this);
    mHandler->PostMessage(Message(1,1,1));
}

AdminPasswdView::~AdminPasswdView() {
    delete mHandler;
}

void AdminPasswdView::Layout() {
    mOldPasswd = new EditView(STRING_OLD_PASSWORD, mOldPwd, 20, 3, 3, 40, 1);
    AddView(mOldPasswd);

    mNewPasswd = new EditView(STRING_NEW_PASSWORD, mNewPwd, 20, 3, 4, 40, 1);
    AddView(mNewPasswd);

    mConfirmPasswd = new EditView(STRING_CONFIRM_PASSWORD, mConfirmPwd, 20, 3, 5, 40, 1);
    AddView(mConfirmPasswd);

    mOptions.push_back(mOldPasswd);
    mOptions.push_back(mNewPasswd);
    mOptions.push_back(mConfirmPasswd);
}

int AdminPasswdView::ProcessEvent(int event) {
    switch (event) {
    case KEY_DOWN:
    case KEY_UP:
        SettingView::ProcessEvent(event);
        if (mFocusItem < mOptions.size()) {
            ((EditView*)mOptions[mFocusItem])->SetEditable(true);
            ((EditView*)mOptions[mFocusItem])->Redraw();
            ((EditView*)mOptions[mFocusItem])->SetCursor();
        }
        return 1;
    default:
        break;
    }
    return mFocusItem < mOptions.size()
            ?((EditView*)mOptions[mFocusItem])->ProcessEvent(event):0;
}

void AdminPasswdView::HandleMessage(int event, int type, void *object) {
    mOldPasswd->SetCursor();
}

UpdateSetView::UpdateSetView(UpdateConfig& config, int x, int y, int width, int height) : SettingView(x, y, width, height){
    Layout(config);
}

void UpdateSetView::Layout(UpdateConfig& config) {
    std::vector<std::string> option;
    option.push_back(STRING_UPDATE_MODE_MANUAL);
    option.push_back(STRING_UPDATE_MODE_DAYS);
    option.push_back(STRING_UDPATE_MODE_WEEKS);
    option.push_back(STRING_UPDATE_MODE_INTERVAL);
    mUpdateType = new OptionView(STRING_UPDATE_MODE, option, (int&)config.mUpdateMode, 3, 3, 40, 1);
    mUpdateType->SetMode(OptionView::RADIO);
    AddView(mUpdateType);

    option.clear();
    option.push_back(STRING_DISABLE);
    option.push_back(STRING_AUTO);
    option.push_back(STRING_ON_TIME);
    mDelayStart = new OptionView(STRING_DELAY_START, option, (int&)config.mDelayStartAction, 3, 4, 40, 1);
    mDelayStart->SetMode(OptionView::RADIO);
    AddView(mDelayStart);

    option.clear();
    option.push_back(STRING_UPDATE_ALL);
    option.push_back(STRING_UPDATE_VIRUSDB);
    mUpdateContent = new OptionView(STRING_UPDATE_CONTENT, option, (int&)config.mUpdateContent, 3, 5, 40, 1);
    mUpdateContent->SetMode(OptionView::RADIO);
    AddView(mUpdateContent);

    option.clear();
    option.push_back(STRING_CONNECT_DIRECT);
    option.push_back(STRING_CONNECT_WITH_BROWERS);
    option.push_back(STRING_CONNECT_WITH_PROXY);
    mProxyView = new OptionView(STRING_PROXY_SET, option, (int&)config.mProxySetting, 3, 6, 40, 1);
    mProxyView->SetMode(OptionView::RADIO);
    AddView(mProxyView);

    option.clear();
    option.push_back(STRING_OFFICAL);
    option.push_back(STRING_OTHER);
    mUpdateSource = new OptionView(STRING_UPDATE_SOURCE, option, (int&)config.mUpdateSourceAction, 3, 7, 40, 1);
    mUpdateSource->SetMode(OptionView::RADIO);
    AddView(mUpdateSource);

    mOptions.push_back(mUpdateType);
    mOptions.push_back(mDelayStart);
    mOptions.push_back(mUpdateContent);
    mOptions.push_back(mProxyView);
    mOptions.push_back(mUpdateSource);

    mUpdateType->SetFocus(true);
}

SingleInputView::SingleInputView(std::string& content, int x, int y, int width, int height) : SettingView(x,y,width,height), mContent(content) {
    Layout();

    mHandler = Handler::NEW(this);
    mHandler->PostMessage(Message(1,1,1));
}

SingleInputView::~SingleInputView() {
    delete mHandler;
}

void SingleInputView::Layout() {
    mInputView = new EditView(STRING_ADD_WHITE_PROMPT, mContent, 30, 3, 3, 40, 1);
    AddView(mInputView);

    mOptions.push_back(mInputView);
}

int SingleInputView::ProcessEvent(int event) {
    switch (event) {
    case KEY_DOWN:
    case KEY_UP:
        mFocusItem < mOptions.size()
                    ?((EditView*)mOptions[mFocusItem])->ProcessEvent(event):0;
        SettingView::ProcessEvent(event);
        if (mFocusItem < mOptions.size()) {
            ((EditView*)mOptions[mFocusItem])->SetEditable(true);
            ((EditView*)mOptions[mFocusItem])->Redraw();
            ((EditView*)mOptions[mFocusItem])->SetCursor();
        }
        return 1;
    default:
        break;
    }
    return mFocusItem < mOptions.size()
            ?((EditView*)mOptions[mFocusItem])->ProcessEvent(event):0;
}

void SingleInputView::HandleMessage(int event, int type, void *object) {
    mInputView->SetCursor();
}

ScheduleSetView::ScheduleSetView(int& scanType, int& scanPos,  int x, int y, int width, int height)
    : SettingView(x, y, width, height), mScanType(scanType), mScanPos(scanPos) {
    Layout(scanType, scanPos);
}

void ScheduleSetView::Layout(int& scanType, int& scanPos) {
    std::vector<std::string> option;
    option.push_back(STRING_SCHEDULE_STARTUP);
    option.push_back(STRING_SCHEDULE_DAYS);
    option.push_back(STRING_SCHEDULE_WEEKS);
    option.push_back(STRING_SCHEDULE_MONTHS);
    mDateView = new OptionView(STRING_SCHEDULE_TIME, option, mScanType, 3, 3, 40, 1);
    mDateView->SetMode(OptionView::RADIO);
    AddView(mDateView);

    option.clear();
    option.push_back(STRING_OPERATE_QUICK_SCAN);
    option.push_back(STRING_OPERATE_ALL_SCAN);
    mTypeView = new OptionView(STRING_SCHEDULE_TYPE, option, mScanPos, 3, 4, 40, 1);
    mTypeView->SetMode(OptionView::RADIO);
    AddView(mTypeView);

    mOptions.push_back(mDateView);
    mOptions.push_back(mTypeView);

    mDateView->SetFocus(true);
}


//------------------------------------------------------
UpdateActiveView::UpdateActiveView(int x, int y, int width, int height) : SettingView(x,y,width,height) {
    Layout();

    mHandler = Handler::NEW(this);
    mHandler->PostMessage(Message(1,1,1));
}

UpdateActiveView::~UpdateActiveView() {
    delete mHandler;
}

void UpdateActiveView::Layout() {
    mSerialView  = new EditView(STRING_EDIT_SERIAL, mSerial, 20, 3, 3, 20, 1);
    mLicPathView = new EditView(STRING_EDIT_LICENSE_PATH, mLicPath, 23, 3, 4, 23, 2);

    AddView(mSerialView);
    AddView(mLicPathView);

    mOptions.push_back(mSerialView);
    mOptions.push_back(mLicPathView);
}

int UpdateActiveView::ProcessEvent(int event) {
    switch (event) {
    case KEY_DOWN:
    case KEY_UP:
        mFocusItem < mOptions.size()
                    ?((EditView*)mOptions[mFocusItem])->ProcessEvent(event):0;
        SettingView::ProcessEvent(event);
        if (mFocusItem < mOptions.size()) {
            ((EditView*)mOptions[mFocusItem])->SetEditable(true);
            ((EditView*)mOptions[mFocusItem])->Redraw();
            ((EditView*)mOptions[mFocusItem])->SetCursor();
        }
        return 1;
    default:
        break;
    }
    return mFocusItem < mOptions.size()
            ?((EditView*)mOptions[mFocusItem])->ProcessEvent(event):0;
}

void UpdateActiveView::HandleMessage(int event, int type, void *object) {
    mSerialView->SetCursor();
}
