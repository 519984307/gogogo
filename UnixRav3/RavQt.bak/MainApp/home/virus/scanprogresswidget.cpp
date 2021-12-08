#include <string>
#include <sstream>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextCodec>

#include <stdio.h>

#include "showbutton.h"
#include "common/scanpausebtn.h"
#include "common/himagebutton.h"
#include "scanprogresscircle.h"
#include "scanprogresswidget.h"


using namespace std;

ScanProgressWidget::ScanProgressWidget(QWidget *parent) : QWidget(parent)
{
}

void ScanProgressWidget::paintEvent(QPaintEvent *)
{
    QPainter paint(this);

    paint.setPen(QPen(QColor(245,245,245), 1, Qt::SolidLine));
    paint.drawEllipse(10, 0, this->width()-20, this->height()-20);
}

/** scan normal widget */
ScanNormalWidget::ScanNormalWidget(QWidget *parent):QWidget(parent)
{
    mNormalIcon = new ShowButton;
    mNormalIcon->loadPixmap(":/resource/scan_normal_icon");

    mLabel = new QLabel(this);
//    QPalette palette;
//    palette.setColor(QPalette::Background, Qt::blue);
//    mLabel->setPalette(palette);
//    mLabel->setAutoFillBackground(true);
    mLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    //mLabel->setText(u8"瑞星杀毒正在保护您的电脑");
    mLabel->setText(u8("瑞星ESM正在保护您的电脑"));

    QFont ft;
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
    ft.setPixelSize(20);
#else
    ft.setPixelSize(32);
#endif
    mLabel->setFont(ft);
    mLabel->adjustSize();

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    mLabel->setPalette(pa);


    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addSpacing(70);
    layout->addWidget(mNormalIcon, 0, Qt::AlignLeft|Qt::AlignVCenter);
    layout->addSpacing(20);
    layout->addWidget(mLabel, 0, Qt::AlignLeft|Qt::AlignVCenter);
    layout->addStretch();
    setLayout(layout);
    //qDebug("ScanNornalWidget x=%d  y=%d   x=%d  y=%d\n", mNormalIcon->geometry().x(), mNormalIcon->geometry().y(), mLabel->geometry().x(), mLabel->geometry().y());

}

/** scan stopped widget */
ScanStoppedlWidget::ScanStoppedlWidget(QWidget *parent):QWidget(parent)
{
    mStoppedIcon = new ShowButton;
    mStoppedIcon->loadPixmap(":/resource/scan_normal_icon");

    mLabel = new QLabel(this);
    mLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    mLabel->setText(u8"未发现威胁");

    QFont ft;
    ft.setPixelSize(32);
    mLabel->setFont(ft);

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    mLabel->setPalette(pa);


    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addSpacing(70);
    layout->addWidget(mStoppedIcon, 0, Qt::AlignLeft);
    layout->addSpacing(10);
    layout->addWidget(mLabel, 0, Qt::AlignLeft);
    layout->addStretch();
    setLayout(layout);
    //qDebug("ScanStoppedlWidget x=%d  y=%d   x=%d  y=%d\n", mNormalIcon->geometry().x(), mNormalIcon->geometry().y(), mLabel->geometry().x(), mLabel->geometry().y());
}

void ScanStoppedlWidget::UpdateInfo(int nThreatens)
{
    if(nThreatens > 0) {
        mLabel->setText(u8"发现威胁");
    }else {
        mLabel->setText(u8"未发现威胁");
    }
}

