#ifndef TIMERGROUP_H
#define TIMERGROUP_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QButtonGroup>
#include <QLineEdit>

#include "common/pubdef.h"

class CheckboxLabel;
class RadioButton;
class ComboBox;
class WeekDaySelector;
class QRegExpValidator;


class TimerGroup : public QGroupBox
{
    Q_OBJECT
public:
    TimerGroup(QWidget *parent = Q_NULLPTR);

    void SetFocus(bool bFocus);

    void UpdateControlStatus();

public slots:
    void SetSizeEditValid();
    void OnClickedSubTitleCheckBox();

    void OnLineEditTextChanged(const QString &text);

    void onUpdateAllDiskControlState();
    void onUpdateQuickDiskControlState();

public:
    bool m_bFocus;

    CheckboxLabel   *m_pclAllDiskScan;
    QButtonGroup    *m_pbgAllDiskScan;
    RadioButton *m_prbTPowerOn;
    RadioButton *m_prbTEveryDay;
    QLineEdit   *m_pleTDayTime;
    RadioButton *m_prbTEveryWeek;
    WeekDaySelector *m_pTWeekSel;
    QLineEdit   *m_pleTWeekTime;
    RadioButton *m_prbTEveryMonth;
    ComboBox    *m_pcbTMonthDay;
    QLineEdit   *m_pleTMonthTime;


    CheckboxLabel   *m_pclFastScan;
    QButtonGroup    *m_pbgFastScan;
    RadioButton *m_prbBPowerOn;
    RadioButton *m_prbBEveryDay;
    QLineEdit   *m_pleBDayTime;
    RadioButton *m_prbBEveryWeek;
    WeekDaySelector *m_pBWeekSel;
    QLineEdit   *m_pleBWeekTime;
    RadioButton *m_prbBEveryMonth;
    ComboBox    *m_pcbBMonthDay;
    QLineEdit   *m_pleBMonthTime;

    QRegExpValidator *m_pValidatorTime;

};


#endif // TIMERGROUP_H
