#include <QHBoxLayout>
#include <QSignalMapper>
#include <QMouseEvent>

#include "headerwidget.h"
#include "titlewidget.h"
#include "virus/scanprogresswidget.h"
#include "defendstatewidget.h"

HeaderWidget::HeaderWidget(QWidget *parent) : QWidget(parent)
{
    m_nVScanState = RAV_QT_VSCAN_STATE_NORMAL;
    setFixedHeight(APP_HEADER_HEIGHT);
    setAutoFillBackground(false);

    mTitleView = new TitleWidget();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    layout->addWidget(mTitleView, 0, Qt::AlignLeft|Qt::AlignTop);
    mVirusScanWidget = new ScanWidget(this);
    //mVirusScanWidget->setFixedWidth();
    layout->addWidget(mVirusScanWidget);

    this->setLayout(layout);
}

HeaderWidget::~HeaderWidget()
{

}

void HeaderWidget::SetVScanState(int nState)
{
    switch(nState){
    case RAV_QT_VSCAN_STATE_NORMAL:
        mVirusScanWidget->UpdateScanInfo(0, 0, 0, 0, 0);
        mVirusScanWidget->UpdateScaningProgress(0);
        mVirusScanWidget->SetNormalState();
        break;
    case RAV_QT_VSCAN_STATE_SCANING:
        mVirusScanWidget->SetScaningState();
        break;
    case RAV_QT_VSCAN_STATE_QUICK_SCANING:
        mVirusScanWidget->SetQuickScaningState();
        break;
    case RAV_QT_VSCAN_STATE_CUSTOM_SCANING:
        mVirusScanWidget->SetCustomScaningState();
        break;
    case RAV_QT_VSCAN_STATE_PAUSED:
        mVirusScanWidget->SetPauseScaningState();
        break;
    case RAV_QT_VSCAN_STATE_READY_TO_STOP:
        mVirusScanWidget->SetReadyToStopState();
        break;
    case RAV_QT_VSCAN_STATE_STOPPED:
        mVirusScanWidget->SetStoppedState();
        break;
    case RAV_QT_VSCAN_STATE_READY:
        mVirusScanWidget->SetReadyState();
        break;
    default:
        break;
    }
}

int HeaderWidget::VScanState()
{
    return mVirusScanWidget->ScanState();
}

void HeaderWidget::UpdateScaningInfo(int nTotal, int nSpeed, int nThreatens, int nProccessed, int nUsingTimes)
{
    return mVirusScanWidget->UpdateScanInfo(nTotal, nSpeed, nThreatens, nProccessed, nUsingTimes);
}

void HeaderWidget::UpdateScaningProgress(int progress)
{
    mVirusScanWidget->UpdateScaningProgress(progress);
}

void HeaderWidget::setBackgroud(QString image)
{
    this->setAutoFillBackground(true);
    QPixmap pixmap = QPixmap(image).scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(this->backgroundRole(),
            QBrush(pixmap.scaled(this->size(),
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation)));
    this->setPalette(palette);
}

void HeaderWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    setBackgroud(":/resource/header_bg");
    mTitleView->setFixedSize(this->width()-APP_EDGE_WIDTH, APP_TITLE_HEIGHT);
    mVirusScanWidget->setFixedSize(width()-APP_EDGE_WIDTH, height()-APP_TITLE_HEIGHT);
    //qDebug("HeaderWidget::resizeEvent w=%d h=%d ", width(), height());

}



