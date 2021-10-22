#include "common/pubdef.h"
#include "netblacklistgroup.h"
#include "appgroup.h"

#include "midscrollwidget.h"

MidScrollWidget::MidScrollWidget(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH);

    m_pLayout = new QVBoxLayout;

    m_pBlacklist = new NetBlacklistGroup(this);

//    m_pSystem = new SystemGroup(this);
//    m_pApp = new AppGroup(this);

    m_pLayout->addWidget(m_pBlacklist);

//    m_pLayout->addWidget(m_pSystem);
//    m_pLayout->addWidget(m_pApp);

    setLayout(m_pLayout);
}
