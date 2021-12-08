#include <QPainter>

#include <QDebug>

#include "scanbotbodywidget.h"
#include "common.h"

#define APP_BOTBODY_LEFT_EDGE   30
#define APP_BOTBODY_TOP_EDGE    6
#define APP_BOTBODY_VERSION_TXT_LEN 80
#define APP_BOTBODY_UPDATE_TXT_LEN  140
#define APP_BOTBODY_IMG_TXT_SPACE   4


ScanBotBodyWidget::ScanBotBodyWidget(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(APP_BOT_BODY_HEIGHT);
    LoadPixmap();
    LoadStatusWidgets();
}

void ScanBotBodyWidget::LoadPixmap()
{
    m_ppVersion = new QPixmap;
    bool bret = m_ppVersion->load(":/resource/version_txt");
    if(!bret){
//        qDebug("ScanBotBodyWidget::LoadPixmap version fail");
    }
    m_ppUpdateTime = new QPixmap;
    bret = m_ppUpdateTime->load(":/resource/update_data_txt");
    if(!bret){
//        qDebug("ScanBotBodyWidget::LoadPixmap updatetime fail");
    }
}

void ScanBotBodyWidget::LoadStatusWidgets()
{
    //QRect pos = rect();
    //qDebug("status pos x,y,w,h: %d  %d  %d  %d", pos.x(), pos.y(), pos.width(), pos.height());
    m_plVersion = new QLabel(this);
    m_plUpdatetime = new QLabel(this);
//    m_cupbCheckUpdate = new CheckUpdatePushButton(this, ":/resource/update_txt_btn");
//    m_pcilAutoProcVirus = new CheckboxImageLabel(this, ":/resource/auto_process_txt");
//    m_pcilShutdownAfterScan = new CheckboxImageLabel(this, ":/resource/auto_shutdown_txt");


    m_plVersion->setText("(3.0.0.73)");
    m_plUpdatetime->setText("2017-11-26 09:13:02");

    m_plVersion->setGeometry(APP_BOTBODY_LEFT_EDGE+APP_BOTBODY_IMG_TXT_SPACE+m_ppVersion->width(),APP_BOTBODY_TOP_EDGE,APP_BOTBODY_VERSION_TXT_LEN,m_ppVersion->height());
    m_plUpdatetime->setGeometry(APP_BOTBODY_LEFT_EDGE+m_ppVersion->width()+m_ppUpdateTime->width()+APP_BOTBODY_VERSION_TXT_LEN+APP_BOTBODY_IMG_TXT_SPACE*2,
                                APP_BOTBODY_TOP_EDGE,APP_BOTBODY_UPDATE_TXT_LEN,m_ppUpdateTime->height());
//    m_cupbCheckUpdate->setGeometry(APP_BOTBODY_LEFT_EDGE+m_ppVersion->width()+APP_BOTBODY_VERSION_TXT_LEN+APP_BOTBODY_IMG_TXT_SPACE+APP_BOTBODY_UPDATE_TXT_LEN+50,
//                                 APP_BOTBODY_TOP_EDGE,m_cupbCheckUpdate->m_ppBackgroundImage->width()/3,m_cupbCheckUpdate->m_ppBackgroundImage->height());

//    m_pcilAutoProcVirus->setGeometry(APP_SCREEN_WIDTH-300,0,m_pcilAutoProcVirus->m_nWidth,m_pcilAutoProcVirus->m_nHeight);
//    m_pcilShutdownAfterScan->setGeometry(APP_SCREEN_WIDTH-150,0,m_pcilShutdownAfterScan->m_nWidth,m_pcilShutdownAfterScan->m_nHeight);
    QFont font;
    font.setPixelSize(10);
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#ifdef QT_NEW_HEADER_54
    font.setWeight(0);
    m_plVersion->setFont(font);
    m_plUpdatetime->setFont(font);
#else
    font.setWeight(25);
    m_plVersion->setFont(font);
    m_plUpdatetime->setFont(font);
#endif
#else  //QT_VERSION
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
    m_plVersion->setFont(font);
    m_plUpdatetime->setFont(font);
#else
    font.setWeight(QFont::Light);
    m_plVersion->setFont(font);
    m_plUpdatetime->setFont(font);
#endif
#endif //QT_VERSION
//    m_cupbCheckUpdate->setCursor(Qt::PointingHandCursor);

//    connect(this->m_cupbCheckUpdate, &CheckUpdatePushButton::clicked, this, &ScanBotBodyWidget::ClickUpdate);
}

void ScanBotBodyWidget::SetVersion(QString sVersion)
{
    m_sVersion = sVersion;
    m_plVersion->setText(sVersion);
}

void ScanBotBodyWidget::SetUpdateTime(QString sTime)
{
    m_sUpdateTime = sTime;
    m_plUpdatetime->setText(sTime);
}

bool ScanBotBodyWidget::GetAutoProcVirus()
{
    return true;
}

bool ScanBotBodyWidget::GetShutdownAfterScan()
{
    return true;
}

void ScanBotBodyWidget::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);

    QPainter painter(this);
    painter.drawPixmap(APP_BOTBODY_LEFT_EDGE,APP_BOTBODY_TOP_EDGE,m_ppVersion->width(),m_ppVersion->height(), *m_ppVersion);
    painter.drawPixmap(APP_BOTBODY_LEFT_EDGE+m_ppVersion->width()+APP_BOTBODY_VERSION_TXT_LEN+APP_BOTBODY_IMG_TXT_SPACE,
                       APP_BOTBODY_TOP_EDGE,m_ppUpdateTime->width(), m_ppUpdateTime->height(), *m_ppUpdateTime);
}

void ScanBotBodyWidget::ClickUpdate(bool bChecked)
{
    Q_UNUSED(bChecked);

    qDebug("ScanBotBodyWidget::ClickUpdate event");

}
