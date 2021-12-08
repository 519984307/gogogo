#include <QHBoxLayout>
#include <QPainter>
#include <sstream>
#include <string>
#include <stdio.h>
#include <unistd.h>

#include "stoppedtopbodywidget.h"
#include "common.h"

using namespace std;

StoppedTopBodyWidget::StoppedTopBodyWidget(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(APP_SCAN_TOP_BODY_HEIGHT);
    m_nThreatens = 0;
    m_nItemCount = 0;

    lbl1 = new QLabel(u8"共发现威胁：");
    lbl1->adjustSize();
    stringstream othreads;
    othreads << m_nThreatens << u8"个";
    m_pLThreatens = new QLabel(othreads.str().c_str());
    m_pLThreatens->adjustSize();

    lbl2 = new QLabel(u8"共扫描对象：");
    lbl2->adjustSize();
    stringstream ocount;
    ocount << m_nItemCount << u8"个";
    m_pLItemsCount = new QLabel(ocount.str().c_str());
    m_pLItemsCount->adjustSize();


    lbl3 = new QLabel(u8"共计用时：");
    lbl3->adjustSize();
    stringstream otimes;
    otimes << "00:00:00";
    m_pLUsingTime = new QLabel(otimes.str().c_str());
    m_pLUsingTime->adjustSize();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addSpacing(50);
    layout->addWidget(lbl1, 0, Qt::AlignVCenter|Qt::AlignLeft);
    layout->addWidget(m_pLThreatens, 0, Qt::AlignVCenter|Qt::AlignLeft);
    layout->addSpacing(70);
    layout->addWidget(lbl2, 0, Qt::AlignVCenter|Qt::AlignLeft);
    layout->addWidget(m_pLItemsCount, 0, Qt::AlignVCenter|Qt::AlignLeft);
    layout->addSpacing(70);
    layout->addWidget(lbl3, 0, Qt::AlignVCenter|Qt::AlignLeft);
    layout->addWidget(m_pLUsingTime, 0, Qt::AlignVCenter|Qt::AlignLeft);
    layout->addSpacing(70);
    layout->addStretch();
    setLayout(layout);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void StoppedTopBodyWidget::UpdateInfo()
{
    stringstream othreads;
    othreads << m_nThreatens << u8"个";
    m_pLThreatens->setText(othreads.str().c_str());

    stringstream ocount;
    ocount << m_nItemCount << u8"个";
    m_pLItemsCount->setText(ocount.str().c_str());

    QString stmp;
    stmp.sprintf("%02d:%02d:%02d", m_nUsingTimes/3600, m_nUsingTimes%3600/60, m_nUsingTimes%60);
    m_pLUsingTime->setText(stmp);

    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
    update();
}

StoppedTopBodyWidget::~StoppedTopBodyWidget()
{

}

void StoppedTopBodyWidget::UpdateStoppedInfo(int nThreatens, int nCount, int nUsingTime)
{
    m_nThreatens = nThreatens;
    m_nItemCount = nCount;
    m_nUsingTimes = nUsingTime;
    UpdateInfo();
}

void StoppedTopBodyWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    //setBackgroud(":/resource/body_bg");
}
