#include <QPainter>
#include <QMouseEvent>

#include "checkboxlabel.h"

CheckboxLabel::CheckboxLabel(QWidget *parent, QString sRightText)
    :QLabel(parent)
{
    m_bChecked = false;
    m_bHoveredCheckbox = false;
    m_sRightText = sRightText;
    LoadImage();
    setCursor(Qt::PointingHandCursor);
    setFixedSize(m_nWidth, m_nHeight);
}

void CheckboxLabel::LoadImage()
{
    m_ppCheckBox = new QPixmap;
    bool bret = m_ppCheckBox->load(":/resource/checkbox");
    if(!bret){
        qDebug("CheckboxLabel::LoadImage checkbox fail");
    }

    m_nWidth = m_ppCheckBox->width()/4 + m_sRightText.length()*13 + 8;
    m_nHeight = m_ppCheckBox->height()/4;
}

void CheckboxLabel::enterEvent(QEvent *pEvent)
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

void CheckboxLabel::leaveEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bHoveredCheckbox = false;
    update();
}

//void CheckboxLabel::mousePressEvent(QMouseEvent *pEvent)
//{

//}

void CheckboxLabel::mouseReleaseEvent(QMouseEvent *pEvent)
{
    if(m_bHoveredCheckbox && pEvent->button()==Qt::LeftButton){
        m_bChecked = !m_bChecked;
        emit clicked();
    }
    update();
}

void CheckboxLabel::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);
    QPainter painter(this);
    if(m_bChecked){
        if (isEnabled()) {
            if(m_bHoveredCheckbox){
                painter.drawPixmap(0,2,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->copy(m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4));
            }else{
                painter.drawPixmap(0,2,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->copy(0,m_ppCheckBox->height()/4,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4));
            }
        }else{
            painter.drawPixmap(0,2,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->copy(m_ppCheckBox->width()/4*3,m_ppCheckBox->height()/4,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4));
        }
    }else{
        if (isEnabled()) {
            if(m_bHoveredCheckbox){
                painter.drawPixmap(0,2,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->copy(m_ppCheckBox->width()/4,0,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4));
            }else{
                painter.drawPixmap(0,2,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->copy(0,0,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4));
            }
        }else{
            painter.drawPixmap(0,2,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->copy(m_ppCheckBox->width()/4*3,0,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4));
        }
    }
#ifdef QT_NEW_HEADER_54
    QFont font("Arial", 10, QFont::Thin, false);
#else
    QFont font("Arial", 10, QFont::Light, false);
#endif
    painter.setFont(font);
    painter.drawText(m_ppCheckBox->width()/4+8, 2, m_nWidth-m_ppCheckBox->width()/4-8, m_nHeight, Qt::AlignLeft, m_sRightText);
}
