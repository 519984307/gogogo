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
//        qDebug("CheckboxLabel::LoadImage checkbox fail");
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

void CheckboxLabel::leaveEvent(QEvent *)
{
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
        update();
        emit clicked();
        return;
    }
    update();
}

void CheckboxLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(m_bChecked){
        if(m_bHoveredCheckbox){
            painter.drawPixmap(0,2,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->copy(m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4));
        }else{
            painter.drawPixmap(0,2,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->copy(0,m_ppCheckBox->height()/4,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4));
        }
    }else{
        if(m_bHoveredCheckbox){
            painter.drawPixmap(0,2,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->copy(m_ppCheckBox->width()/4,0,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4));
        }else{
            painter.drawPixmap(0,2,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4,m_ppCheckBox->copy(0,0,m_ppCheckBox->width()/4,m_ppCheckBox->height()/4));
        }
    }
    QFont font;
    font.setPixelSize(10);
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#ifdef QT_NEW_HEADER_54
    font.setWeight(0);
#else
    font.setWeight(25);
#endif
#else //QT_VERSION
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
#endif //QT_VERION
    painter.setFont(font);
    painter.drawText(m_ppCheckBox->width()/4+8, 2, m_nWidth-m_ppCheckBox->width()/4-8, m_nHeight, Qt::AlignLeft, m_sRightText);
}
