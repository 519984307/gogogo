#include <QFont>
#include <QPainter>
#include <QDebug>

#include "animitedbutton.h"

AnimitedButton::AnimitedButton()
{

}

AnimitedButton::AnimitedButton(QString sBackgroundImage, QString sCenterAnimitedImage, QString sText, int nFontSize)
{
    m_bEntered = false;
    m_bLeave = true;
    m_nCenterImgIndex = 0;
    loadImage(sBackgroundImage, sCenterAnimitedImage);
    //setStyleSheet("QPushButton{ text-align : bottom; }");
    setFixedSize(m_nBackgroundImageWidth/3, m_nBackgroundImageHeight);
    m_sText = sText;
    m_nFontSize = nFontSize;
}

void AnimitedButton::enterImageChange(QVariant qv)
{
    m_nCenterImgIndex = qv.toInt();
    //qDebug("enterImageChange event, %d", m_nCenterImgIndex);
    if(m_nCenterImgIndex >= 14){
        m_ppaAnimination->stop();
    }
    update();
}

void AnimitedButton::enterEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = true;
    m_bLeave = false;
    m_ppaAnimination->start();
    //qDebug() << "enterEvent";
}

void AnimitedButton::leaveEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    if(m_nCenterImgIndex >= 14){
        m_bEntered = false;
        //update();
    }
    m_bLeave = true;
    //qDebug() << "leaveEvent";
}

void AnimitedButton::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);
    QPainter painter(this);
    int xpos = (m_nBackgroundImageWidth/3 - m_nCenterImgWidth/15)/2;
    int ypos = (m_nBackgroundImageHeight - m_nCenterImgHeight)/2;
    if(m_bEntered){
        painter.drawPixmap(0, 0, m_nBackgroundImageWidth/3, m_nBackgroundImageHeight, m_ppBackgroundImage->copy(m_nBackgroundImageWidth/3, 0, m_nBackgroundImageWidth/3, m_nBackgroundImageHeight));
        painter.drawPixmap(xpos, ypos-20, m_nCenterImgWidth/15, m_nCenterImgHeight, m_ppCenterAnimitedImage->copy(m_nCenterImgWidth/15*m_nCenterImgIndex, 0, m_nCenterImgWidth/15, m_nCenterImgHeight));
    }
    if(m_bLeave){
        painter.drawPixmap(0, 0, m_nBackgroundImageWidth/3, m_nBackgroundImageHeight, m_ppBackgroundImage->copy(0,0, m_nBackgroundImageWidth/3, m_nBackgroundImageHeight));
        painter.drawPixmap(xpos, ypos-20, m_nCenterImgWidth/15, m_nCenterImgHeight, m_ppCenterAnimitedImage->copy(m_nCenterImgWidth/15*m_nCenterImgIndex, 0, m_nCenterImgWidth/15, m_nCenterImgHeight));
    }
    QFont font;
    font.setPixelSize(m_nFontSize);
    font.setWeight(57);
    painter.setFont(font);
    QRect pos = rect();
    //qDebug("x,y,w,h %d  %d  %d  %d", pos.x(), pos.y(), pos.width(), pos.height());
    QRect newPos = QRect(pos.x(), m_nCenterImgHeight, pos.width(), pos.height()-m_nCenterImgHeight);
    painter.drawText(newPos, Qt::AlignHCenter|Qt::AlignVCenter, m_sText);
}

int AnimitedButton::loadImage(QString sBackgroundImage, QString sAniminationImage)
{
    m_ppBackgroundImage = new QPixmap;
    bool ret = m_ppBackgroundImage->load(sBackgroundImage);
    if(ret){
        m_nBackgroundImageWidth = m_ppBackgroundImage->width();
        m_nBackgroundImageHeight = m_ppBackgroundImage->height();
        //qDebug("BackWidth: %d BackHeight: %d", m_nBackgroundImageWidth, m_nBackgroundImageHeight);
    }
    m_ppCenterAnimitedImage = new QPixmap;
    ret = m_ppCenterAnimitedImage->load(sAniminationImage);
    if(ret){
        m_nCenterImgWidth = m_ppCenterAnimitedImage->width();
        m_nCenterImgHeight = m_ppCenterAnimitedImage->height();
        //qDebug("CenterWidth: %d CenterHeight: %d", m_nCenterImgWidth, m_nCenterImgHeight);
    }
    m_nCenterImgIndex = 14;
    m_ppaAnimination = new QPropertyAnimation(this, "");
    m_ppaAnimination->setStartValue(0);
    m_ppaAnimination->setEndValue(14);
    m_ppaAnimination->setDuration(600);
    connect(m_ppaAnimination, SIGNAL(valueChanged(QVariant)), this, SLOT(enterImageChange(QVariant)));
    return 0;
}
