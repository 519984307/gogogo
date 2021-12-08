#ifndef UPGRADEGROUP_H
#define UPGRADEGROUP_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QButtonGroup>
#include <QLineEdit>
#include <QComboBox>
#include <QRegExpValidator>
#include <QMenu>
#include <QAction>


class CheckboxLabel;
class RadioButton;
class ComboBox;
class WeekDaySelector;
class PushButton;
class UpdateSourceTable;


class UpgradeGroup : public QGroupBox
{
    Q_OBJECT
public:
    UpgradeGroup(QWidget *parent = Q_NULLPTR);

    void SetFocus(bool bFocus);

    void MenuAddRisingClicked();
    void MenuAddOthersClicked();

public slots:
    void SetSizeEditValid();

    void OnLineEditTextChanged(const QString &text);
    void PopAddMenu();
    void TrigerAddSourceMenu(QAction *pAction);

    void OnClickedUpgradeMode(int index);
    void OnClickedDelayBootType(int index);
    void OnClickedForceUpgrade();
    void OnClickedBandWidth();
    void OnChangedProxyType(int index);

public:
    bool m_bFocus;
    //升级模式
    QButtonGroup   *m_pbgUpgradeMode;
    RadioButton     *m_prbManual;
    RadioButton     *m_prbEveryDay;
    RadioButton     *m_prbEveryWeek;
//    RadioButton     *m_prbInterval;

    QLineEdit       *m_pleDayTime;
    WeekDaySelector *m_pwdlWeekDays;
    QLineEdit       *m_pleWeekTime;
//    QLineEdit       *m_pleIntervalHours;
    QLabel *    lUpgradeModeTxt;
//    QLabel *    lHourTxt;


    //升级内容
    QButtonGroup    *m_pbgUpgradeType;
    RadioButton     *m_prbUpgradeAll;
    RadioButton     *m_prbUpgradeVirusDB;
    CheckboxLabel   *m_pclRealtimeCheckVDBVersion;
    QLabel *    lUpgradeType;

#if 0
    // 以下内容，是因为linux没有实现，所以先屏掉，什么时候有时间和windows同步了，再把此设置放开
    //延时启动
    QButtonGroup    *m_pbgDelayBoot;
    RadioButton     *m_prbDisabledDelayBoot;
    RadioButton     *m_prbAutoDalayBoot;
    RadioButton     *m_prbInRecentMinutesBoot;
    QLineEdit       *m_pleRecentMinutesBoot;
    QLabel  *   lDelayBootTxt;
    QLabel  *   lMinutesLaterTxt;

    //缓存空间
    CheckboxLabel   *m_pclCleaningSysTemp;
    QLabel  *   lBufferReserve;
    //强制更新
    CheckboxLabel   *m_pclForceUpgrade;
    QLineEdit       *m_pleAfterDaysUpgrade;
    QLabel  *   lForceUpdateTxt;
    QLabel  *   lDaysLaterTxt;
    //代理设置
    ComboBox   *m_pcbProxyType;
    QLineEdit   *m_pleAddress;
    QLineEdit   *m_plePort;
    QLineEdit   *m_pleUser;
    QLineEdit   *m_plePassword;
    QLabel *    lProxySetTxt;
    QLabel *    lAddressTxt;
    QLabel  *   lPortTxt;
    QLabel  *   lUserTxt;
    QLabel  *   lPasswdTxt;
    //带宽限制
    CheckboxLabel   *m_pclBandWidthLimit;
    QLineEdit       *m_pleLimitedBandWidth; //unit kb
    QLineEdit       *m_pleBandWidthLimitValidStartTime;
    QLineEdit       *m_pleBandWidthLimitValidEndTime;
    QLabel  *   lBandWidthLimitTxt;
    QLabel  *   lBandWidthLimitTxt1;
    QLabel  *   lBandWidthUnitTxt;
    QLabel  *   lValidTimeTxt;
    QLabel  *   lSpanSymbleTxt;
#endif
    //升级源
    UpdateSourceTable   *m_pTable;
    PushButton          *m_pbAddBtn;
    QLabel  *   lUpdateSourceTxt;
    QLabel  *   lAddOtherServer;

    QRegExpValidator *m_pValidatorTime;

    QMenu   *m_pMenu;
    QAction *m_paAddRising;
    QAction *m_paAddOthers;
};


#endif // UPGRADEGROUP_H
