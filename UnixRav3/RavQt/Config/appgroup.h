#ifndef APPGROUP_H
#define APPGROUP_H

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


class AppGroup : public QGroupBox
{
    Q_OBJECT
public:
    AppGroup(QWidget *parent = Q_NULLPTR);

    void SetFocus(bool bFocus);

public slots:
    void SetSizeEditValid();

public:
    bool m_bFocus;

    QButtonGroup    *m_pbgFoundThreaten;
    RadioButton     *m_prbEnableRun;
    RadioButton     *m_prbDisableRun;

    QButtonGroup    *m_pbgProcType;
    RadioButton     *m_prbAuto;
    RadioButton     *m_prbAskMe;

    CheckboxLabel   *m_pclLogIntercept;

    CheckboxLabel   *m_pclPopup;



};


#endif // APPGROUP_H
