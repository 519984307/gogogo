#include <QVBoxLayout>

#include "common/pubdef.h"
#include "topscrollwidget.h"

TopScrollWidget::TopScrollWidget(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH);

    m_pLayout = new QVBoxLayout;
    m_pLayout->setContentsMargins(0,0,0,0);
    m_pVDetail = new VirusDetailGroup(this);


    setLayout(m_pLayout);
}

//VirusDetailGroup        *m_pVDetail;
//ScanEventGroup          *m_pScanEvent;
//SysReinforceGroup       *m_pSysReinforce;
//AppReinforceGroup       *m_pAppReinforce;
//IsolationGroup          *m_pIsolation;
