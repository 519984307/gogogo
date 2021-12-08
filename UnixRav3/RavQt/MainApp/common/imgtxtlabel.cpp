#include <QPainter>
#include <QMouseEvent>

#include <QDebug>

#include "common/pubdef.h"
#include "imgtxtlabel.h"

ImgTxtLabel::ImgTxtLabel(QWidget *parent)
    :QPushButton(parent),m_ppImage(NULL)
{
    setCursor(Qt::PointingHandCursor);
    setFixedSize(48, 15);

    m_bDisabled = false;
    m_bEntered = false;
    m_bSelected = false;
    m_bMousePressed = false;

    m_ppImage = new QPixmap;
    bool bret = m_ppImage->load(":/resource/drop_down_btn");
    if(!bret){
//        qDebug("ImgTxtLabel load pixmap sImage fail");
    }
}

ImgTxtLabel::~ImgTxtLabel()
{
    if(m_ppImage) {
        delete m_ppImage;
    }
}

void ImgTxtLabel::SetText(QString sText)
{
    m_sText = sText;
    update();
}

void ImgTxtLabel::SetDisable(bool bDisable)
{
    m_bDisabled = bDisable;
    update();
}

void ImgTxtLabel::SetSelected(bool bSelected)
{
    m_bSelected = bSelected;
    update();

}

void ImgTxtLabel::enterEvent(QEvent *)
{
    m_bEntered = true;
    update();
}

void ImgTxtLabel::leaveEvent(QEvent *)
{
    m_bEntered = false;
    update();
}

void ImgTxtLabel::mouseMoveEvent(QMouseEvent *)
{

}

void ImgTxtLabel::mousePressEvent(QMouseEvent *pEvent)
{
    if(pEvent->button() == Qt::LeftButton)
    {
        m_bMousePressed = true;
        update();
    }
}

void ImgTxtLabel::mouseReleaseEvent(QMouseEvent *)
{
    if(m_bMousePressed)
    {
        m_bMousePressed = false;
//        if(m_bDisabled == false){
//            m_bSelected = true;
//        }
//        update();
        emit clicked();
    }
}

void ImgTxtLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QFont font;
    font.setPixelSize(10);
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#ifdef QT_NEW_HEADER_54
    font.setWeight(12);
#else
    font.setWeight(25);
#endif
#else //QT_VERSION
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::ExtraLight);
#else
    font.setWeight(QFont::Light);
#endif
#endif //QT_VERSION
    painter.setFont(font);
    if(m_bSelected){
        //setStyleSheet("background-color:rgb(123,89,159)");
        painter.fillRect(0,0,width(),height(),QColor(123,89,159));
        painter.setBrush(Qt::white);
    }else{
        if(m_bEntered){
            painter.drawPixmap(34, 4, 13, 7, m_ppImage->copy(14, 0, 13, 7));
        }else{
           painter.drawPixmap(34, 4, 13, 7, m_ppImage->copy(0, 0, 13, 7));
        }
        painter.setBrush(Qt::black);
    }
    painter.drawText(2, 11, m_sText);
}
