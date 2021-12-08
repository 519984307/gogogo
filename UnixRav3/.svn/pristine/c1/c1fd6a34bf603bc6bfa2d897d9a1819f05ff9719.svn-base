#ifndef KVGROUP_H
#define KVGROUP_H

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


class KVGroup : public QGroupBox
{
public:
    KVGroup(QWidget *parent = Q_NULLPTR);

    void SetFocus(bool bFocus);

public slots:
    void SetSizeEditValid();

public:
    bool m_bFocus;

    QButtonGroup    *m_pbgFileType;
    RadioButton *m_prbAllFiles;
    RadioButton *m_prbProgDoc;

    CheckboxLabel   *m_pclOnWildlist;
    CheckboxLabel   *m_pclHeurist;
    CheckboxLabel   *m_pclProcCompressed;
    QLineEdit   *m_pleSize;


    QButtonGroup    *m_pbgFoundVirus;
    RadioButton *m_prbAuto;
    RadioButton *m_prbManual;

};


#endif // KVGROUP_H
