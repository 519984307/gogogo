#ifndef BACKUPGROUP_H
#define BACKUPGROUP_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QButtonGroup>

#include "common/checkboxlabel.h"
#include "common/radiobutton.h"


class CheckboxLabel;
class RadioButton;


class BackupGroup : public QGroupBox
{
    Q_OBJECT
public:
    BackupGroup(QWidget *parent = Q_NULLPTR);

    void SetFocus(bool bFocus);

public:
    bool m_bFocus;

    CheckboxLabel   *m_pclBackup;

    QButtonGroup    *m_pbgLongFile;
    RadioButton *m_prbAskMe;
    RadioButton *m_prbDeleteFile;
    RadioButton *m_prbDontCare;

    QButtonGroup    *m_pbgNoSpace;
    RadioButton *m_prbAutoReplace;
    RadioButton *m_prbAutoIncrease;

    QButtonGroup    *m_pbgBackupFail;
    RadioButton *m_prbFailAskMe;
    RadioButton *m_prbFailDeleteFile;
    RadioButton *m_prbFailDontCare;



//    CheckboxLabel   *m_pVirusTrace;
//    CheckboxLabel   *m_pVirusLog;
//    CheckboxLabel   *m_pCloud;
//    CheckboxLabel   *m_pBuffer;

};


#endif // BACKUPGROUP_H
