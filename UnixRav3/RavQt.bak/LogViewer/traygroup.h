#ifndef TRAYGROUP_H
#define TRAYGROUP_H

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


class TrayGroup : public QGroupBox
{
public:
    TrayGroup(QWidget *parent = Q_NULLPTR);

    void SetFocus(bool bFocus);

public slots:
    void SetSizeEditValid();

public:
    bool m_bFocus;

    CheckboxLabel   *m_pclHideTaskbarTray;

};

#endif // TRAYGROUP_H
