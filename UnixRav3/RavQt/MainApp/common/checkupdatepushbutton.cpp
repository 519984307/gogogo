#include <QPainter>

#include "checkupdatepushbutton.h"

CheckUpdatePushButton::CheckUpdatePushButton()
{

}

CheckUpdatePushButton::CheckUpdatePushButton(QWidget *parent, QString sBackgroundImage)
    :QPushButton(parent)
{
    m_bEntered = false;
    m_bLeave = true;
    loadImage(sBackgroundImage);
    //qDebug("CheckUpdatePushButton::CheckUpdatePushButton");
}

//void CheckUpdatePushButton::clicked(bool checked)
//{

//}

void CheckUpdatePushButton::enterEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = true;
    m_bLeave = false;
    update();
    //qDebug() << "enterEvent";
}

void CheckUpdatePushButton::leaveEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = false;
    m_bLeave = true;
    update();
    //qDebug() << "leaveEvent";
}

void CheckUpdatePushButton::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);
    QPainter painter(this);
    if(m_bEntered){
        painter.drawPixmap(0, 0, m_ppBackgroundImage->width()/3,m_ppBackgroundImage->height(),
                           m_ppBackgroundImage->copy(m_ppBackgroundImage->width()/3, 0, m_ppBackgroundImage->width()/3, m_ppBackgroundImage->height()));
    }
    if(m_bLeave){
        painter.drawPixmap(0, 0, m_ppBackgroundImage->width()/3,m_ppBackgroundImage->height(),
                           m_ppBackgroundImage->copy(0, 0, m_ppBackgroundImage->width()/3, m_ppBackgroundImage->height()));
     }
}

int CheckUpdatePushButton::loadImage(QString sBackgroudImage)
{
    m_ppBackgroundImage = new QPixmap;
    bool ret = m_ppBackgroundImage->load(sBackgroudImage);
    if(!ret){
//        qDebug("CheckUpdatePushButton::loadImage fail");
    }
    setFixedSize(m_ppBackgroundImage->width()/3, m_ppBackgroundImage->height());
    return 0;
}

