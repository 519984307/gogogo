#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <sstream>
#include <string>

#include "common.h"
#include "animitedbutton.h"
#include "imgtxtlabel.h"
#include "scantopbodywidget.h"


using namespace std;

ScanTopBodyWidget::ScanTopBodyWidget(QWidget *parent, bool scanIntelligent) : QWidget(parent),lbl2(NULL),m_pModeSelector(NULL), mIntelligentMode(scanIntelligent)
{
    //setFixedHeight(APP_SCAN_TOP_BODY_HEIGHT);
    setFixedSize(APP_SCREEN_WIDTH, APP_SCAN_TOP_BODY_HEIGHT);
    //m_nThreads = 0;
    m_nMode = 0;
    m_nLocalThreatens = 0;
    m_nCloudThreatens = 0;

    //lbl1 = new QLabel(u8"正在扫描:");
    //lbl1->adjustSize();
    //m_pLFileName = new QLabel;
    //m_pLFileName->setText("");
    //m_pLFileName->adjustSize();

    if (mIntelligentMode) {
        lbl2 = new QLabel(u8"查杀模式：");
        lbl2->adjustSize();

        m_pModeSelector = new ImgTxtLabel;
        m_pModeSelector->SetText(u8"自动");
    }
//    m_pLModeTxt = new QLabel(u8"自动");
//    m_pLModeTxt->adjustSize();


    //m_pLModeImg = new QLabel(this);
    //m_pIDropDown = new QPixmap(":/resource/drop_down_btn");
    //m_pLModeImg->setPixmap(m_pIDropDown->copy(0,0,m_pIDropDown->width()/3, m_pIDropDown->height()));

    lbl3 = new QLabel(u8"本地引擎发现威胁：");
    lbl3->adjustSize();
    stringstream olocalthreaten;
    olocalthreaten << m_nLocalThreatens;
    m_pLLocalThreatens = new QLabel(olocalthreaten.str().c_str());
    m_pLLocalThreatens->adjustSize();

    lbl4 = new QLabel(u8"云发现威胁：");
    lbl4->adjustSize();
    stringstream ocloudthreaten;
    ocloudthreaten << m_nCloudThreatens;
    m_pLCloudThreatens = new QLabel(ocloudthreaten.str().c_str());
    m_pLCloudThreatens->adjustSize();

    QVBoxLayout *vlayout = new QVBoxLayout;
    //vlayout->addWidget(progressbar);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addSpacing(30);
    if (lbl2) {
        layout->addWidget(lbl2, 0, Qt::AlignVCenter|Qt::AlignLeft);
        //    layout->addWidget(m_pLModeTxt, 0, Qt::AlignVCenter|Qt::AlignLeft);
            layout->addWidget(m_pModeSelector, 0, Qt::AlignVCenter|Qt::AlignLeft);
    }

    layout->addSpacing(50);
    layout->addWidget(lbl3, 0, Qt::AlignVCenter|Qt::AlignLeft);
    layout->addWidget(m_pLLocalThreatens, 0, Qt::AlignVCenter|Qt::AlignLeft);
    layout->addSpacing(50);
    layout->addWidget(lbl4, 0, Qt::AlignVCenter|Qt::AlignLeft);
    layout->addWidget(m_pLCloudThreatens, 0, Qt::AlignVCenter|Qt::AlignLeft);
    layout->addStretch();
    vlayout->addLayout(layout);
    setLayout(vlayout);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void ScanTopBodyWidget::UpdateScanInfo(int nThreads, int nLocalThreaten, int nCloudThreaten, QString sFile)
{
    m_nThreads = nThreads;
    m_nLocalThreatens = nLocalThreaten;
    m_nCloudThreatens = nCloudThreaten;
    m_sFileName = sFile;
    UpdateInfo();
}

void ScanTopBodyWidget::ResetInfo()
{    UpdateScanInfo(0, 0, 0, "");
}

void ScanTopBodyWidget::UpdateInfo()
{
    //m_pLFileName->setText(m_sFileName);

    QString stext = QString("%1").arg(m_nLocalThreatens, 0, 10);
    m_pLLocalThreatens->setText(stext);

    stext = QString("%1").arg(m_nCloudThreatens, 0, 10);
    m_pLCloudThreatens->setText(stext);

    //this->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

ScanTopBodyWidget::~ScanTopBodyWidget()
{

}

void ScanTopBodyWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    //setBackgroud(":/resource/body_bg");
}
