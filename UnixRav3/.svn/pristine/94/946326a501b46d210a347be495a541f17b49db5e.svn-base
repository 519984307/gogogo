#ifndef SYSTEMGROUP_H
#define SYSTEMGROUP_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QButtonGroup>
#include <QLineEdit>

#include "common/checkboxlabel.h"
#include "common/radiobutton.h"

class CheckboxLabel;
class RadioButton;


class SystemGroup : public QGroupBox
{
    Q_OBJECT
public:
    SystemGroup(QWidget *parent = Q_NULLPTR);

    void SetFocus(bool bFocus);

public slots:
    void SetSizeEditValid();

public:
    bool m_bFocus;

    QButtonGroup    *m_pbgFoundThreaten;
    RadioButton     *m_prbProcAuto;
    RadioButton     *m_prbAskMe;

    CheckboxLabel   *m_pclLogIntercept;

    QButtonGroup    *m_pbgMonitorSensibility;
    RadioButton     *m_prbLow;
    RadioButton     *m_prbMiddle;
    RadioButton     *m_prbHigh;

    CheckboxLabel   *m_pclAuditMode;

    CheckboxLabel   *m_pclPassDigitalSignature;
};

#endif // SYSTEMGROUP_H
