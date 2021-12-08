#ifndef UDISKGROUP_H
#define UDISKGROUP_H

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


class UDiskGroup : public QGroupBox
{
    Q_OBJECT
public:
    UDiskGroup(QWidget *parent = Q_NULLPTR);

    void SetFocus(bool bFocus);

public slots:
    void SetSizeEditValid();

public:
    bool m_bFocus;

    QLabel *lcomment1;
    QLabel *lcomment2;

    QButtonGroup    *m_pbgOperationType;
    RadioButton     *m_prbAskMe;
    RadioButton     *m_prbProcImmediately;

    QLineEdit   *m_pleSize;

};

#endif // UDISKGROUP_H
