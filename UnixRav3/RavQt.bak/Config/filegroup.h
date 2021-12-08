#ifndef FILEGROUP_H
#define FILEGROUP_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QButtonGroup>
#include <QLineEdit>
#include <QPalette>

#include "common/pushbutton.h"
#include "common/checkboxlabel.h"
#include "common/radiobutton.h"

//class PushButton;
//class CheckboxLabel;
//class RadioButton;
class DirTable;
class ProcNameTable;
class DirSelectDialog;


class FileGroup : public QGroupBox
{
    Q_OBJECT
public:
    FileGroup(QWidget *parent = Q_NULLPTR);

    void SetFocus(bool bFocus);

public slots:
    void SetSizeEditValid();

    void AddDirClicked();
    void AddProcNameClicked();


public:
    bool m_bFocus;

    CheckboxLabel   *m_pclStartAtPowerOn;
    CheckboxLabel   *m_pclStartSmartBlacklist;
//    CheckboxLabel   *m_pclStartKernelScan;

    QButtonGroup    *m_pbgMode;
    //RadioButton     *m_prbFastMode;
    RadioButton     *m_prbStandardMode;
    RadioButton     *m_prbProfessionalMode;
    //RadioButton     *m_prbPlusMode;

    QButtonGroup    *m_pbgFileType;
    RadioButton     *m_prbAllFiles;
    RadioButton     *m_prbProgDoc;

    CheckboxLabel   *m_pTrustProgAnalysis;
//    CheckboxLabel   *m_pclEmbedSacan;

    CheckboxLabel   *m_pclOnWildlist;
    CheckboxLabel   *m_pclHeuristScan;
    CheckboxLabel   *m_pclProcCompressed;
    QLineEdit   *m_pleSize;
    QLabel *lcomment1;
    QLabel *lcomment2;

    QButtonGroup    *m_pbgFoundVirus;
    RadioButton *m_prbFoundVAuto;
    RadioButton *m_prbFoundVManual;
    RadioButton *m_prbFoundVIgnore;

    QButtonGroup    *m_pbgFailV;
    RadioButton *m_prbFailVDelete;
    RadioButton *m_prbFailVIgnore;

    CheckboxLabel   *m_pclNotifyMe;

    PushButton  *m_ppbDir;
    PushButton  *m_ppbProcName;
    DirTable    *m_pDirTable;
    ProcNameTable   *m_pProcNameTable;
};

#endif // FILEGROUP_H
