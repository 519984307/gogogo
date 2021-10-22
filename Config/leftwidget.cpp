#include <QPainter>
#include <QVBoxLayout>

#include "leftwidget.h"
#include "common/pubdef.h"
#include "appconfig.h"

struct stru_table_text LW_table_text[] = {
    {IMG_TXT_LABEL_TYPE_KILL, u8"病毒查杀"},
    {IMG_TXT_LABEL_TYPE_NETDEFEND, u8"上网防护"},
    {IMG_TXT_LABEL_TYPE_SETUP, u8"基础设置"},
    {IMG_TXT_LABEL_TYPE_IP, u8"IP设置"}, //2021.9.18 by zhangdong


    {IMG_TXT_LABEL_TYPE_ORI, u8"常规项"},
    {IMG_TXT_LABEL_TYPE_WHITELIST, u8"白名单"},
    {IMG_TXT_LABEL_TYPE_BACKUP, u8"杀毒备份"},
    {IMG_TXT_LABEL_TYPE_KV, u8"病毒扫描"},
    {IMG_TXT_LABEL_TYPE_TIMER, u8"定时扫描"},
    {IMG_TXT_LABEL_TYPE_FILE, u8"文件监控"},
    {IMG_TXT_LABEL_TYPE_UDISK, u8"U盘监控"},
    //{IMG_TXT_LABEL_TYPE_SYSTEM, u8"系统加固"},
    //{IMG_TXT_LABEL_TYPE_APP, u8"应用加固"},
    //{IMG_TXT_LABEL_TYPE_NETBLACKLIST, u8"黑名单"}//,
    //{IMG_TXT_LABEL_TYPE_ADMIN, u8"管理员身份"},
//    {IMG_TXT_LABEL_TYPE_TRAY, u8"托盘设置"},
//    {IMG_TXT_LABEL_TYPE_LICENSE, u8"激活授权"},
    {IMG_TXT_LABEL_TYPE_UPGRADE, u8"软件更新"},
    {IMG_TXT_LABEL_TYPE_DISCOVERYCENTER, u8"中心发现"},
    {IMG_TXT_LABEL_TYPE_IPWHITELIST, u8"白名单"}, //2021.9.18 by zhangdong
    {IMG_TXT_LABEL_TYPE_IPBLACKLIST, u8"黑名单"},//2021.9.18 by zhangdong
    {IMG_TXT_LABEL_TYPE_IPPORT, u8"端口"}   //2021.10.8 by zhangdong
};


LeftWidget::LeftWidget(QWidget *parent, int nFunction, GlobalConf *pConf, int nHeight) : QWidget(parent)
{
    m_pMainWindow = (MainWindow*)parent;
    m_nFunction = nFunction;
    m_pConf = pConf;
    //setStyleSheet("background-color:rgb(248,248,248)");
    setFixedSize(APP_SCREEN_LEFT_WIDTH, nHeight);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(248,248,248));
    setPalette(palette);
    setAutoFillBackground(true);

    //setStyleSheet("background-color:rgb(248,248,248)");

    InitTree();

    m_TopLabels.append(m_pitlOri);
    m_TopLabels.append(m_pitlWhiteList);
    m_TopLabels.append(m_pitlBackup);
    m_TopLabels.append(m_pitlKV);
    m_TopLabels.append(m_pitlTimer);
    if (m_nFunction & AppConfig::FILE_DEFEN) {
        m_TopLabels.append(m_pitlFile);
    }else{
        m_pitlFile->hide();
    }
    if (m_nFunction & AppConfig::PLUGIN_CHECKER) {
        m_TopLabels.append(m_pitlUDisk);
    }else{
        m_pitlUDisk->hide();
    }
//    if((RAVQT_FUNC_FLAG & 0x40)==0 || (RAVQT_FUNC_FLAG & 0x20)==0){
//        m_pitlFile->hide();
//        m_pitlUDisk->hide();
//    }else{
//        m_TopLabels.append(m_pitlFile);
//        m_TopLabels.append(m_pitlUDisk);
//    }

   // m_TopLabels.append(m_pitlSystem);
   // m_TopLabels.append(m_pitlApp);

    //m_EndLabels.append(m_pitlAdmin);
//    m_EndLabels.append(m_pitlTray);
//    m_EndLabels.append(m_pitlLicense);
    if (HideAdvanceFunction()) {
        m_EndLabels.append(m_pitlUpgrade);
        m_EndLabels.append(m_pitlCenterDiscovery);
    }

    m_AddLabels.append(m_pitlIpWhiteList);
    m_AddLabels.append(m_pitlIpBlackList);
    m_AddLabels.append(m_pitlIpPort);

    SetTopDisable();
    m_pitlOri->SetSelected(true);
}

