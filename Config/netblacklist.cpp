#include <QGridLayout>
#include <QHBoxLayout>

#include "common/pubdef.h"

#include "kvgroup.h"


//#define BACKUP_LEFT_LABEL_WIDTH    150
//#define BACKUP_RIGHT_CHKBOX_WIDTH  180

KVGroup::KVGroup(QWidget *parent):QGroupBox(parent),m_bFocus(false)
{
    setTitle(u8"病毒扫描");
    QFont font("Arial", 10, QFont::Thin, false);
    setFont(font);

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
    setFixedHeight(210);
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

    QLabel  *lFileType = new QLabel(u8"文件类型：", this);
    lFileType->setFont(font);
    m_pbgFileType = new QButtonGroup;
    m_prbAllFiles = new RadioButton(u8"所有文件", this);
    m_prbProgDoc = new RadioButton(u8"程序和文档", this);
    m_pbgFileType->addButton(m_prbAllFiles, APP_ALL_FILES);
    m_pbgFileType->addButton(m_prbProgDoc, APP_PROG_AND_DOC);
    m_prbAllFiles->setFont(font);
    m_prbProgDoc->setFont(font);
    lFileType->setFixedHeight(m_prbAllFiles->height());
    lFileType->setGeometry(25, 30, lFileType->width(), lFileType->height());
    m_prbAllFiles->setGeometry(110, 30, m_prbAllFiles->width(), m_prbAllFiles->height());
    m_prbProgDoc->setGeometry(270, 30, m_prbProgDoc->width(), m_prbProgDoc->height());

    QLabel  *lKVEngine = new QLabel(u8"查杀引擎：", this);
    lKVEngine->setFixedHeight(m_prbAllFiles->height());
    lKVEngine->setFont(font);
    m_pclOnWildlist = new CheckboxLabel(this, u8"仅查杀流行病毒");
    m_pclHeurist = new CheckboxLabel(this, u8"启发式查杀");
    m_pclProcCompressed = new CheckboxLabel(this, u8"启动压缩包查杀");
    QLabel *lcomment1 = new QLabel(u8"查杀不大于", this);
    QLabel *lcomment2 = new QLabel(u8"M的压缩包", this);
    QLabel *lcomment3 = new QLabel(u8"（重点查杀活跃病毒）", this);
    QLabel *lcomment4 = new QLabel(u8"（可有效发现可疑病毒）", this);
    QLabel *lcomment5 = new QLabel(u8"（查杀压缩包内的文件）", this);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::gray);
    lcomment3->setPalette(pa);
    lcomment4->setPalette(pa);
    lcomment5->setPalette(pa);
    lcomment1->adjustSize();
    lcomment2->adjustSize();
    lcomment3->adjustSize();
    lcomment4->adjustSize();
    lcomment5->adjustSize();
    m_pleSize = new QLineEdit("100", this);
    m_pclOnWildlist->setFont(font);
    m_pclHeurist->setFont(font);
    m_pclProcCompressed->setFont(font);
    lcomment1->setFont(font);
    lcomment2->setFont(font);
    lcomment3->setFont(font);
    lcomment4->setFont(font);
    lcomment5->setFont(font);
    m_pleSize->setFont(font);
    lKVEngine->setGeometry(25, 55, lKVEngine->width(), lKVEngine->height());
    m_pclOnWildlist->setGeometry(110, 60, m_pclOnWildlist->width(), lcomment1->height());
    lcomment3->setGeometry(110+m_pclOnWildlist->width(), 60, lcomment3->width(), lcomment3->height());
    m_pclHeurist->setGeometry(110, 85, m_pclHeurist->width(), lcomment1->height());
    lcomment4->setGeometry(110+m_pclHeurist->width(), 85, lcomment4->width(), lcomment4->height());
    m_pclProcCompressed->setGeometry(110, 110, m_pclProcCompressed->width(), lcomment1->height());
    lcomment5->setGeometry(110+m_pclProcCompressed->width(), 110, lcomment5->width(), lcomment5->height());
    lcomment1->setGeometry(140, 135, lcomment1->width(), lcomment1->height());
    m_pleSize->setGeometry(140+lcomment1->width(), 135, 40, lcomment1->height());
    lcomment2->setGeometry(140+lcomment1->width()+45, 135, lcomment2->width(), lcomment2->height());

    QLabel *lFoundVirus = new QLabel(u8"发现病毒：", this);
    lFoundVirus->setFont(font);
    lFoundVirus->adjustSize();
    m_pbgFoundVirus = new QButtonGroup;
    m_prbAuto = new RadioButton(u8"自动处理", this);
    m_prbManual = new RadioButton(u8"手动处理", this);
    m_pbgFileType->addButton(m_prbAuto, APP_AUTO);
    m_pbgFileType->addButton(m_prbManual, APP_MANUAL);
    m_prbAuto->setFont(font);
    m_prbManual->setFont(font);
    lFoundVirus->setGeometry(25, 170, lFoundVirus->width(), lFoundVirus->height());
    m_prbAuto->setGeometry(110, 170, m_prbAuto->width(), lFoundVirus->height());
    m_prbManual->setGeometry(270, 170, m_prbManual->width(), lFoundVirus->height());

    connect(m_pclProcCompressed, &CheckboxLabel::clicked, this, &KVGroup::SetSizeEditValid);

    SetFocus(m_bFocus);
}

void KVGroup::SetFocus(bool bFocus)
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

void KVGroup::SetSizeEditValid()
{
    if(m_pclProcCompressed->m_bChecked == false){
        m_pleSize->setDisabled(true);
    }else{
        m_pleSize->setDisabled(false);
    }

}
