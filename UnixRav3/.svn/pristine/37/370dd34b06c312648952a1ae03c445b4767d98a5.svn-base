#include <QFont>
#include <QPainter>
#include <QDebug>

#include <string>

#include "defendbutton.h"

using namespace std;

DefendButton::DefendButton()
{

}

DefendButton::DefendButton(QString sBackgroundImage, QString sText, int nFontSize)
{
    m_bEntered = false;
    m_bLeave = true;
    loadImage(sBackgroundImage);
    setFixedSize(m_nBackgroundImageWidth, m_nBackgroundImageHeight/3);
    m_sText = sText;
    m_nFontSize = nFontSize;
    //qDebug("DefendButton::DefendButton");
}

void DefendButton::SetBtnText(QString sText, int nTotal, int nCurrent)
{
    m_sText = sText;
    m_nTotal = nTotal;
    m_nCurrent = nCurrent;
    update();
}

void DefendButton::enterEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = true;
    m_bLeave = false;
    update();
    //qDebug() << "enterEvent";
}

void DefendButton::leaveEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = false;
    m_bLeave = true;
    update();
    //qDebug() << "leaveEvent";
}

void DefendButton::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);
    QPainter painter(this);
    if(m_bEntered){
        painter.drawPixmap(0, 0, m_nBackgroundImageWidth, m_nBackgroundImageHeight/3, m_ppBackgroundImage->copy(0, m_nBackgroundImageHeight/3, m_nBackgroundImageWidth, m_nBackgroundImageHeight/3));
    }
    if(m_bLeave){
        painter.drawPixmap(0, 0, m_nBackgroundImageWidth, m_nBackgroundImageHeight/3, m_ppBackgroundImage->copy(0,0, m_nBackgroundImageWidth, m_nBackgroundImageHeight/3));
     }
    QFont font;
    font.setPixelSize(m_nFontSize);
    font.setWeight(QFont::Normal);
    painter.setFont(font);
    QRect pos = rect();
    //qDebug("DefendButton x,y,w,h %d  %d  %d  %d", pos.x(), pos.y(), pos.width(), pos.height());
    QRect newPos = QRect(pos.x()+10, pos.y(), pos.width(), pos.height());

    QRect qrect;
    int xpos = 0, width = 0;
    if(m_nTotal == 0) {
        painter.drawText(newPos, Qt::AlignHCenter|Qt::AlignVCenter, m_sText, &qrect);
        //qDebug("text pos1:x,y,w,h  %d  %d  %d  %d", qrect.x(), qrect.y(), qrect.width(), qrect.height());
    }else{
        QString sText = m_sText + " (";
        xpos = 55;
        width = pos.width();
        painter.drawText(QRect(xpos, pos.y(),width, pos.height()), Qt::AlignLeft|Qt::AlignVCenter, sText, &qrect);
        //qDebug("text pos1:x,y,w,h  %d  %d  %d  %d", qrect.x(), qrect.y(), qrect.width(), qrect.height());

        painter.setPen(QColor(255,0,0));
        sText.setNum(m_nCurrent);
        xpos += qrect.width();
        width -= qrect.width();
        painter.drawText(QRect(xpos, pos.y(), width, pos.height()), Qt::AlignLeft|Qt::AlignVCenter, sText, &qrect);
        //qDebug("qrect x,y,w,h:%d  %d  %d  %d", qrect.x(), qrect.y(), qrect.width(), qrect.height());

        painter.setPen(QColor(0,0,0));
        sText = QString("/%1)").arg(m_nTotal,0,10);
        xpos += qrect.width();
        width -= qrect.width();
        //qDebug("newPos x,y,w,h:%d  %d  %d  %d", newPos.x(), newPos.y(), newPos.width(), newPos.height());
        painter.drawText(QRect(xpos, pos.y(), width, pos.height()), Qt::AlignLeft|Qt::AlignVCenter, sText, &qrect);
    }
}

int DefendButton::loadImage(QString sBackgroudImage)
{
    m_ppBackgroundImage = new QPixmap;
    bool ret = m_ppBackgroundImage->load(sBackgroudImage);
    if(ret){
        m_nBackgroundImageWidth = m_ppBackgroundImage->width();
        m_nBackgroundImageHeight = m_ppBackgroundImage->height();
        //qDebug("BackWidth: %d BackHeight: %d", m_nBackgroundImageWidth, m_nBackgroundImageHeight);
    }

    return 0;
}
