#include <QPainter>
#include <QMouseEvent>

#include "checkboximagelabel.h"

CheckboxImageLabel::CheckboxImageLabel(QWidget *parent, QString sRightText)
    :QLabel(parent)
{
    m_bChecked = false;
    m_bHoveredCheckbox = false;
    LoadImage(sRightText);
    setCursor(Qt::PointingHandCursor);
}

void CheckboxImageLabel::LoadImage(QString sRightText)
{
    m_ppCheckBox = new QPixmap;
    bool bret = m_ppCheckBox->load(":/resource/checkbox");
    if(!bret){
//        qDebug("CheckboxImageLabel::LoadImage checkbox fail");
    }
    m_ppRightText = new QPixmap;
    bret = m_ppRightText->load(sRightText);
    if(!bret){
//        qDebug("CheckboxImageLabel::LoadImage sRightText fail");
    }
    m_nWidth = m_ppCheckBox->width()/4+m_ppRightText->width();
    m_nHeight = m_ppCheckBox->height()/4>m_ppRightText->height()?m_ppCheckBox->height()/4:m_ppRightText->height();
    setFixedSize(m_nWidth, m_nHeight);
}

void CheckboxImageLabel::enterEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    //QPoint pt = QCursor::pos();
    //if(QRect(0,0,m_nWidth,m_nHeight).contains(mapFromGlobal(pt))){
        m_bHoveredCheckbox = true;
    //}else{
    //    m_bHoveredCheckbox = false;
    //}
    update();
}

void CheckboxImageLabel::leaveEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bHoveredCheckbox = false;
    update();
}

void CheckboxImageLabel::mouseReleaseEvent(QMouseEvent *pEvent)
{
    if(m_bHoveredCheckbox && pEvent->button()==Qt::LeftButton){
        m_bChecked = !m_bChecked;
        emit clicked();
    }
    update();
}

void CheckboxImageLabel::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);
    QPainter painter(this);
    if(m_bChecked){
        if(m_bHoveredCheckbox){
            painter.drawPixmap(0,0,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->copy(m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4));
        }else{
            painter.drawPixmap(0,0,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->copy(0,m_ppCheckBox->height()/4,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4));
        }
    }else{
        if(m_bHoveredCheckbox){
            painter.drawPixmap(0,0,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->copy(m_ppCheckBox->width()/4,0,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4));
        }else{
            painter.drawPixmap(0,0,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->copy(0,0,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4));
        }
    }
    painter.drawPixmap(m_ppCheckBox->width()/4,4,m_ppRightText->width(),m_ppRightText->height(),*m_ppRightText);
}
