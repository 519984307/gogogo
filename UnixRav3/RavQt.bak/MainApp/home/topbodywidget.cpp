#include <QHBoxLayout>
#include <QPainter>

#include "topbodywidget.h"
#include "common.h"

TopBodyWidget::TopBodyWidget(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(APP_TOP_BODY_HEIGHT);
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setMargin(0);
    hlayout->setContentsMargins(0,0,0,0);
    hlayout->setSpacing(80);
    hlayout->addStretch();
    m_pAllKillButton = new AnimitedButton(":/resource/hexagon_small_btn", ":/resource/kill_all_frames", u8"全盘查杀");
    m_pFastKillButton = new AnimitedButton(":/resource/hexagon_btn", ":/resource/fast_kill_frames", u8"快速查杀", 15);
    m_pCustomKillButton = new AnimitedButton(":/resource/hexagon_small_btn", ":/resource/custom_kill_frames", u8"自定义查杀");
    //hlayout->addSpacing(130);
    //hlayout->addWidget(m_pAllKillButton, 0, Qt::AlignLeft|Qt::AlignBottom);
    hlayout->addWidget(m_pAllKillButton, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hlayout->addWidget(m_pFastKillButton, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hlayout->addWidget(m_pCustomKillButton, 0, Qt::AlignLeft|Qt::AlignVCenter);
    hlayout->addStretch();
    this->setLayout(hlayout);
}

TopBodyWidget::~TopBodyWidget()
{

}

void TopBodyWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    //setBackgroud(":/resource/body_bg");
}
