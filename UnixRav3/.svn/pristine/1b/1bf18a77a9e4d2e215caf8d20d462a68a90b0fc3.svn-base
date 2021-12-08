#include <QGridLayout>
#include <QHBoxLayout>

#include "common/pubdef.h"
#include "udiskgroup.h"



UDiskGroup::UDiskGroup(QWidget *parent):QGroupBox(parent),m_bFocus(false)
{
    setTitle(u8"U盘监控");
    QFont font;
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
    font.setPointSize(10);
    setFont(font);

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
    setFixedHeight(100);
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

    QLabel *lUDisk = new QLabel(u8"插入U盘时：", this);
    lUDisk->setFont(font);
    lUDisk->adjustSize();
    m_pbgOperationType = new QButtonGroup;
    m_prbAskMe = new RadioButton(u8"询问是否查杀", this);
    m_prbProcImmediately = new RadioButton(u8"立即查杀", this);
    m_pbgOperationType->addButton(m_prbAskMe, APP_ASK_ME);
    m_pbgOperationType->addButton(m_prbProcImmediately, APP_PROC_IMMEDIATELY);
    m_prbAskMe->setFont(font);
    m_prbProcImmediately->setFont(font);
    lUDisk->setGeometry(25, 30, lUDisk->width(), 18);
    m_prbAskMe->setGeometry(110, 30, m_prbAskMe->width(), lUDisk->height());
    m_prbProcImmediately->setGeometry(270, 30, m_prbProcImmediately->width(), lUDisk->height());

    QLabel  *lScanLevel = new QLabel(u8"查杀深度：", this);
    lScanLevel->adjustSize();
    lScanLevel->setFont(font);
    lcomment1 = new QLabel(u8"递归查杀", this);
    lcomment2 = new QLabel(u8"层目录深度", this);
    QLabel *lcomment3 = new QLabel(u8"（-1代表查杀所有目录）", this);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::gray);
    lcomment3->setPalette(pa);
    lcomment1->adjustSize();
    lcomment2->adjustSize();
    lcomment3->adjustSize();
    m_pleSize = new QLineEdit("2", this);
    lcomment1->setFont(font);
    lcomment2->setFont(font);
    QPalette pd;
    pd.setColor(QPalette ::Disabled,QPalette::Text,Qt::gray);
    lcomment1->setPalette(pd);
    lcomment2->setPalette(pd);
    lcomment3->setFont(font);
    m_pleSize->setFont(font);
    lScanLevel->setGeometry(25, 60, lScanLevel->width(), lScanLevel->height());
    lcomment1->setGeometry(110, 60, lcomment1->width()+5, lScanLevel->height());
    m_pleSize->setGeometry(lcomment1->x()+lcomment1->width(), 57, 40, 18);
    lcomment2->setGeometry(m_pleSize->x()+m_pleSize->width(), 60, lcomment2->width()+5, lScanLevel->height());
    lcomment3->setGeometry(lcomment2->x()+lcomment2->width(), 60, lcomment3->width()+5, lScanLevel->height());

    SetFocus(m_bFocus);
}

void UDiskGroup::SetFocus(bool bFocus)
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

void UDiskGroup::SetSizeEditValid()
{
    if (m_pleSize->isEnabled()){
        lcomment1->setEnabled(true);
        lcomment2->setEnabled(true);
    }else{
        lcomment1->setEnabled(false);
        lcomment2->setEnabled(false);
    }
}

