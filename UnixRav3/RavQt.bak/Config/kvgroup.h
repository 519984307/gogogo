#ifndef KVGROUP_H
#define KVGROUP_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QButtonGroup>
#include <QLineEdit>
#include <QPalette>

#include "common/checkboxlabel.h"
#include "common/radiobutton.h"

class CheckboxLabel;
class RadioButton;


class KVGroup : public QGroupBox
{
    Q_OBJECT
public:
    KVGroup(QWidget *parent = Q_NULLPTR);

    void SetFocus(bool bFocus);
    void SetCloudEngine(bool bEnable, const char * szUrl);

public slots:
    void SetSizeEditValid();
    void UpdateEditStatus(int buttonID);

protected:


public:
    bool m_bFocus;

    QLabel  *lFileType;
    QLabel  *lKVEngine;
    QLabel *lcomment1;
    QLabel *lcomment2;
    QLabel *lcomment3;
    QLabel *lcomment4;
    QLabel *lcomment5;
    QLabel *lFoundVirus;

    QPalette normalLabelPalette;
    QPalette disableLablePalette;

    QButtonGroup    *m_pbgFileType;
    RadioButton *m_prbAllFiles;
    RadioButton *m_prbProgDoc;
    RadioButton *m_prbUserDefine;
    QLineEdit   *m_pleFileTypes;

    CheckboxLabel   *m_pclOnWildlist;
    CheckboxLabel   *m_pclHeurist;
    CheckboxLabel   *m_pclProcCompressed;
    QLineEdit   *m_pleSize;


    QButtonGroup    *m_pbgFoundVirus;
    RadioButton *m_prbAuto;
    RadioButton *m_prbManual;
    RadioButton *m_prbIgnore;


	QLabel        * m_pCloudEngineLabel;
	QLabel        * m_pCloudUrlLabel;
	CheckboxLabel * m_pCloudEnable;
	QLineEdit     * m_pCloudUrl;
};


#endif // KVGROUP_H
