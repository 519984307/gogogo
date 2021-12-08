#ifndef SETTINGVIEW_H
#define SETTINGVIEW_H

#include "view.h"
#include "handler.h"

#include <string>
#include <vector>

class EditView;
class TextView;
class OptionView;
class AntiVirusSetting;
class UpdateConfig;
class ActiveDefenConfig;
class SettingView : public View {
public:
    SettingView(int x, int y, int width, int height);

protected:
    virtual void Layout() {}
    int ProcessEvent(int event);

    void Refresh();

protected:
    std::vector<View*> mOptions;
    int mFocusItem;
};

class DefenceSettingView : public SettingView {
public:
    DefenceSettingView(ActiveDefenConfig& config, int x, int y, int width, int height);
    ~DefenceSettingView() {}

protected:
    void Layout(ActiveDefenConfig& config);

public:
    OptionView  *mProcessSetView;
    OptionView  *mNetfilteSetView;
    OptionView  *mBrowserBlockView;
    OptionView  *mPluginSetView;
    OptionView  *mFileDefenceView;

private:
    ActiveDefenConfig& mConfig;
};

class ScanSettingView : public SettingView
{
public:
    ScanSettingView(AntiVirusSetting& config, int x, int y, int width, int height);
    ~ScanSettingView();

protected:
    void Layout(AntiVirusSetting& config);

public:
    OptionView  *mScanTypeView;
    OptionView  *mScanEngineView;
    OptionView  *mFindVirusView;
    OptionView  *mCleanFailedView;

private:
    AntiVirusSetting& mConfig;

    int mScanType;
    int mVirusEngineType;
    int mFindVirusType;
    int mCleanFailType;
};

class VirusSettingView : public SettingView {
public:
    VirusSettingView(AntiVirusSetting& config,int x, int y, int width, int height);
    ~VirusSettingView();

private:
    void Layout(AntiVirusSetting& config);

public:
    OptionView  *mBackupView;
    OptionView  *mLargeFileView;
    OptionView  *mSpaceLessView;
    OptionView  *mBackupFailedView;      

private:
    AntiVirusSetting& mConfig;
    int mVirusBackupType;
};

class AdminPasswdView : public SettingView, public HandleObserver {
public:
    AdminPasswdView(int x, int y, int width, int height);
    ~AdminPasswdView();

private:
    void Layout();
    int ProcessEvent(int event);
    void HandleMessage(int event, int type, void *object);

private:
    EditView    *mOldPasswd;
    EditView    *mNewPasswd;
    EditView    *mConfirmPasswd;

    std::string mOldPwd;
    std::string mNewPwd;
    std::string mConfirmPwd;

    Handler *mHandler;
};

class UpdateSetView : public SettingView {
public:
    UpdateSetView(UpdateConfig& config, int x, int y, int width, int height);

private:
    void Layout(UpdateConfig& config);

private:
    OptionView  *mUpdateType;
    OptionView  *mDelayStart;
    OptionView  *mUpdateContent;
    OptionView  *mProxyView;
    OptionView  *mUpdateSource;

};

class SingleInputView : public SettingView, public HandleObserver {
public:
    SingleInputView(std::string& content, int x, int y, int width, int height);
    ~SingleInputView();

    std::string GetContent() const {
        return mContent;
    }

private:
    void Layout();
    int ProcessEvent(int event);
    void HandleMessage(int event, int type, void *object);

private:
    EditView    *mInputView;

    std::string& mContent;

    Handler *mHandler;
};

class ScheduleSetView : public SettingView {
public:
    ScheduleSetView(int& scanType, int& scanPos, int x, int y, int width, int height);

private:
    void Layout(int& scanType, int& scanPos);

private:
    OptionView  *mDateView;
    OptionView  *mTypeView;

private:
    int &mScanPos;
    int &mScanType;
};

class UpdateActiveView : public SettingView, public HandleObserver {
public:
    UpdateActiveView(int x, int y, int width, int height);
    ~UpdateActiveView();

    const std::string & GetSerial() {
        return mSerial;
    }

    const std::string & GetLicPath() {
        return mLicPath;
    }

private:
    void Layout();
    int  ProcessEvent(int event);
    void HandleMessage(int event, int type, void *object);

private:
    EditView     *mSerialView, *mLicPathView;
    std::string   mSerial, mLicPath;
    Handler      *mHandler;
};

#endif // SETTINGVIEW_H
