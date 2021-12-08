#include <QGridLayout>
#include <QHBoxLayout>
#include <QPalette>

#include "common/pubdef.h"
#include "admingroup.h"


AdminGroup::AdminGroup(QWidget *parent):QGroupBox(parent),m_bFocus(false)
{
    setTitle(QString::fromUtf8("管理员身份"));
    QFont font;
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
    font.setPointSize(10);
    setFont(font);
    QPalette pa;
    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
    setFixedHeight(90);
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

    QLabel *lAdmin = new QLabel(u8"管理员密码：", this);
    lAdmin->setFont(font);
    lAdmin->adjustSize();
    m_plePasswd = new QLineEdit(this);
    m_plePasswd->setStyleSheet("background:transparent;border-width:0;border-style:outset");
    m_plePasswd->setFont(font);
    m_plePasswd->setReadOnly(true);
    m_plePasswd->setFocusPolicy(Qt::NoFocus);
    m_pclSetOrChange = new ClickedLabel(this);
    m_pclSetOrChange->setFont(font);

    pa.setColor(QPalette::Text,Qt::gray);
    m_plePasswd->setPalette(pa);
    m_plePasswd->setText(u8"（暂无密码）");

    pa.setColor(QPalette::WindowText,Qt::blue);
    m_pclSetOrChange->setPalette(pa);
    m_pclSetOrChange->setText(u8"设置一个");
    m_pclSetOrChange->setCursor(Qt::PointingHandCursor);
    lAdmin->setGeometry(25, 30, lAdmin->width(), lAdmin->height());
    m_plePasswd->setGeometry(110, 30, 80, lAdmin->height());
    m_pclSetOrChange->setGeometry(200, 30,m_pclSetOrChange->width(), lAdmin->height());

    QLabel *lImg = new QLabel(this);
    lImg->setPixmap(QPixmap(":/Images/info_icon"));
    lImg->adjustSize();

    QLabel *lcomment = new QLabel(u8"此密码为管理员唯一身份标识，特殊操作时所用，密码锁定后不可修改。", this);
#ifdef QT_NEW_HEADER_54
    QFont font1("Arial", 9, QFont::Thin, false);
#else
    QFont font1("Arial", 9, QFont::Light, false);
#endif
    pa.setColor(QPalette::WindowText,Qt::gray);
    lcomment->setFont(font);
    lcomment->setPalette(pa);
    lcomment->adjustSize();
    lImg->setGeometry(25, 55, lImg->width(), lImg->height());
    lcomment->setGeometry(25+lImg->width()+5, 55, lcomment->width(), lImg->height());

    SetFocus(m_bFocus);
}

void AdminGroup::SetFocus(bool bFocus)
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

void AdminGroup::SetSizeEditValid()
{


}