/** scaning state widget */
ScaningStateWidget::ScaningStateWidget(QWidget *parent) : QWidget(parent)
{
    //setFixedWidth(parent->width());
    string stmp;
    m_nTotal = 0;
    m_nSpeed = 0;
    m_nThreaten = 0;
    m_nProcessed = 0;
    m_nUsingTime = 0;
//    m_pScaningIcon = new ShowButton();
//    m_pScaningIcon->loadPixmap(":/resource/scan_normal_icon");
    m_Circle = new ScanProgressCircle(this);
    m_nScaningState = RAV_QT_VSCAN_STATE_SCANING;
    m_pLabel = new QLabel();
    m_pLabel->setText(u8"正在进行全盘查杀");
    m_pLabel->adjustSize();
    QFont ft;
    ft.setPixelSize(24);
    m_pLabel->setFont(ft);
    m_pLabel->adjustSize();
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    m_pLabel->setPalette(pa);
    m_pPStaticText = new QPixmap();
    m_pPStaticText->load(":/resource/scan_state_cn_txt");
    lbl1 = new QLabel();
    lbl1->setPixmap(m_pPStaticText->copy(0, 0, m_pPStaticText->width(), m_pPStaticText->height()/5));
    lbl1->setFixedSize(m_pPStaticText->width(), m_pPStaticText->height()/5);
    m_pLTotal = new QLabel();
    stringstream   ostr;
    ostr << m_nTotal << u8"个" ;
    stmp = ostr.str();
    m_pLTotal->setPalette(pa);
    m_pLTotal->setText(stmp.c_str());
    m_pLTotal->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    m_pLTotal->adjustSize();
    lbl2 = new QLabel();
    lbl2->setPixmap(m_pPStaticText->copy(0, m_pPStaticText->height()/5, m_pPStaticText->width(), m_pPStaticText->height()/5));
    lbl2->setFixedSize(m_pPStaticText->width(), m_pPStaticText->height()/5);
    m_pLSpeed = new QLabel();
    stringstream ospeed;
    ospeed << m_nSpeed << u8"个/秒";
    m_pLSpeed->setPalette(pa);
    m_pLSpeed->setText(ospeed.str().c_str());
    m_pLSpeed->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    m_pLSpeed->adjustSize();
    lbl3 = new QLabel();
    lbl3->setPixmap(m_pPStaticText->copy(0, m_pPStaticText->height()/5*2, m_pPStaticText->width(), m_pPStaticText->height()/5));
    lbl3->setFixedSize(m_pPStaticText->width(), m_pPStaticText->height()/5);
    m_pLThreaten = new QLabel();
    stringstream othreaten;
    othreaten << m_nThreaten << u8"个" ;
    m_pLThreaten->setText(othreaten.str().c_str());
    m_pLThreaten->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    m_pLThreaten->adjustSize();
    pa.setColor(QPalette::WindowText,Qt::red);
    m_pLThreaten->setPalette(pa);
    lbl4 = new QLabel();
    lbl4->setPixmap(m_pPStaticText->copy(0, m_pPStaticText->height()/5*3, m_pPStaticText->width(), m_pPStaticText->height()/5));
    lbl4->setFixedSize(m_pPStaticText->width(), m_pPStaticText->height()/5);
    m_pLProcessed = new QLabel();
    stringstream oprocessed;
    oprocessed << m_nProcessed << u8"个" ;
    pa.setColor(QPalette::WindowText,Qt::white);
    m_pLProcessed->setPalette(pa);
    m_pLProcessed->setText(oprocessed.str().c_str());
    m_pLProcessed->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    m_pLProcessed->adjustSize();
    lbl5 = new QLabel();
    lbl5->setPixmap(m_pPStaticText->copy(0, m_pPStaticText->height()/5*4, m_pPStaticText->width(), m_pPStaticText->height()/5));
    lbl5->setFixedSize(m_pPStaticText->width(), m_pPStaticText->height()/5);
    m_pLUsingTime = new QLabel();
    stringstream otime;
    otime << u8"00:00:00" ;
    m_pLUsingTime->setPalette(pa);
    m_pLUsingTime->setText(otime.str().c_str());
    m_pLUsingTime->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    m_pLUsingTime->adjustSize();
    m_pPauseBtn = new ScanPauseBtn;
    m_pStopBtn = new HImageButton(":/resource/scan_cancle_btn");

    progressbar = new QProgressBar;
    progressbar->setFixedSize(870, 7);
    progressbar->setStyleSheet("QProgressBar{height:30;background:rgb(85,44,117);color:rgb(0,255,0);}");
    progressbar->setTextVisible(false);
    progressbar->setRange(0, 100);
    progressbar->setValue(0);
    progressbar->setOrientation(Qt::Horizontal);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->setSpacing(0);
    hlayout->addSpacing(70);
    hlayout->addWidget(m_Circle, 0, Qt::AlignVCenter|Qt::AlignLeft);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setSpacing(0);
    vlayout->addSpacing(30);
    //vlayout->addWidget(m_pLabel, 0, Qt::AlignTop|Qt::AlignLeft);
    QHBoxLayout *halayout = new QHBoxLayout;
    halayout->setContentsMargins(0, 0, 0, 0);
    halayout->addWidget(m_pLabel, 0, Qt::AlignTop|Qt::AlignLeft);
    halayout->addStretch();
    halayout->addWidget(m_pPauseBtn, 0, Qt::AlignTop|Qt::AlignLeft);
    halayout->addSpacing(18);
    halayout->addWidget(m_pStopBtn, 0, Qt::AlignTop|Qt::AlignLeft);
    halayout->addSpacing(60);
    vlayout->addLayout(halayout);
    QHBoxLayout *hblayout = new QHBoxLayout;
    hblayout->setContentsMargins(0, 0, 0, 0);
    hblayout->addWidget(lbl1, 0, Qt::AlignVCenter|Qt::AlignLeft);
    hblayout->addWidget(m_pLTotal, 0, Qt::AlignVCenter|Qt::AlignLeft);
    hblayout->addSpacing(5);
    hblayout->addWidget(lbl2, 0, Qt::AlignVCenter|Qt::AlignLeft);
    hblayout->addWidget(m_pLSpeed, 0, Qt::AlignVCenter|Qt::AlignLeft);
    hblayout->addSpacing(5);
    hblayout->addWidget(lbl3, 0, Qt::AlignVCenter|Qt::AlignLeft);
    hblayout->addWidget(m_pLThreaten, 0, Qt::AlignVCenter|Qt::AlignLeft);
    hblayout->addSpacing(5);
    hblayout->addWidget(lbl4, 0, Qt::AlignVCenter|Qt::AlignLeft);
    hblayout->addWidget(m_pLProcessed, 0, Qt::AlignVCenter|Qt::AlignLeft);
    hblayout->addSpacing(5);
    hblayout->addWidget(lbl5, 0, Qt::AlignVCenter|Qt::AlignLeft);
    hblayout->addWidget(m_pLUsingTime, 0, Qt::AlignVCenter|Qt::AlignLeft);
    //hblayout->addStretch();
    hblayout->addSpacing(100);
    //hblayout->addWidget(m_pStopBtn, 0, Qt::AlignVCenter|Qt::AlignRight);
    //hblayout->addSpacing(10);
    vlayout->addLayout(hblayout);
    hlayout->addSpacing(20);
    hlayout->addLayout(vlayout);
    hlayout->addSpacing(60);
    QVBoxLayout *vtlayout = new QVBoxLayout;
//    vtlayout->setMargin(0);
    vtlayout->setContentsMargins(0, 0, 0, 0);
    vtlayout->setSpacing(0);
    vtlayout->addLayout(hlayout);
    vtlayout->addWidget(progressbar);

    setLayout(vtlayout);
    setAutoFillBackground(true);
    //layout()->setSizeConstraint(QLayout::SetFixedSize);
    //qDebug("ScaningStateWidget::ScaningStateWidget w=%d h=%d", width(), height());
}