void LeftWidget::InitTree()
{
    m_pitlKill = new ImgTxtLabel(this, m_pConf);
   // m_pitlNetDefend = new ImgTxtLabel(this, m_pConf);

    if (HideAdvanceFunction()) {
        m_pitlSetup = new ImgTxtLabel(this, m_pConf);
    }

	//2021.9.18 by zhangdong
    m_pitlIp = new ImgTxtLabel(this, m_pConf);

    m_pitlOri = new ImgTxtLabel(this, m_pConf);
    m_pitlWhiteList = new ImgTxtLabel(this, m_pConf);
    m_pitlBackup = new ImgTxtLabel(this, m_pConf);
    m_pitlKV = new ImgTxtLabel(this, m_pConf);
    m_pitlTimer = new ImgTxtLabel(this, m_pConf);
    m_pitlFile = new ImgTxtLabel(this, m_pConf);
    m_pitlUDisk = new ImgTxtLabel(this, m_pConf);
//    m_pitlSystem = new ImgTxtLabel(this, m_pConf);
//    m_pitlApp = new ImgTxtLabel(this, m_pConf);

//    m_pitlNetBlacklist = new ImgTxtLabel(this, m_pConf);

   // m_pitlAdmin = new ImgTxtLabel(this, m_pConf);
//    m_pitlTray = new ImgTxtLabel(this, m_pConf);
//    m_pitlLicense = new ImgTxtLabel(this, m_pConf);
    if (HideAdvanceFunction()) {
        m_pitlUpgrade = new ImgTxtLabel(this, m_pConf);
        m_pitlCenterDiscovery = new ImgTxtLabel(this, m_pConf);
    }
	//2021.9.22 by zhangdong
    m_pitlIpWhiteList = new ImgTxtLabel(this, m_pConf);
    m_pitlIpBlackList = new ImgTxtLabel(this, m_pConf);
    m_pitlIpPort =		new ImgTxtLabel(this, m_pConf);

    m_pitlKill->LoadPixmap(":/resource/kill_gray");
    m_pitlKill->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_KILL].text);
    if (HideAdvanceFunction()) {
        m_pitlSetup->LoadPixmap(":/resource/setup_gray");
        m_pitlSetup->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_SETUP].text);
    }

	/****2021.9.22 by zhangdong*******/
    m_pitlIp->LoadPixmap(":/resource/setup_gray");
    m_pitlIp->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_IP].text);

    m_pitlOri->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_ORI].text);
    m_pitlWhiteList->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_WHITELIST].text);
    m_pitlBackup->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_BACKUP].text);
    m_pitlKV->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_KV].text);
    m_pitlTimer->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_TIMER].text);
    m_pitlFile->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_FILE].text);
    m_pitlUDisk->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_UDISK].text);
//    m_pitlSystem->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_SYSTEM].text);
//    m_pitlApp->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_APP].text);

    //m_pitlNetDefend->LoadPixmap(":/resource/netdefend_gray_icon");
    //m_pitlNetDefend->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_NETDEFEND].text);
   // m_pitlNetBlacklist->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_NETBLACKLIST].text);

    //m_pitlAdmin->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_ADMIN].text);
//    m_pitlTray->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_TRAY].text);
//    m_pitlLicense->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_LICENSE].text);

    //　在这里作限制，　如果是保密机或者单机版隐藏掉升级和中心发现
    if (HideAdvanceFunction()) {
        m_pitlUpgrade->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_UPGRADE].text);
        m_pitlCenterDiscovery->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_DISCOVERYCENTER].text);
    }
	/****2021.9.22 by zhangdong*******/
    m_pitlIpWhiteList->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_IPWHITELIST].text);
    m_pitlIpBlackList->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_IPBLACKLIST].text);
    m_pitlIpPort->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_IPPORT].text);
}

