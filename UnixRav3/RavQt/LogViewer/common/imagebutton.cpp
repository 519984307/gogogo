#include <QPainter>
#include <QDebug>

#include "pubdef.h"
#include "imagebutton.h"

ImageButton::ImageButton(QString sStateImage, QWidget *parent)
    :QPushButton(parent),m_ppStateImage(NULL)
{
    LoadImage(sStateImage);
    setFixedSize(m_ppStateImage->width()/3, m_ppStateImage->height());
    setCursor(Qt::PointingHandCursor);
    m_bEntered = false;
    m_bLeave = true;
    m_bMousePress = false;
}

ImageButton::~ImageButton()
{
    if(m_ppStateImage) {
        delete m_ppStateImage;
        m_ppStateImage = NULL;
    }
}

void ImageButton::ResetImage(QString sStateImage)
{
    LoadImage(sStateImage);
    setFixedSize(m_ppStateImage->width()/3, m_ppStateImage->height());
    update();
}

void ImageButton::enterEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = true;
    m_bLeave = false;
    update();
}

void ImageButton::leaveEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = false;
    m_bLeave = true;
}

void ImageButton::mousePressEvent(QMouseEvent *event)
{
    //若点击鼠标左键
    if(event->button() == Qt::LeftButton)
    {
        m_bMousePress = true;
        update();
    }
}

void ImageButton::mouseReleaseEvent(QMouseEvent *)
{
    if(m_bMousePress)
    {
        m_bMousePress = false;
        update();
        emit clicked();
    }
}

void ImageButton::paintEvent(QPaintEvent *pPEvent)
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
}

void ImageButton::LoadImage(QString sStateImage)
{
    if(m_ppStateImage) {
        delete m_ppStateImage;
        m_ppStateImage = NULL;
    }
    m_ppStateImage = new QPixmap;
    bool bret = m_ppStateImage->load(sStateImage);
    if(!bret){
        qDebug("ImageButton::LoadImage sStateImage fail");
    }
}
