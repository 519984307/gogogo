#include <QGridLayout>
#include <QHBoxLayout>

#include "common/pubdef.h"

#include "backupgroup.h"


#define BACKUP_LEFT_LABEL_WIDTH    150
#define BACKUP_RIGHT_CHKBOX_WIDTH  180

BackupGroup::BackupGroup(QWidget *parent):QGroupBox(parent),m_bFocus(false)
{
    setTitle(u8"杀毒备份");
    QFont font;
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
    font.setPointSize(10);
    setFont(font);

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
    setFixedHeight(140);
    setFlat(true);

    setStyleSheet("QGroupBox {\
                  font-size:13px; \
                  border: 2px solid #7B599F;\
                  margin-top: 1ex; \
                  }\
                  QGroupBox::title {\
                  top: 1px;\
                  left: 20px;\
                  subcontrol-origin: margin;\
                  subcontrol-position: top left;}");

    QLabel  *lBackup = new QLabel(u8"备份文件：", this);
    lBackup->setFont(font);
    //lBackup->setFixedWidth(BACKUP_LEFT_LABEL_WIDTH);
    m_pclBackup = new CheckboxLabel(this, u8"杀毒时备份原文件");
    m_pclBackup->setFont(font);
    lBackup->setFixedHeight(m_pclBackup->height());
    lBackup->setGeometry(25, 30, lBackup->width(), lBackup->height());
    m_pclBackup->setGeometry(110, 30, m_pclBackup->width(), m_pclBackup->height());

    QLabel  *lLongFile = new QLabel(u8"文件超长：", this);
    lLongFile->setFixedHeight(m_pclBackup->height());
    lLongFile->setFont(font);
    m_pbgLongFile = new QButtonGroup(this);
    m_prbAskMe = new RadioButton(u8"询问我", this);
    //m_prbAskMe->setText(u8"询问我");
    m_prbAskMe->setFixedHeight(m_pclBackup->height());
    m_prbAskMe->setFont(font);
    m_prbDeleteFile = new RadioButton(u8"删除文件", this);
    m_prbDeleteFile->setFixedHeight(m_pclBackup->height());
    m_prbDeleteFile->setFont(font);
    m_prbDontCare = new RadioButton(u8"不处理", this);
    m_prbDontCare->setFixedHeight(m_pclBackup->height());
    m_prbDontCare->setFont(font);
    m_pbgLongFile->addButton(m_prbAskMe, APP_ASK_ME);
    m_pbgLongFile->addButton(m_prbDeleteFile, APP_DELETE_FILE);
    m_pbgLongFile->addButton(m_prbDontCare, APP_DONT_CARE);
    lLongFile->setGeometry(25, 55, lLongFile->width(), lLongFile->height());
    m_prbAskMe->setGeometry(110, 55, m_prbAskMe->width()+5, m_prbAskMe->height());
    m_prbDeleteFile->setGeometry(270, 55, m_prbDeleteFile->width()+5, m_prbDeleteFile->height());
    m_prbDontCare->setGeometry(430, 55, m_prbDontCare->width()+5, m_prbDontCare->height());

    QLabel *lNoSpace = new QLabel(u8"空间不足：", this);
    lNoSpace->setFont(font);
    lNoSpace->setFixedHeight(m_pclBackup->height());
    m_pbgNoSpace = new QButtonGroup(this);
    m_prbAutoReplace = new RadioButton(u8"自动覆盖老文件", this);
    m_prbAutoReplace->setFixedSize(130, m_pclBackup->height());
    m_prbAutoReplace->setFont(font);
    m_prbAutoIncrease = new RadioButton(u8"空间自动增长", this);
    m_prbAutoIncrease->setFixedHeight(m_pclBackup->height());
    m_prbAutoIncrease->setFont(font);
    m_pbgNoSpace->addButton(m_prbAutoReplace, APP_AUTO_REPLACE);
    m_pbgNoSpace->addButton(m_prbAutoIncrease, APP_AUTO_INCREASE);
    lNoSpace->setGeometry(25, 80, lNoSpace->width(), lNoSpace->height());
    m_prbAutoReplace->setGeometry(110, 80, m_prbAutoReplace->width()+5, m_prbAutoReplace->height());
    m_prbAutoIncrease->setGeometry(270, 80, m_prbAutoIncrease->width()+5, m_prbAutoIncrease->height());

    QLabel  *lBackupOption = new QLabel(u8"备份失败：", this);
    lBackupOption->setFont(font);
    lBackupOption->setFixedHeight(m_pclBackup->height());
    m_pbgBackupFail = new QButtonGroup(this);
    m_prbFailAskMe = new RadioButton(u8"询问我", this);
    m_prbFailAskMe->setFixedHeight(m_pclBackup->height());
    m_prbFailAskMe->setFont(font);
    m_prbFailDeleteFile = new RadioButton(u8"删除文件", this);
    m_prbFailDeleteFile->setFixedHeight(m_pclBackup->height());
    m_prbFailDeleteFile->setFont(font);
    m_prbFailDontCare = new RadioButton(u8"不处理", this);
    m_prbFailDontCare->setFixedHeight(m_pclBackup->height());
    m_prbFailDontCare->setFont(font);
    m_pbgBackupFail->addButton(m_prbFailAskMe, APP_ASK_ME);
    m_pbgBackupFail->addButton(m_prbFailDeleteFile, APP_DELETE_FILE);
    m_pbgBackupFail->addButton(m_prbFailDontCare, APP_DONT_CARE);
    lBackupOption->setGeometry(25, 105, lBackupOption->width(), lBackupOption->height());
    m_prbFailAskMe->setGeometry(110, 105, m_prbFailAskMe->width()+5, m_prbFailAskMe->height());
    m_prbFailDeleteFile->setGeometry(270, 105, m_prbFailDeleteFile->width()+5, m_prbFailDeleteFile->height());
    m_prbFailDontCare->setGeometry(430, 105, m_prbFailDontCare->width()+5, m_prbFailDontCare->height());

    SetFocus(m_bFocus);
}

void BackupGroup::SetFocus(bool bFocus)
{
    m_bFocus = bFocus;
    if(m_bFocus){
        setStyleSheet("QGroupBox {\
              font-size:13px; \
              border: 2px solid #7B599F;\
              margin-top: 1ex; \
              }\
              QGroupBox::title {\
              top: 1px;\
              left: 20px;\
              subcontrol-origin: margin;\
              subcontrol-position: top left;}");

    }else{
        setStyleSheet("QGroupBox {\
              font-size:13px; \
              border: 2px solid #EFEBE7;\
              margin-top: 1ex; \
              }\
              QGroupBox::title {\
              top: 1px;\
              left: 20px;\
              subcontrol-origin: margin;\
              subcontrol-position: top left;}");
    }
    update();
}

