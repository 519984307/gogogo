#include <QPainter>


#include "bodywidget.h"
#include "common.h"


BodyWidget::BodyWidget(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(APP_BODY_HEIGHT);
//    vlayout = new QVBoxLayout;
//    vlayout->setMargin(0);
//    vlayout->setContentsMargins(0,0,0,0);
    m_pMainWindow = (MainWindow*)parent;

    m_pTopBody = new TopBodyWidget(this);
    m_pMidBody = new MidBodyWidget(this);
    m_pBotBody = new BotBodyWidget(this, m_pMainWindow);

    m_pScanTopBody = new ScanTopBodyWidget(this, !m_pMainWindow->IsSecretMachine());
    m_pScanTopBody->hide();
    m_pScanMidBody = new ScanMidBodyWidget(this);
    m_pScanMidBody->hide();
    m_pScanBotBody = new ScanBotBodyWidget(this);
    m_pScanBotBody->hide();
    m_pStoppedTopBody = new StoppedTopBodyWidget(this);
    m_pStoppedTopBody->hide();

    m_pLine = new QFrame(this);
    m_pLine->setObjectName(QString("line"));
    m_pLine->setFrameShape(QFrame::HLine);
    m_pLine->setFrameShadow(QFrame::Raised);
    //m_pLine->setFrameStyle(QFrame::Panel | QFrame::Raised);

    SetNormalState();
    m_nScanState = RAV_QT_VSCAN_STATE_NORMAL;
}

BodyWidget::~BodyWidget()
{

}

void BodyWidget::SetVScanState(int nState)
{
    switch(nState){
    case RAV_QT_VSCAN_STATE_NORMAL:
        SetNormalState();
        break;
    case RAV_QT_VSCAN_STATE_SCANING:
    case RAV_QT_VSCAN_STATE_QUICK_SCANING:
    case RAV_QT_VSCAN_STATE_CUSTOM_SCANING:
        SetScaningState();
        break;
    case RAV_QT_VSCAN_STATE_STOPPED:
        SetStopState();
        break;
    default:
        break;
    }
    m_nScanState = nState;
}

void BodyWidget::AddVirusLog(const string &sVName, const string &sType, const string &sPath, const string &sStatus)
{
    if(m_nScanState != RAV_QT_VSCAN_STATE_SCANING && m_nScanState != RAV_QT_VSCAN_STATE_QUICK_SCANING
            && m_nScanState != RAV_QT_VSCAN_STATE_CUSTOM_SCANING)
        return;
    m_pScanMidBody->AddVirusLog(sVName, sType, sPath, sStatus);
}

void BodyWidget::ResetVirusLog()
{
    if(m_pScanMidBody == NULL)
        return;
    m_pScanMidBody->ResetVirusLog();
    m_pScanTopBody->ResetInfo();
}

void BodyWidget::UpdateScanInfo(int nThreads, int nLocalThreaten, int nCloudThreaten, QString &sFile)
{
    m_pScanTopBody->UpdateScanInfo(nThreads, nLocalThreaten, nCloudThreaten, sFile);
}

void BodyWidget::UpdateThreadInfo(int tag, int count, string &filename)
{
    m_pScanMidBody->UpdateThreadInfo(tag, count, filename);
}

void BodyWidget::UpdateScaningThreadCount(int count)
{
    m_pScanMidBody->UpdateScaningThreadCount(count);
}

void BodyWidget::UpdateStoppedInfo(int nThreatens, int nCount, int nUsingTime)
{
    m_pStoppedTopBody->UpdateStoppedInfo(nThreatens, nCount, nUsingTime);
}

