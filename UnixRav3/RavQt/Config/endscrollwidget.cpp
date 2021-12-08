#include "common/pubdef.h"
#include "endscrollwidget.h"

EndScrollWidget::EndScrollWidget(QWidget *parent, MainWindow *pMainWindow) : QWidget(parent)
{
    m_pMainWindow = pMainWindow;

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH);

    m_pLayout = new QVBoxLayout;

   // m_pAdmin = new AdminGroup(this);
   // m_pTray = new TrayGroup(this);
   // m_pLicense = new LicenseGroup(this);
    if (!(m_pMainWindow->IsSecretMachine() || m_pMainWindow->IsCenterLess())) {
        m_pUpgrade = new UpgradeGroup(this);
        m_pCenterDiscovery = new CenterDiscoveryGroup(this);

        //m_pLayout->addWidget(m_pAdmin);
        //m_pLayout->addWidget(m_pTray);
        //m_pLayout->addWidget(m_pLicense);

        m_pLayout->addWidget(m_pUpgrade);
        m_pLayout->addWidget(m_pCenterDiscovery);
    }
    setLayout(m_pLayout);
}