void LeftWidget::OnClickLabel(ImgTxtLabel *pLabel)
{
    qDebug("LeftWidget::OnClickLabel(ImgTxtLabel *pLabel) %s", pLabel->m_sText.toStdString().c_str());
    bool bfind = false;
    if(m_pitlKill->IsDisable()){
        QVector<ImgTxtLabel*>::iterator it = m_TopLabels.begin();
        while (it != m_TopLabels.end()) {
            ImgTxtLabel *pl = *it;
            if(pl == pLabel){
                pl->SetSelected(true);
                bfind = true;
                //qDebug("LeftWidget::OnClickeLabel find label:%s", pl->objectName());
                it++;
                continue;
            }
            pl->SetSelected(false);
            it++;
        }
        if(!bfind){
            m_pitlOri->SetSelected(true);
        }
    }//else if(m_pitlNetDefend->IsDisable()){
     //   m_pitlNetBlacklist->SetSelected(true);
    //}
    else if(m_pitlSetup->IsDisable()){
        QVector<ImgTxtLabel*>::iterator it = m_EndLabels.begin();
        while (it != m_EndLabels.end()) {
            ImgTxtLabel *pl = *it;
            if(pl == pLabel){
                pl->SetSelected(true);
                bfind = true;
                //qDebug("LeftWidget::OnClickeLabel find label:%s", pl->objectName());
                it++;
                continue;
            }
            pl->SetSelected(false);
            it++;
        }
        if(!bfind){
            m_pitlUpgrade->SetSelected(true); //m_pitlAdmin->SetSelected(true);
        }
    }
	/****2021.9.23 by zhangdong*******/
	else if(m_pitlIp->IsDisable()){
        QVector<ImgTxtLabel*>::iterator it = m_AddLabels.begin();
        while (it != m_AddLabels.end()) {
            ImgTxtLabel *pl = *it;
            if(pl == pLabel){
                pl->SetSelected(true);
                bfind = true;
                //qDebug("LeftWidget::OnClickeLabel find label:%s", pl->objectName());
                it++;
                continue;
            }
            pl->SetSelected(false);
            it++;
        }
        if(!bfind){
            m_pitlIpWhiteList->SetSelected(true);
        }
	}
}

ImgTxtLabel* LeftWidget::TopSelectedLabel()
{
    QVector<ImgTxtLabel*>::iterator it = m_TopLabels.begin();
    while (it != m_TopLabels.end()) {
        ImgTxtLabel *pl = *it;
        if(pl->IsSelected()){
            return pl;
        }
        it++;
    }
    return m_pitlOri;
}

//ImgTxtLabel* LeftWidget::MidSelectedLabel()
//{
//    return m_pitlNetBlacklist;
//}

ImgTxtLabel* LeftWidget::EndSelectLabel()
{
    qDebug("LeftWidget::EndSelectLabel()");
    QVector<ImgTxtLabel*>::iterator it = m_EndLabels.begin();
    while (it != m_EndLabels.end()) {
        ImgTxtLabel *pl = *it;
        if(pl->IsSelected()){
            return pl;
        }
        it++;
    }
    return m_pitlUpgrade;
    //return m_pitlAdmin;
}

/****2021.9.23 by zhangdong*******/
ImgTxtLabel* LeftWidget::AddSelectLabel()
{
    QVector<ImgTxtLabel*>::iterator it = m_AddLabels.begin();
    while (it != m_AddLabels.end()) {
        ImgTxtLabel *pl = *it;
        if(pl->IsSelected()){
            return pl;
        }
        it++;
    }
    return m_pitlIpWhiteList;
}

bool LeftWidget::HideAdvanceFunction() {
    return !(m_pMainWindow->IsSecretMachine() || m_pMainWindow->IsCenterLess());
}

