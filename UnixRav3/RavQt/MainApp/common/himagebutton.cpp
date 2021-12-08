#include <QPainter>
#include <QDebug>

#include "himagebutton.h"

HImageButton::HImageButton(QString sStateImage, QWidget *parent)
    :QPushButton(parent)
{
    LoadImage(sStateImage);
    setFixedSize(m_ppStateImage->width(), m_ppStateImage->height()/3);
    setCursor(Qt::PointingHandCursor);
    m_bEntered = false;
    m_bLeave = true;
    m_bMousePress = false;
}

void HImageButton::enterEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = true;
    m_bLeave = false;
    update();
}

void HImageButton::leaveEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = false;
    m_bLeave = true;
}

void HImageButton::mousePressEvent(QMouseEvent *event)
{
    //若点击鼠标左键
    if(event->button() == Qt::LeftButton)
    {
        m_bMousePress = true;
        update();
    }
}

void HImageButton::mouseReleaseEvent(QMouseEvent *)
{
    if(m_bMousePress)
    {
        m_bMousePress = false;
        update();
        emit clicked();
    }
}

void HImageButton::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);
    QPainter painter(this);
    if(m_bEntered){
        if(m_bMousePress){
            painter.drawPixmap(0,0,m_ppStateImage->width(),m_ppStateImage->height()/3, m_ppStateImage->copy(0,m_ppStateImage->height()/3,m_ppStateImage->width(),m_ppStateImage->height()/3));
        }else{
            painter.drawPixmap(0,0,m_ppStateImage->width(),m_ppStateImage->height()/3, m_ppStateImage->copy(0,0,m_ppStateImage->width(),m_ppStateImage->height()/3));
        }
    }
    if(m_bLeave){
        painter.drawPixmap(0,0,m_ppStateImage->width(),m_ppStateImage->height()/3, m_ppStateImage->copy(0,0,m_ppStateImage->width(),m_ppStateImage->height()/3));
    }
}

void HImageButton::LoadImage(QString sStateImage)
{
    m_ppStateImage = new QPixmap;
    bool bret = m_ppStateImage->load(sStateImage);
    if(!bret){
//        qDebug("HImageButton::LoadImage sStateImage fail");
    }
}
