#include <QPainter>
#include <QVBoxLayout>

#include "leftwidget.h"
#include "common/pubdef.h"
#include "appconfig.h"


struct stru_table_text LW_table_text[] = {
    {IMG_TXT_LABEL_TYPE_KILL,u8"病毒查杀"},
    {IMG_TXT_LABEL_TYPE_VIRUS_DETAIL,u8"病毒详情"},
    {IMG_TXT_LABEL_TYPE_SCAN_EVENT,u8"扫描事件"},
    {IMG_TXT_LABEL_TYPE_ISOLATION,u8"隔离区"},
    {IMG_TXT_LABEL_TYPE_NET_DEFENCE,u8"上网防护"},
    {IMG_TXT_LABEL_TYPE_URL_ACCESS,u8"网址访问"},
    {IMG_TXT_LABEL_TYPE_IP_RULE,u8"IP规则"},
    {IMG_TXT_LABEL_TYPE_BASIC_LOG,u8"基础日志"},
    {IMG_TXT_LABEL_TYPE_INSTALLATION,u8"安装部署"},
    {IMG_TXT_LABEL_TYPE_PLATFORM_LOG,u8"平台日志"}
};


LeftWidget::LeftWidget(QWidget *parent, GlobalConf *pConf, int nFunctions, bool centerless) : QWidget(parent)
{
    m_pConf = pConf;
    m_pitlPlatformLog = NULL;

    m_nFunctions = nFunctions;
    //setStyleSheet("background-color:rgb(248,248,248)");
    //setFixedSize(APP_SCREEN_LEFT_WIDTH, nHeight);
    setFixedWidth(APP_SCREEN_LEFT_WIDTH);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(248,248,248));
    setPalette(palette);
    setAutoFillBackground(true);

    //setStyleSheet("background-color:rgb(248,248,248)");

    InitTree(centerless);

    m_TopLabels.append(m_pitlVirusDetail);
    m_TopLabels.append(m_pitlScanEvent);
//    m_TopLabels.append(m_pitlAppReinforce);
    m_TopLabels.append(m_pitlIsolation);

    m_MidLabels.append(m_pitlUrlAccess);
    m_MidLabels.append(m_pitlIPRule);

    m_EndLabels.append(m_pitlInstallation);

    if (!centerless) {
        //m_EndLabels.append(m_pitlPlatformLog);
    }

    SetTopDisable();
    m_pitlVirusDetail->SetSelected(true);
}

void LeftWidget::InitTree(bool centerless)
{
    m_pitlKill = new ImgTxtLabel(this, m_pConf);
    m_pitlNetDefence = new ImgTxtLabel(this, m_pConf);
    m_pitlBasicLog = new ImgTxtLabel(this, m_pConf);

    m_pitlVirusDetail = new ImgTxtLabel(this, m_pConf);
    m_pitlScanEvent = new ImgTxtLabel(this, m_pConf);
//    m_pitlAppReinforce = new ImgTxtLabel(this, m_pConf);
    m_pitlIsolation = new ImgTxtLabel(this, m_pConf);

    m_pitlUrlAccess = new ImgTxtLabel(this, m_pConf);
    m_pitlIPRule = new ImgTxtLabel(this, m_pConf);

    m_pitlInstallation = new ImgTxtLabel(this, m_pConf);

    if (!centerless) {
        //m_pitlPlatformLog = new ImgTxtLabel(this, m_pConf);
    }

    m_pitlKill->LoadPixmap(":/resource/kill_gray");
    m_pitlKill->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_KILL].text);

    m_pitlNetDefence->LoadPixmap((":/resource/netdefend_gray"));
    m_pitlNetDefence->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_NET_DEFENCE].text);

    m_pitlBasicLog->LoadPixmap(":/resource/setup_gray");
    m_pitlBasicLog->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_BASIC_LOG].text);

    m_pitlVirusDetail->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_VIRUS_DETAIL].text);
    m_pitlScanEvent->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_SCAN_EVENT].text);
    m_pitlIsolation->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_ISOLATION].text);

    m_pitlUrlAccess->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_URL_ACCESS].text);
    m_pitlIPRule->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_IP_RULE].text);

    m_pitlInstallation->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_INSTALLATION].text);

    if (!centerless) {
        //m_pitlPlatformLog->SetText(LW_table_text[IMG_TXT_LABEL_TYPE_PLATFORM_LOG].text);
    }

}

