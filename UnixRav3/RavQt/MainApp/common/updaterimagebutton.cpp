#include <QPainter>
#include <QDebug>

#include "updaterimagebutton.h"

UpdaterImageButton::UpdaterImageButton(QString sStateImage, QWidget *parent)
    :QPushButton(parent)
{
    LoadImage(sStateImage);
    setFixedSize(m_ppStateImage->width()/3, m_ppStateImage->height());
    setCursor(Qt::PointingHandCursor);
    m_bEntered = false;
    m_bLeave = true;
    m_bMousePress = false;
}

void UpdaterImageButton::SetButtonText(QString sText)
{
    m_sText = sText;
    update();
}

void UpdaterImageButton::enterEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = true;
    m_bLeave = false;
    update();
}

void UpdaterImageButton::leaveEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = false;
    m_bLeave = true;
}

void UpdaterImageButton::mousePressEvent(QMouseEvent *event)
{
    //若点击鼠标左键
    if(event->button() == Qt::LeftButton)
    {
        m_bMousePress = true;
        update();
    }
}

void UpdaterImageButton::mouseReleaseEvent(QMouseEvent *)
{
    if(m_bMousePress)
    {
        m_bMousePress = false;
        update();
        emit clicked();
    }
}

void UpdaterImageButton::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);
    QPainter painter(this);
    if(m_bEntered){
        if(m_bMousePress){
            painter.drawPixmap(0,0,m_ppStateImage->width()/3,m_ppStateImage->height(), m_ppStateImage->copy(m_ppStateImage->width()/3*2,0,m_ppStateImage->width()/3,m_ppStateImage->height()));
        }else{
            painter.drawPixmap(0,0,m_ppStateImage->width()/3,m_ppStateImage->height(), m_ppStateImage->copy(m_ppStateImage->width()/3,0,m_ppStateImage->width()/3,m_ppStateImage->height()));
        }
    }
    if(m_bLeave){
        painter.drawPixmap(0,0,m_ppStateImage->width()/3,m_ppStateImage->height(), m_ppStateImage->copy(0,0,m_ppStateImage->width()/3,m_ppStateImage->height()));
    }
    if(m_sText.size() > 0)
        painter.drawText(rect(), Qt::AlignHCenter|Qt::AlignVCenter, QString::fromUtf8(m_sText.toUtf8()));
}

void UpdaterImageButton::LoadImage(QString sStateImage)
{
    m_ppStateImage = new QPixmap;
    bool bret = m_ppStateImage->load(sStateImage);
    if(!bret){
        qDebug("UpdaterImageButton::LoadImage sStateImage fail");
    }
}
