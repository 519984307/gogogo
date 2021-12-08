#include <QHBoxLayout>
#include <QPainter>

#include "common.h"
#include "scantopbodywidget.h"
#include "scanmidbodywidget.h"
#include "scanbotbodywidget.h"
#include "scanbodywidget.h"


ScanBodyWidget::ScanBodyWidget(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(APP_BODY_HEIGHT);
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setMargin(0);
    vlayout->setContentsMargins(0,0,0,0);

    m_pTopBody = new ScanTopBodyWidget;
    m_pMidBody = new ScanMidBodyWidget(this);
    m_pBotBody = new ScanBotBodyWidget;

    m_pLine = new QFrame(this);
    m_pLine->setObjectName(QString("line"));
    m_pLine->setFrameShape(QFrame::HLine);
    m_pLine->setFrameShadow(QFrame::Raised);
    //m_pLine->setFrameStyle(QFrame::Panel | QFrame::Raised);

//    //hlayout->addSpacing(130);
    vlayout->addWidget(m_pTopBody);//, 0, QT::AlignBottom);
    vlayout->addWidget(m_pMidBody);
    //vlayout->addWidget(m_pLine);
    vlayout->addWidget(m_pBotBody);
    //qDebug("line:x,y,w,h:%d  %d  %d  %d", m_pLine->x(), m_pLine->y(), m_pLine->width(), m_pLine->height());
    this->setLayout(vlayout);
}

ScanBodyWidget::~ScanBodyWidget()
{

}

void ScanBodyWidget::setBackgroud(QString image)
{
    setAutoFillBackground(true);
    QPixmap pixmap = QPixmap(image).scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(this->backgroundRole(), QBrush(pixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

    this->setPalette(palette);
    //m_pLine->setFixedWidth(this->width()-20);
    m_pLine->setGeometry(QRect(this->x()+10, this->y()+181, this->width()-20, 3));
    //m_pLine->raise();
    //QRect geo = geometry();
    //qDebug("body geo x,y,w,h:%d  %d  %d  %d", geo.x(), geo.y(), geo.width(), geo.height());
    //qDebug("body x,y,w,h:%d  %d  %d  %d", this->x(), this->y(), this->width(), this->height());
    //qDebug("line:x,y,w,h:%d  %d  %d  %d", m_pLine->x(), m_pLine->y(), m_pLine->width(), m_pLine->height());
}

void ScanBodyWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    setBackgroud(":/resource/body_bg");
}