void ScaningStateWidget::UpdateScanInfo(int nTotal, int nSpeed, int nThreatens, int nProccessed, int nUsingTimes)
{
    m_nTotal = nTotal;
    m_nSpeed = nSpeed;
    m_nThreaten = nThreatens;
    m_nProcessed = nProccessed;
    m_nUsingTime = nUsingTimes;
    UpdateInfo();
}

void ScaningStateWidget::UpdateScaningState(int nState)
{
    QString stxt;
    QString alltxt;
    m_nPrevScaningState = m_nScaningState;
    m_nScaningState = nState;
    if(nState == RAV_QT_VSCAN_STATE_PAUSED || nState == RAV_QT_VSCAN_STATE_READY_TO_STOP){
        switch(m_nPrevScaningState){
        case RAV_QT_VSCAN_STATE_SCANING:
            stxt = u8"全盘查杀";
            break;
        case RAV_QT_VSCAN_STATE_QUICK_SCANING:
            stxt = u8"快速查杀";
            break;
        case RAV_QT_VSCAN_STATE_CUSTOM_SCANING:
            stxt = u8"自定义查杀";
            break;
        default:
            break;
        }
    }
    m_Circle->SetWarnLevel(ScanProgressCircle::NORMAL);
    switch(nState){
    case RAV_QT_VSCAN_STATE_SCANING:
        m_Circle->SetCircleType(ScanProgressCircle::ALL_KILL);
        m_Circle->StartMotion();
        m_pLabel->setText(u8"正在进行全盘查杀");
        break;
    case RAV_QT_VSCAN_STATE_QUICK_SCANING:
        m_Circle->SetCircleType(ScanProgressCircle::FAST_KILL);
        m_Circle->StartMotion();
        m_pLabel->setText(u8"正在进行快速查杀");
        break;
    case RAV_QT_VSCAN_STATE_CUSTOM_SCANING:
        m_Circle->SetCircleType(ScanProgressCircle::CUSTOM_KILL);
        m_Circle->StartMotion();
        m_pLabel->setText(u8"正在进行自定义查杀");
        break;
    case RAV_QT_VSCAN_STATE_READY:
        m_pLabel->setText(u8"杀毒引擎准备中...");
        m_Circle->StartMotion();
        break;
    case RAV_QT_VSCAN_STATE_PAUSED:
        alltxt = stxt + u8"已暂停";
        m_pLabel->setText(alltxt);
        break;
    case RAV_QT_VSCAN_STATE_READY_TO_STOP:
        alltxt = u8"正在停止" + stxt;
        m_pLabel->setText(alltxt);
        break;
    default:
        break;
    }
}

