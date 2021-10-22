#ifndef CENTERDISCOVERY_H
#define CENTERDISCOVERY_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QButtonGroup>
#include <QLineEdit>
#include <QComboBox>
#include <QRegExpValidator>


class CheckboxLabel;
class RadioButton;
class ComboBox;
class WeekDaySelector;
class PushButton;
class CenterDiscoveryTable;


class CenterDiscoveryGroup : public QGroupBox
{
    Q_OBJECT
public:
    CenterDiscoveryGroup(QWidget *parent = Q_NULLPTR);

    void SetFocus(bool bFocus);

public slots:
    void OnLineEditTextChanged(const QString &text);
    void AddCenterClicked();

public:
    bool m_bFocus;

    QLabel      *lTableName;
    PushButton  *m_pbAddBtn;
    CenterDiscoveryTable    *m_pTable;

//    QLabel      *lConnectType;
//    ComboBox    *m_pcbConnectType;

//    QLabel      *lUploadBandwidthLimit;
//    ComboBox    *m_pcbUploadBandwidthLimit;
//    QLabel      *lValidTime;
//    QLineEdit   *m_pleValidTimeStart;
//    QLabel      *lSeparate;
//    QLineEdit   *m_pleValidTimeEnd;

//    CheckboxLabel   *m_pclUseProxy;
//    QLabel      *lUseProxyDesc;
//    CheckboxLabel   *m_pclOnlyValidedForTerminal;

    QRegExpValidator *m_pValidatorTime;
};


#endif // CENTERDISCOVERY_H
