#include "common/pubdef.h"
#include "addscrollwidget.h"

void AddScrollWidget::EnableFireWallClicked()
{
	m_pIpWhiteList->firewallflag = !m_pIpWhiteList->firewallflag;
	m_pIpBlackList->firewallflag = !m_pIpBlackList->firewallflag;
	m_pIpPort->firewallflag = !m_pIpPort->firewallflag;
	m_pIpWhiteList->m_IpWhiteName->firewallflag = !m_pIpWhiteList->m_IpWhiteName->firewallflag;
	m_pIpBlackList->m_IpBlackName->firewallflag = !m_pIpBlackList->m_IpBlackName->firewallflag;
	m_pIpPort->m_PortName->firewallflag = !m_pIpPort->m_PortName->firewallflag;
}

AddScrollWidget::AddScrollWidget(QWidget *parent, MainWindow *pMainWindow) : QWidget(parent)
{
    m_pMainWindow = pMainWindow;

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH);

    m_pLayout = new QVBoxLayout;

	m_eFireWall = new QCheckBox("开启防火墙");
	//m_eFireWall->setFont(font);
	connect(m_eFireWall, SIGNAL(clicked()), this, SLOT(EnableFireWallClicked()));

   m_pIpWhiteList = new PortGroup(0,this);
   m_pIpBlackList = new PortGroup(1,this);
   m_pIpPort =		new PortGroup(2,this);
    if (!(m_pMainWindow->IsSecretMachine() || m_pMainWindow->IsCenterLess())) {
        //m_pUpgrade = new UpgradeGroup(this);
        //m_pCenterDiscovery = new CenterDiscoveryGroup(this);

        //m_pLayout->addWidget(m_pAdmin);
        //m_pLayout->addWidget(m_pTray);
        //m_pLayout->addWidget(m_pLicense);

        //m_pLayout->addWidget(m_pUpgrade);
        //m_pLayout->addWidget(m_pCenterDiscovery);
    }
    m_pLayout->addWidget(m_eFireWall);
    m_pLayout->addWidget(m_pIpWhiteList);
    m_pLayout->addWidget(m_pIpBlackList);
    m_pLayout->addWidget(m_pIpPort);
    setLayout(m_pLayout);
}
