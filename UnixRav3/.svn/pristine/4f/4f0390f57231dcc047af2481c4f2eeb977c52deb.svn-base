#include <QPainter>
#include <QToolTip>
#include <QHelpEvent>
#include <QListWidget>

#include <QDebug>

#include "imgtipslabel.h"

ImgTipsLabel::ImgTipsLabel()
{

}


ImgTipsLabel::ImgTipsLabel(QString sIcoImage, QString sTipsImage, ImgLabelType eType)
{
    m_bEntered = false;
    m_bLeave = true;
    m_bTipsEventOccur = false;
    m_eType = eType;
    //setStyleSheet("QPushButton{ text-align : bottom; }");

    LoadImage(sIcoImage, sTipsImage);

    setFixedSize(m_ppIcoImage->width()/2, m_ppIcoImage->height());
    setMouseTracking(true);
    //setAutoFillBackground(true);
}

//bool ImgTipsLabel::event(QEvent *pEvent)
//{
//    if (pEvent->type() == QEvent::ToolTip) {
//         if(QRect(0,0,m_ppIcoImage->width()/2,m_ppIcoImage->height()).contains(this->mapFromGlobal(QCursor::pos()))){
//             //QPoint pt = mapToGlobal(QPoint(geometry().x()-20, geometry().y()-20));
//             m_bTipsEventOccur = true;
//             //QToolTip::showText(QPoint(pt.x()-20, pt.y()-20), m_sTips);
//             qDebug() << "event: show tips";
//         } else {
//             //QToolTip::hideText();
//             m_bTipsEventOccur = false;
//             pEvent->ignore();
//         }
//         update();
//         return true;
//     }
//     return QWidget::event(pEvent);
//}

void ImgTipsLabel::enterEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    m_bEntered = true;
    m_bLeave = false;
    update();
    //qDebug() << "ImgTipsLabel enterEvent";
}

void ImgTipsLabel::leaveEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    m_bEntered = false;
    m_bLeave = true;
    update();
    //qDebug() << "leaveEvent";
}

void ImgTipsLabel::enterTipsChange(QVariant qv)
{
    Q_UNUSED(qv);
    //m_nCenterImgIndex = qv.toInt();
    //qDebug("enterImageChange event, %d", m_nCenterImgIndex);
    //if(m_nCenterImgIndex >= 14){
    //    m_ppaAnimination->stop();
    //}
    //update();
}

void ImgTipsLabel::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);

    QPainter painter(this);
    //QRect pos = rect();
    QPoint pt = mapToGlobal(QPoint(geometry().x()-10, geometry().y()-10));

    if(m_bEntered){
        painter.drawPixmap(0, 0, m_ppIcoImage->width()/2, m_ppIcoImage->height(), m_ppIcoImage->copy(m_ppIcoImage->width()/2,0,m_ppIcoImage->width()/2,m_ppIcoImage->height()));
    }
    if(m_bLeave){
        painter.drawPixmap(0, 0, m_ppIcoImage->width()/2, m_ppIcoImage->height(), m_ppIcoImage->copy(0,0,m_ppIcoImage->width()/2,m_ppIcoImage->height()));
    }
    switch(m_eType){
    case IMG_LABEL_TYPE_BASIC_ENGINE:
        //painter.drawPixmap(5, m_ppIcoImage->height()-10, m_ppTextImage->width(), m_ppTextImage->height()/4, m_ppTextImage->copy(0,0,m_ppTextImage->width(),m_ppTextImage->height()/4));
        if(m_bTipsEventOccur){
            painter.drawPixmap(pt.x(),pt.y(),m_ppTipsImage->width(),m_ppTipsImage->height()/4,m_ppTipsImage->copy(0,0,m_ppTipsImage->width(),m_ppTipsImage->height()/4));
            QPainter ppainter(this->parentWidget());
        }
        break;
    case IMG_LABEL_TYPE_ADMIN_ENGINE:
        //painter.drawPixmap(0, m_ppIcoImage->height()-10, m_ppTextImage->width(), m_ppTextImage->height()/4, m_ppTextImage->copy(0,m_ppTextImage->height()/4,m_ppTextImage->width(),m_ppTextImage->height()/4));
        if(m_bTipsEventOccur){
            painter.drawPixmap(m_ppIcoImage->width(),0,m_ppTipsImage->width(),m_ppTipsImage->height()/4,m_ppTipsImage->copy(0,m_ppTipsImage->height()/4,m_ppTipsImage->width(),m_ppTipsImage->height()/4));
        }
        break;
    case IMG_LABEL_TYPE_CLOUD_ENGINE:
        //painter.drawPixmap(5, m_ppIcoImage->height()-10, m_ppTextImage->width(), m_ppTextImage->height()/4, m_ppTextImage->copy(0,m_ppTextImage->height()/2,m_ppTextImage->width(),m_ppTextImage->height()/4));
        if(m_bTipsEventOccur){
            painter.drawPixmap(m_ppIcoImage->width(),0,m_ppTipsImage->width(),m_ppTipsImage->height()/4,m_ppTipsImage->copy(0,m_ppTipsImage->height()/2,m_ppTipsImage->width(),m_ppTipsImage->height()/4));
        }
        break;
    case IMG_LABLE_TYPE_GENE_ENGINE:
        //painter.drawPixmap(5, m_ppIcoImage->height()-10, m_ppTextImage->width(), m_ppTextImage->height()/4, m_ppTextImage->copy(0,m_ppTextImage->height()/4*3,m_ppTextImage->width(),m_ppTextImage->height()/4));
        if(m_bTipsEventOccur){
            painter.drawPixmap(m_ppIcoImage->width(),0,m_ppTipsImage->width(),m_ppTipsImage->height()/4,m_ppTipsImage->copy(0,m_ppTipsImage->height()/4*3,m_ppTipsImage->width(),m_ppTipsImage->height()/4));
        }
        break;
    }
}

int ImgTipsLabel::LoadImage(QString sIcoImage, QString sTipsImage)
{
    m_ppIcoImage = new QPixmap;
    bool bret = m_ppIcoImage->load(sIcoImage);
    if(!bret){
//        qDebug("TmgTipsLabel::LoadImage sIcoImage fail");
    }

    m_ppTipsImage = new QPixmap;
    bret = m_ppTipsImage->load(sTipsImage);
    if(!bret){
//        qDebug("ImgTipsLabel::LoadImage sTipsImage fail");
    }
//    m_ppaAnimination = new QPropertyAnimation(this, "");
//    m_ppaAnimination->setStartValue(0);
//    m_ppaAnimination->setEndValue(14);
//    m_ppaAnimination->setDuration(600);
//    connect(m_ppaAnimination, SIGNAL(valueChanged(QVariant)), this, SLOT(enterImageChange(QVariant)));
    return 0;
}
