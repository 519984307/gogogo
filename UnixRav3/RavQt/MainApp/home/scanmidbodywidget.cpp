#include <QPainter>
#include <QToolTip>
#include <QModelIndex>
#include <QEvent>
#include <QHelpEvent>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
//#include <QGuiApplication>
#include <QVBoxLayout>

#include <QDebug>

#include "common/pushbutton.h"
#include "common/scanresulttable.h"
#include "common/imagebutton.h"
#include "scanchart.h"
#include "scanmidbodywidget.h"


ScanMidBodyWidget::ScanMidBodyWidget(QWidget *parent) : QWidget(parent)
{
    //setFixedHeight(APP_SCAN_MID_BODY_HEIGHT);
    setFixedHeight(280);
    setMouseTracking(true);
    //setAutoFillBackground(true);
    //connect(this, SIGNAL(entered(QModelIndex)), m_pBasicEngine, SLOT(showToolTip(QModelIndex)));
    m_pTable = new ScanResultTable(this);
    m_pTable->setFixedSize(630, 270);
    m_pChart1 = new ScanChart(1, this);
    m_pChart2 = new ScanChart(2, this);
    m_pChart3 = new ScanChart(3, this);
    m_pChart4 = new ScanChart(4, this);

    m_pChart1->setGeometry(10, 0, m_pChart1->width(), m_pChart1->height());
    m_pChart2->setGeometry(10, 70, m_pChart2->width(), m_pChart2->height());
    m_pChart3->setGeometry(10, 140, m_pChart3->width(), m_pChart3->height());
    m_pChart4->setGeometry(10, 210, m_pChart4->width(), m_pChart4->height());
    m_pTable->setGeometry(240, 0,  200, 750);

    m_pBtnQuit = new ImageButton(":/resource/page_close_btn", this);
    m_pBtnQuit->setGeometry(815, 0, 50, 50);
    m_pBtnQuit->hide();
}

ScanMidBodyWidget::~ScanMidBodyWidget()
{
}

void ScanMidBodyWidget::SetStoppedState()
{
    m_pBtnQuit->show();
}
void ScanMidBodyWidget::SetScaningState()
{
    m_pBtnQuit->hide();
}

void ScanMidBodyWidget::UpdateThreadInfo(int tag, int count, string &filename)
{
    if (tag < mScanThreadCount) {
        switch(tag){
        case 0:
            if(m_pChart1->isHidden() || m_pChart1->IsStopped()) {
                m_pChart1->show();
                m_pChart1->Start();
            }
            m_pChart1->UpdateData(count, filename);
            break;
        case 1:
            if(m_pChart2->isHidden() || m_pChart2->IsStopped()) {
                m_pChart2->show();
                m_pChart2->Start();
            }
            m_pChart2->UpdateData(count, filename);
            break;
        case 2:
            if(m_pChart3->isHidden() || m_pChart3->IsStopped()) {
                m_pChart3->show();
                m_pChart3->Start();
            }
            m_pChart3->UpdateData(count, filename);
            break;
        case 3:
            if(m_pChart4->isHidden() || m_pChart4->IsStopped()) {
                m_pChart4->show();
                m_pChart4->Start();
            }
            m_pChart4->UpdateData(count, filename);
            break;
        default:
            break;
        }
    }
}

void ScanMidBodyWidget::UpdateScaningThreadCount(int count)
{
    mScanThreadCount = count;
    switch(count){
    case 1:
        m_pChart1->show();
        m_pChart1->Start();
        m_pChart2->hide();
        m_pChart3->hide();
        m_pChart4->hide();
        break;
    case 2:
        m_pChart1->show();
        m_pChart1->Start();
        m_pChart2->show();
        m_pChart2->Start();
        m_pChart3->hide();
        m_pChart4->hide();
        break;
    case 3:
        m_pChart1->show();
        m_pChart1->Start();
        m_pChart2->show();
        m_pChart2->Start();
        m_pChart3->show();
        m_pChart3->Start();
        m_pChart4->hide();
        break;
    case 4:
        m_pChart1->show();
        m_pChart1->Start();
        m_pChart2->show();
        m_pChart2->Start();
        m_pChart3->show();
        m_pChart3->Start();
        m_pChart4->show();
        m_pChart4->Start();
        break;
    default:
        break;
    }
}

void ScanMidBodyWidget::AddVirusLog(const string &sVName, const string &sType, const string &sPath, const string &sStatus)
{
    if(m_pTable == NULL)
        return;
    m_pTable->AddItem(sVName.c_str(), sType.c_str(), sPath.c_str(), sStatus.c_str());
}

void ScanMidBodyWidget::ResetVirusLog()
{
    if(m_pTable == NULL)
        return;
    m_pTable->DelAllItems();
    m_pChart1->Reset();
    m_pChart2->Reset();
    m_pChart3->Reset();
    m_pChart4->Reset();
    update();
}