void ScaningStateWidget::UpdateScaningProgress(int progress)
{
    if(m_Circle == NULL || progressbar == NULL) {
        return;
    }
    //qDebug("vvvvvvvvvvvvvvvvv %d", progress);
    m_Circle->SetProgress(progress);
    progressbar->setValue(progress);
}

void ScaningStateWidget::UpdateInfo()
{
    QString stext = QString("%1个").arg(m_nTotal, 0, 10);
    m_pLTotal->setText(stext);

    stext = QString("%1个/秒").arg(m_nSpeed, 0, 10);
    m_pLSpeed->setText(stext);

    stext = QString("%1个").arg(m_nThreaten, 0, 10);
    m_pLThreaten->setText(stext);

    stext = QString("%1个").arg(m_nProcessed, 0, 10);
    m_pLProcessed->setText(stext);

    stext = QString("%1:%2:%3").arg(m_nUsingTime/3600, 2, 10, QChar('0')).arg(m_nUsingTime%3600/60, 2, 10, QChar('0')).arg(m_nUsingTime%60, 2, 10, QChar('0'));
    //qDebug("ScaningStateWidget::UpdateInfo m_nTotal=%d \n", m_nTotal);
    m_pLUsingTime->setText(stext);

    //layout()->setSizeConstraint(QLayout::SetFixedSize);
}

//void ScaningStateWidget::resizeEvent(QResizeEvent *)
//{
//    //m_pScaningIcon->setFixedSize(height()*0.7, height()*0.7);
//}

/** virus scan widget */
ScanWidget::ScanWidget(QWidget *parent) : QWidget(parent)
{
    m_nVScanState = RAV_QT_VSCAN_STATE_NORMAL;

    mScanNormalWidget = new ScanNormalWidget(this);
    mScanNormalWidget->setGeometry(0, 0, width(), height());
    //qDebug("xxxxxxxxxxxxxx  ScanWidget::ScanWidget w:%d  h:%d", width(), height());
    mScaningWidget = new ScaningStateWidget(this);
    mScaningWidget->setGeometry(0, 0, width(), height());
    mScanStoppedWidget = new ScanStoppedlWidget(this);
    mScanStoppedWidget->setGeometry(0, 0, width(), height());
    SetNormalState();
    //SetScaningState();
    //qDebug("ScanWidget::ScanWidget w=%d h=%d", width(), height());
}

void ScanWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    mScanNormalWidget->setFixedSize(width(), height());
    //mScanNormalWidget->mLabel->setFixedWidth(500);
    mScanNormalWidget->setGeometry(0, 0, width(), height());
    //qDebug("xxxxxxxxxxxxxx  ScanWidget::resizeEvent w:%d  h:%d", width(), height());
    //qDebug("xxxxxxxxxxxxxx  ScanWidget::resizeEvent normalWidget w:%d  h:%d", mScanNormalWidget->width(), mScanNormalWidget->height());
    //qDebug("xxxxxxxxxxx ScanWidget::resizeEvent label w:%d  h:%d", mScanNormalWidget->mLabel->width(), mScanNormalWidget->mLabel->height());
    mScaningWidget->setFixedSize(width(), height());
    mScaningWidget->setGeometry(0, 0, width(), height());
    mScanStoppedWidget->setFixedSize(width(), height());
    mScanStoppedWidget->setGeometry(0, 0, width(), height());
    //qDebug("ScanWidget::resizeEvent w=%d h=%d NormalWidget:w=%d h=%d", width(), height(), mScanNormalWidget->width(), mScanNormalWidget->height());
}

