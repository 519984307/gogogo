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

#include "common/checkboxlabel.h"
#include "common/radiobutton.h"
#include "common/combobox.h"

class CheckboxLabel;
class RadioButton;
class ComboBox;


class UpgradeGroup : public QGroupBox
{
public:
    UpgradeGroup(QWidget *parent = Q_NULLPTR);

    void SetFocus(bool bFocus);

public slots:
    void SetSizeEditValid();

public:
    bool m_bFocus;

    ComboBox   *m_pcbUpgradeMode;

    QButtonGroup    *m_pbgUpgradeType;
    RadioButton     *m_prbUpgradeAll;
    RadioButton     *m_prbUpgradeVirusDB;

    ComboBox   *m_pcbProxyType;
    QLineEdit   *m_pleAddress;
    QLineEdit   *m_plePort;
    QLineEdit   *m_pleUser;
    QLineEdit   *m_plePassword;

};


#endif // UPGRADEGROUP_H
