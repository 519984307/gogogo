#include <QPainter>
#include <QMouseEvent>

#include <QDebug>

#include "common/pubdef.h"
#include "imgtxtlabel.h"


//ImgTxtLabel::ImgTxtLabel(QWidget *parent, Qt::WindowFlags f)
ImgTxtLabel::ImgTxtLabel(QWidget *parent, GlobalConf *pConf)
    :QPushButton(parent),m_ppImage(NULL)
{
    m_pConf = pConf;
    setCursor(Qt::PointingHandCursor);
    setFixedSize(APP_SCREEN_LEFT_WIDTH-1, IMG_TXT_LABEL_HEIGHT);

    m_bDisabled = false;
    m_bEntered = false;
    m_bSelected = false;
    m_bMousePressed = false;
}

void ImgTxtLabel::LoadPixmap(QString sImage)
{
    m_ppImage = new QPixmap;
    bool bret = m_ppImage->load(sImage);
    if(!bret){
        qDebug("ImgTxtLabel::LoadPixmap sImage fail");
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
        m_pConf->m_pLeftClickedLable = this;
        emit clicked();
    }
}

void ImgTxtLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(m_ppImage != NULL){
#ifdef QT_NEW_HEADER_54
        QFont font("System", 11, QFont::ExtraLight, false);
#else
        QFont font("System", 11, QFont::Light, false);
#endif
        if(m_bDisabled){
            //setStyleSheet("background-color:rgb(248,248,248)");
            painter.fillRect(0,0,width(),height(),QColor(248,248,248));
        }else{
            if(m_bEntered){
                //setStyleSheet("background-color:rgb(212,205,217)");
                painter.fillRect(0,0,width(),height(),QColor(212,205,217));
            }else{
                //setStyleSheet("background-color:rgb(235,235,235)");
                painter.fillRect(0,0,width(),height(),QColor(235,235,235));
            }
        }
        painter.drawPixmap(8,(height()-m_ppImage->height())/2,m_ppImage->width(),m_ppImage->height(),*m_ppImage);
        painter.setFont(font);
        painter.setBrush(Qt::black);
        painter.drawText(m_ppImage->width()+10,11+(height()-11)/2, m_sText);
    }else{
#ifdef QT_NEW_HEADER_54
        QFont font("System", 10, QFont::ExtraLight, false);
#else
        QFont font("System", 10, QFont::Light, false);
#endif
        painter.setFont(font);
        if(m_bSelected){
            //setStyleSheet("background-color:rgb(123,89,159)");
            painter.fillRect(0,0,width(),height(),QColor(123,89,159));
            painter.setBrush(Qt::white);
        }else{
            if(m_bEntered){
                //setStyleSheet("background-color:rgb(222,215,219)");
                painter.fillRect(0,0,width(),height(),QColor(222,215,219));
            }else{
                //setStyleSheet("background-color:rgb(248,248,248)");
                painter.fillRect(0,0,width(),height(),QColor(248,248,248));
            }
            painter.setBrush(Qt::black);
        }
        painter.drawText(43, 10+(height()-10)/2, m_sText);
    }
    //painter.fillRect(0,0,width(),height()-2, Qt::red);
}