void ScanWidget::SetNormalState()
{
    UpdateScanInfo(0, 0, 0, 0, 0);
    mScaningWidget->setHidden(true);
    mScanNormalWidget->setHidden(false);
    mScanStoppedWidget->setHidden(true);

    mScanNormalWidget->setFixedSize(width(), height());
    m_nVScanState = RAV_QT_VSCAN_STATE_NORMAL;
    //qDebug("ScanWidget::SetNormalState NormalWidget:w=%d h=%d", mScanNormalWidget->width(), mScanNormalWidget->height());
}

void ScanWidget::SetScaningState()
{
    mScanNormalWidget->setHidden(true);
    mScanStoppedWidget->setHidden(true);
    mScaningWidget->setHidden(false);
    mScaningWidget->UpdateScaningState(RAV_QT_VSCAN_STATE_SCANING);
    mScaningWidget->show();
    mScaningWidget->setFixedSize(width(), height());
    m_nVScanState = RAV_QT_VSCAN_STATE_SCANING;
}

void ScanWidget::SetQuickScaningState()
{
    mScanNormalWidget->setHidden(true);
    mScanStoppedWidget->setHidden(true);
    mScaningWidget->setHidden(false);
    mScaningWidget->UpdateScaningState(RAV_QT_VSCAN_STATE_QUICK_SCANING);
    mScaningWidget->show();
    mScaningWidget->setFixedSize(width(), height());
    m_nVScanState = RAV_QT_VSCAN_STATE_QUICK_SCANING;
}

void ScanWidget::SetCustomScaningState()
{
    mScanNormalWidget->setHidden(true);
    mScanStoppedWidget->setHidden(true);
    mScaningWidget->setHidden(false);
    mScaningWidget->UpdateScaningState(RAV_QT_VSCAN_STATE_CUSTOM_SCANING);
    mScaningWidget->show();
    mScaningWidget->setFixedSize(width(), height());
    m_nVScanState = RAV_QT_VSCAN_STATE_CUSTOM_SCANING;
}

void ScanWidget::SetPauseScaningState()
{
    mScanNormalWidget->setHidden(true);
    mScanStoppedWidget->setHidden(true);
    mScaningWidget->setHidden(false);
    mScaningWidget->UpdateScaningState(RAV_QT_VSCAN_STATE_PAUSED);
    mScaningWidget->show();
    mScaningWidget->setFixedSize(width(), height());
    m_nVScanState = RAV_QT_VSCAN_STATE_PAUSED;
}

void ScanWidget::SetReadyToStopState()
{
    mScanNormalWidget->setHidden(true);
    mScanStoppedWidget->setHidden(true);
    mScaningWidget->setHidden(false);
    mScaningWidget->UpdateScaningState(RAV_QT_VSCAN_STATE_READY_TO_STOP);
    mScaningWidget->show();
    mScaningWidget->setFixedSize(width(), height());
    m_nVScanState = RAV_QT_VSCAN_STATE_READY_TO_STOP;
}

void ScanWidget::SetStoppedState()
{
    mScaningWidget->setHidden(true);
    mScanNormalWidget->setHidden(true);
    mScanStoppedWidget->setHidden(false);
    mScanStoppedWidget->setFixedSize(width(), height());
    m_nVScanState = RAV_QT_VSCAN_STATE_STOPPED;
    update();
}

void ScanWidget::SetReadyState()
{
    mScaningWidget->setHidden(true);
    mScanNormalWidget->setHidden(true);
    mScanStoppedWidget->setHidden(false);
    mScanStoppedWidget->setFixedSize(width(), height());
    m_nVScanState = RAV_QT_VSCAN_STATE_READY;
    update();
}

void ScanWidget::UpdateScanInfo(int nTotal, int nSpeed, int nThreatens, int nProccessed, int nUsingTimes)
{
    if(mScaningWidget==NULL || mScanStoppedWidget==NULL) {
        return;
    }
    mScaningWidget->UpdateScanInfo(nTotal, nSpeed, nThreatens, nProccessed, nUsingTimes);
    mScanStoppedWidget->UpdateInfo(nThreatens);
}

void ScanWidget::UpdateScaningProgress(int progress)
{
    if(mScaningWidget == NULL) {
        return;
    }
    mScaningWidget->UpdateScaningProgress(progress);
}