void LeftWidget::SetTopDisable()
{
    int end_label_pos_num = 6;
    m_pitlKill->setGeometry(0,0,m_pitlKill->width(),m_pitlKill->height());
    m_pitlKill->SetDisable(true);

    m_pitlOri->setGeometry(0,IMG_TXT_LABEL_HEIGHT,m_pitlKill->width(),m_pitlKill->height());
    m_pitlOri->show();
    m_pitlWhiteList->setGeometry(0,IMG_TXT_LABEL_HEIGHT*2,m_pitlKill->width(),m_pitlKill->height());
    m_pitlWhiteList->show();
    m_pitlBackup->setGeometry(0,IMG_TXT_LABEL_HEIGHT*3,m_pitlKill->width(),m_pitlKill->height());
    m_pitlBackup->show();
    m_pitlKV->setGeometry(0,IMG_TXT_LABEL_HEIGHT*4,m_pitlKill->width(),m_pitlKill->height());
    m_pitlKV->show();
    m_pitlTimer->setGeometry(0,IMG_TXT_LABEL_HEIGHT*5,m_pitlKill->width(),m_pitlKill->height());
    m_pitlTimer->show();
    if (m_nFunction & AppConfig::FILE_DEFEN) {
        m_pitlFile->setGeometry(0,IMG_TXT_LABEL_HEIGHT*6,m_pitlKill->width(),m_pitlKill->height());
        m_pitlFile->show();
        end_label_pos_num++;
        if (m_nFunction & AppConfig::PLUGIN_CHECKER) {
            m_pitlUDisk->setGeometry(0,IMG_TXT_LABEL_HEIGHT*7,m_pitlKill->width(),m_pitlKill->height());
            m_pitlUDisk->show();
            end_label_pos_num++;
        }
    }else{
        if (m_nFunction & AppConfig::PLUGIN_CHECKER) {
            m_pitlUDisk->setGeometry(0,IMG_TXT_LABEL_HEIGHT*6,m_pitlKill->width(),m_pitlKill->height());
            m_pitlUDisk->show();
            end_label_pos_num++;
        }
    }

//    if((RAVQT_FUNC_FLAG & 0x40) && (RAVQT_FUNC_FLAG & 0x20)){
//        m_pitlFile->setGeometry(0,IMG_TXT_LABEL_HEIGHT*6,m_pitlKill->width(),m_pitlKill->height());
//        m_pitlFile->show();
//        m_pitlUDisk->setGeometry(0,IMG_TXT_LABEL_HEIGHT*7,m_pitlKill->width(),m_pitlKill->height());
//        m_pitlUDisk->show();
//    }
//    m_pitlSystem->setGeometry(0,IMG_TXT_LABEL_HEIGHT*8,m_pitlSetup->width(),m_pitlSetup->height());
//    m_pitlSystem->show();
//    m_pitlApp->setGeometry(0,IMG_TXT_LABEL_HEIGHT*8,m_pitlSetup->width(),m_pitlSetup->height());
//    m_pitlApp->show();
//    m_pitlNetDefend->setGeometry(0,IMG_TXT_LABEL_HEIGHT*8,m_pitlNetDefend->width(),m_pitlNetDefend->height());
//    m_pitlNetBlacklist->hide();

    if (HideAdvanceFunction()) {
        //qDebug("LeftWidget::SetTopDisable() end_label_pos_num=%d, width=%d height=%d", end_label_pos_num, m_pitlSetup->width(), m_pitlSetup->height());
        m_pitlSetup->setGeometry(0, IMG_TXT_LABEL_HEIGHT*end_label_pos_num, m_pitlSetup->width(), m_pitlSetup->height());
        m_pitlSetup->SetDisable(false);
        m_pitlSetup->show();
        //m_pitlAdmin->hide();
        //m_pitlTray->hide();
        //m_pitlLicense->hide();
        m_pitlUpgrade->hide();
        m_pitlCenterDiscovery->hide();
		end_label_pos_num++;
    }

	/****2021.9.22 by zhangdong*******/
	m_pitlIp->setGeometry(0, IMG_TXT_LABEL_HEIGHT*end_label_pos_num, m_pitlIp->width(), m_pitlIp->height());
    m_pitlIp->SetDisable(false);
    m_pitlIp->show();
    m_pitlIpWhiteList->hide();
    m_pitlIpBlackList->hide();
    m_pitlIpPort->hide();
}

//void LeftWidget::SetMidDisable()
//{
//    m_pitlKill->setGeometry(0,0,m_pitlKill->width(),m_pitlKill->height());
//    m_pitlKill->SetDisable(false);

//    m_pitlOri->hide();
//    m_pitlWhiteList->hide();
//    m_pitlBackup->hide();
//    m_pitlKV->hide();
//    m_pitlTimer->hide();
//    m_pitlFile->hide();
//    m_pitlUDisk->hide();

//    m_pitlNetDefend->setGeometry(0,IMG_TXT_LABEL_HEIGHT,m_pitlKill->width(),m_pitlKill->height());
//    m_pitlNetDefend->SetDisable(true);
//    m_pitlNetBlacklist->setGeometry(0,IMG_TXT_LABEL_HEIGHT*2,m_pitlKill->width(),m_pitlKill->height());
//    m_pitlNetBlacklist->show();

//    m_pitlSetup->setGeometry(0,IMG_TXT_LABEL_HEIGHT*3,m_pitlSetup->width(),m_pitlSetup->height());
//    m_pitlSetup->SetDisable(false);
//    //m_pitlAdmin->hide();
//    m_pitlTray->hide();
//    m_pitlLicense->hide();
//    m_pitlUpgrade->hide();
//}