void LeftWidget::OnClickLabel(ImgTxtLabel *pLabel)
{
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
            m_pitlVirusDetail->SetSelected(true);
        }
    }else if(m_pitlNetDefence->IsDisable()){
        QVector<ImgTxtLabel*>::iterator it = m_MidLabels.begin();
        while (it != m_MidLabels.end()) {
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
            m_pitlUrlAccess->SetSelected(true);
        }
    }else if(m_pitlBasicLog->IsDisable()){
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
            m_pitlInstallation->SetSelected(true);
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
    return m_pitlVirusDetail;
}

ImgTxtLabel* LeftWidget::MidSelectedLabel()
{
    QVector<ImgTxtLabel*>::iterator it = m_MidLabels.begin();
    while (it != m_MidLabels.end()) {
        ImgTxtLabel *pl = *it;
        if(pl->IsSelected()){
            return pl;
        }
        it++;
    }
    return m_pitlUrlAccess;
}

ImgTxtLabel* LeftWidget::EndSelectLabel()
{
    QVector<ImgTxtLabel*>::iterator it = m_EndLabels.begin();
    while (it != m_EndLabels.end()) {
        ImgTxtLabel *pl = *it;
        if(pl->IsSelected()){
            return pl;
        }
        it++;
    }
    return m_pitlInstallation;
}

void LeftWidget::SetTopDisable()
{
    m_pitlKill->setGeometry(0,0,m_pitlKill->width(),m_pitlKill->height());
    m_pitlKill->SetDisable(true);

    m_pitlVirusDetail->setGeometry(0,IMG_TXT_LABEL_HEIGHT,m_pitlBasicLog->width(),m_pitlBasicLog->height());
    m_pitlVirusDetail->show();
    m_pitlScanEvent->setGeometry(0,IMG_TXT_LABEL_HEIGHT*2,m_pitlBasicLog->width(),m_pitlBasicLog->height());
    m_pitlScanEvent->show();
//    m_pitlAppReinforce->setGeometry(0,IMG_TXT_LABEL_HEIGHT*4,m_pitlBasicLog->width(),m_pitlBasicLog->height());
//    m_pitlAppReinforce->show();
    m_pitlIsolation->setGeometry(0,IMG_TXT_LABEL_HEIGHT*3,m_pitlBasicLog->width(),m_pitlBasicLog->height());
    m_pitlIsolation->show();

    if (m_nFunctions & AppConfig::NET_FILTER) {
        m_pitlNetDefence->setGeometry(0,IMG_TXT_LABEL_HEIGHT*4,m_pitlBasicLog->width(),m_pitlBasicLog->height());
        m_pitlNetDefence->SetDisable(false);

        m_pitlBasicLog->setGeometry(0,IMG_TXT_LABEL_HEIGHT*5,m_pitlBasicLog->width(),m_pitlBasicLog->height());
        m_pitlBasicLog->SetDisable(false);
    }else {
        m_pitlNetDefence->hide();

        m_pitlBasicLog->setGeometry(0,IMG_TXT_LABEL_HEIGHT*4,m_pitlBasicLog->width(),m_pitlBasicLog->height());
        m_pitlBasicLog->SetDisable(false);
    }

    m_pitlUrlAccess->hide();
    m_pitlIPRule->hide();

    m_pitlInstallation->hide();

    if (m_pitlPlatformLog) {
        m_pitlPlatformLog->hide();
    }
}