void BodyWidget::SetNormalState()
{
    m_pStoppedTopBody->hide();
    m_pScanTopBody->hide();
    m_pScanMidBody->ResetVirusLog();
    m_pScanMidBody->hide();
    m_pScanBotBody->hide();

    m_pLine->setObjectName(QString("line"));
    m_pLine->setFrameShape(QFrame::HLine);
    m_pLine->setFrameShadow(QFrame::Raised);

    m_pTopBody->setGeometry(0, 0, APP_SCREEN_WIDTH, m_pTopBody->height());
    m_pTopBody->show();
    m_pMidBody->setGeometry(0, m_pTopBody->height(), 920, m_pMidBody->height());
    m_pMidBody->show();;
    m_pBotBody->setGeometry(0, m_pTopBody->height()+m_pMidBody->height(), 920, m_pBotBody->height());
    m_pBotBody->show();

    //qDebug("line:x,y,w,h:%d  %d  %d  %d", m_pLine->x(), m_pLine->y(), m_pLine->width(), m_pLine->height());

    m_nScanState = RAV_QT_VSCAN_STATE_NORMAL;
}

void BodyWidget::SetScaningState()
{
    ResetVirusLog();
    m_pStoppedTopBody->hide();
    m_pTopBody->hide();
    m_pMidBody->hide();
    m_pBotBody->hide();

    m_pLine->setObjectName(QString("line"));
    m_pLine->setFrameShape(QFrame::HLine);
    m_pLine->setFrameShadow(QFrame::Raised);
    //m_pLine->setFrameStyle(QFrame::Panel | QFrame::Raised);

    m_pScanTopBody->setGeometry(0, 0, APP_SCREEN_WIDTH, m_pScanTopBody->height());
    m_pScanTopBody->show();
    m_pScanMidBody->setGeometry(0, m_pScanTopBody->height(), APP_SCREEN_WIDTH, m_pScanMidBody->height());
    m_pScanMidBody->show();
    m_pScanBotBody->setGeometry(0, this->height()-m_pScanBotBody->height(), APP_SCREEN_WIDTH, m_pScanBotBody->height());
    m_pScanBotBody->show();
    m_pScanMidBody->SetScaningState();

    m_nScanState = RAV_QT_VSCAN_STATE_SCANING;
}

void BodyWidget::SetStopState()
{
    m_pScanTopBody->hide();
    m_pTopBody->hide();
    m_pMidBody->hide();
    m_pBotBody->hide();

    m_pLine->setObjectName(QString("line"));
    m_pLine->setFrameShape(QFrame::HLine);
    m_pLine->setFrameShadow(QFrame::Raised);

    m_pStoppedTopBody->setGeometry(0, 0, APP_SCREEN_WIDTH, m_pStoppedTopBody->height());
    m_pStoppedTopBody->show();
    m_pScanMidBody->SetStoppedState();
    m_pScanMidBody->setGeometry(0, m_pStoppedTopBody->height(), APP_SCREEN_WIDTH, m_pScanMidBody->height());
    m_pScanMidBody->show();
    m_pScanBotBody->setGeometry(0, this->height()-m_pScanBotBody->height(), APP_SCREEN_WIDTH, m_pScanBotBody->height());
    m_pScanBotBody->show();

    //qDebug("line:x,y,w,h:%d  %d  %d  %d", m_pLine->x(), m_pLine->y(), m_pLine->width(), m_pLine->height());

    m_nScanState = RAV_QT_VSCAN_STATE_STOPPED;
}

void BodyWidget::setBackgroud(QString image)
{
    setAutoFillBackground(true);
    QPixmap pixmap = QPixmap(image).scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(this->backgroundRole(), QBrush(pixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

    this->setPalette(palette);
    //m_pLine->setFixedWidth(this->width()-20);
    m_pLine->setGeometry(QRect(this->x()+5, this->height()-50, this->width()-10, 3));
    //m_pLine->raise();
    //QRect geo = geometry();
    //qDebug("body geo x,y,w,h:%d  %d  %d  %d", geo.x(), geo.y(), geo.width(), geo.height());
    //qDebug("body x,y,w,h:%d  %d  %d  %d", this->x(), this->y(), this->width(), this->height());
    //qDebug("line:x,y,w,h:%d  %d  %d  %d", m_pLine->x(), m_pLine->y(), m_pLine->width(), m_pLine->height());
}

void BodyWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    setBackgroud(":/resource/body_bg");
}