void LeftWidget::SetEndDisable()
{
    qDebug("LeftWidget::SetEndDisable()");
    if (HideAdvanceFunction()) {
        m_pitlKill->setGeometry(0,0,m_pitlKill->width(),m_pitlKill->height());
        m_pitlKill->SetDisable(false);

        m_pitlOri->hide();
        m_pitlWhiteList->hide();
        m_pitlBackup->hide();
        m_pitlKV->hide();
        m_pitlTimer->hide();
        m_pitlFile->hide();
        m_pitlUDisk->hide();
        //m_pitlSystem->hide();
        //m_pitlApp->hide();

        //m_pitlNetDefend->setGeometry(0,IMG_TXT_LABEL_HEIGHT,m_pitlSetup->width(),m_pitlSetup->height());
        //m_pitlNetDefend->SetDisable(false);
        //m_pitlNetBlacklist->hide();

        m_pitlSetup->setGeometry(0, IMG_TXT_LABEL_HEIGHT, m_pitlSetup->width(), m_pitlSetup->height());
        m_pitlSetup->SetDisable(true);

        //m_pitlAdmin->setGeometry(0,IMG_TXT_LABEL_HEIGHT*3,m_pitlSetup->width(),m_pitlSetup->height());
        //m_pitlAdmin->show();
        //m_pitlTray->setGeometry(0,IMG_TXT_LABEL_HEIGHT*3,m_pitlSetup->width(),m_pitlSetup->height());
        //m_pitlTray->show();
        //m_pitlLicense->setGeometry(0,IMG_TXT_LABEL_HEIGHT*4,m_pitlSetup->width(),m_pitlSetup->height());
        //m_pitlLicense->show();
        m_pitlUpgrade->setGeometry(0, IMG_TXT_LABEL_HEIGHT*2, m_pitlSetup->width(), m_pitlSetup->height());
        m_pitlUpgrade->show();
        m_pitlCenterDiscovery->setGeometry(0, IMG_TXT_LABEL_HEIGHT*3, m_pitlSetup->width(), m_pitlSetup->height());
        m_pitlCenterDiscovery->show();

		/****2021.9.22 by zhangdong*******/
		m_pitlIp->setGeometry(0, IMG_TXT_LABEL_HEIGHT*4, m_pitlIp->width(), m_pitlIp->height());
		m_pitlIp->SetDisable(false);
		m_pitlIp->show();
		m_pitlIpWhiteList->hide();
		m_pitlIpBlackList->hide();
		m_pitlIpPort->hide();
    }
}

/****2021.9.22 by zhangdong*******/
void LeftWidget::SetAddDisable()
{
	m_pitlKill->setGeometry(0,0,m_pitlKill->width(),m_pitlKill->height());
	m_pitlKill->SetDisable(false);
	m_pitlKill->show();
	
	m_pitlOri->hide();
	m_pitlWhiteList->hide();
	m_pitlBackup->hide();
	m_pitlKV->hide();
	m_pitlTimer->hide();
	m_pitlFile->hide();
	m_pitlUDisk->hide();

    if (HideAdvanceFunction()) {
        //qDebug("LeftWidget::SetTopDisable() end_label_pos_num=%d, width=%d height=%d", end_label_pos_num, m_pitlSetup->width(), m_pitlSetup->height());
        m_pitlSetup->setGeometry(0, IMG_TXT_LABEL_HEIGHT, m_pitlSetup->width(), m_pitlSetup->height());
        m_pitlSetup->SetDisable(false);
        m_pitlSetup->show();
        m_pitlUpgrade->hide();
        m_pitlCenterDiscovery->hide();
    }
	m_pitlIp->setGeometry(0,IMG_TXT_LABEL_HEIGHT*2,m_pitlIp->width(),m_pitlIp->height());
	m_pitlIp->SetDisable(true);
	m_pitlIp->show();
	
	m_pitlIpWhiteList->setGeometry(0, IMG_TXT_LABEL_HEIGHT*3, m_pitlIp->width(), m_pitlIp->height());
	m_pitlIpWhiteList->show();
	m_pitlIpBlackList->setGeometry(0, IMG_TXT_LABEL_HEIGHT*4, m_pitlIp->width(), m_pitlIp->height());
	m_pitlIpBlackList->show();
	m_pitlIpPort->setGeometry(0, IMG_TXT_LABEL_HEIGHT*5, m_pitlIp->width(), m_pitlIp->height());
	m_pitlIpPort->show();

}

void LeftWidget::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);
    QPainter painter(this);
    QPen pen(QColor(212,205,217), 1);
    painter.setPen(pen);
    painter.drawLine(APP_SCREEN_LEFT_WIDTH-1, 0, APP_SCREEN_LEFT_WIDTH-1, height());
}