void LeftWidget::SetMidDisable()
{
    m_pitlKill->setGeometry(0,0,m_pitlKill->width(),m_pitlKill->height());
    m_pitlKill->SetDisable(false);

    m_pitlVirusDetail->hide();
    m_pitlScanEvent->hide();
    m_pitlIsolation->hide();

    m_pitlNetDefence->setGeometry(0,IMG_TXT_LABEL_HEIGHT,m_pitlBasicLog->width(),m_pitlBasicLog->height());
    m_pitlNetDefence->SetDisable(true);

    m_pitlUrlAccess->setGeometry(0, IMG_TXT_LABEL_HEIGHT*2,m_pitlBasicLog->width(),m_pitlBasicLog->height());
    m_pitlUrlAccess->show();
    m_pitlIPRule->setGeometry(0, IMG_TXT_LABEL_HEIGHT*3,m_pitlBasicLog->width(),m_pitlBasicLog->height());
    m_pitlIPRule->show();

    m_pitlBasicLog->setGeometry(0,IMG_TXT_LABEL_HEIGHT*4,m_pitlBasicLog->width(),m_pitlBasicLog->height());
    m_pitlBasicLog->SetDisable(false);

    m_pitlInstallation->hide();
    if (m_pitlPlatformLog) {
        m_pitlPlatformLog->hide();
    }
}

void LeftWidget::SetEndDisable()
{
    m_pitlKill->setGeometry(0,0,m_pitlKill->width(),m_pitlKill->height());
    m_pitlKill->SetDisable(false);

    m_pitlVirusDetail->hide();
    m_pitlScanEvent->hide();
//    m_pitlAppReinforce->hide();
    m_pitlIsolation->hide();

    if (m_nFunctions & AppConfig::NET_FILTER) {
        m_pitlNetDefence->setGeometry(0,IMG_TXT_LABEL_HEIGHT, m_pitlBasicLog->width(), m_pitlBasicLog->height());
        m_pitlNetDefence->SetDisable(false);
        m_pitlBasicLog->setGeometry(0,IMG_TXT_LABEL_HEIGHT*2,m_pitlBasicLog->width(),m_pitlBasicLog->height());
        m_pitlBasicLog->SetDisable(true);

        m_pitlInstallation->setGeometry(0,IMG_TXT_LABEL_HEIGHT*3,m_pitlBasicLog->width(),m_pitlBasicLog->height());
        m_pitlInstallation->show();
        if (m_pitlPlatformLog) {
            m_pitlPlatformLog->setGeometry(0,IMG_TXT_LABEL_HEIGHT*4,m_pitlBasicLog->width(),m_pitlBasicLog->height());
            m_pitlPlatformLog->show();
        }
    }else {
        m_pitlNetDefence->hide();
        m_pitlBasicLog->setGeometry(0,IMG_TXT_LABEL_HEIGHT,m_pitlBasicLog->width(),m_pitlBasicLog->height());
        m_pitlBasicLog->SetDisable(true);

        m_pitlInstallation->setGeometry(0,IMG_TXT_LABEL_HEIGHT*2,m_pitlBasicLog->width(),m_pitlBasicLog->height());
        m_pitlInstallation->show();

        if (m_pitlPlatformLog) {
            m_pitlPlatformLog->setGeometry(0,IMG_TXT_LABEL_HEIGHT*3,m_pitlBasicLog->width(),m_pitlBasicLog->height());
            m_pitlPlatformLog->show();
        }
    }

    m_pitlUrlAccess->hide();
    m_pitlIPRule->hide();


}

void LeftWidget::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);
    QPainter painter(this);
    QPen pen(QColor(212,205,217), 1);
    painter.setPen(pen);
    painter.drawLine(APP_SCREEN_LEFT_WIDTH-1, 0, APP_SCREEN_LEFT_WIDTH-1, height());
}


