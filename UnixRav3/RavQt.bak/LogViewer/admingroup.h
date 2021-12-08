#ifndef ADMINGROUP_H
#define ADMINGROUP_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QButtonGroup>
#include <QLineEdit>

#include "common/checkboxlabel.h"
#include "common/radiobutton.h"
#include "common/clickedlabel.h"

class CheckboxLabel;
class RadioButton;
class ClickedLabel;


class AdminGroup : public QGroupBox
{
public:
    AdminGroup(QWidget *parent = Q_NULLPTR);

    void SetFocus(bool bFocus);

public slots:
    void SetSizeEditValid();

public:
    bool m_bFocus;

    QLineEdit   *m_plePasswd;

    ClickedLabel  *m_pclSetOrChange;


};

#endif // ADMINGROUP_H
