#include <QPainter>
#include <QHBoxLayout>
#include <QDebug>

#include "botbodywidget.h"
#include "common.h"

#define APP_BOTBODY_LEFT_EDGE   30
#define APP_BOTBODY_TOP_EDGE    6
#define APP_BOTBODY_VERSION_TXT_LEN 80
#define APP_BOTBODY_UPDATE_TXT_LEN  140
#define APP_BOTBODY_IMG_TXT_SPACE   4


BotBodyWidget::BotBodyWidget(QWidget *parent, MainWindow *pMainWindow) : QWidget(parent)
{
    m_pMainWindow = pMainWindow;
    setFixedHeight(APP_BOT_BODY_HEIGHT);
    LoadPixmap();
    LoadStatusWidgets();
}

void BotBodyWidget::LoadPixmap()
{
    m_ppVersion = new QPixmap;
    bool bret = m_ppVersion->load(":/resource/version_txt");
    if(!bret){
//        qDebug("BotBodyWidget::LoadPixmap version fail");
    }
    m_ppUpdateTime = new QPixmap;
    bret = m_ppUpdateTime->load(":/resource/update_data_txt");
    if(!bret){
//        qDebug("BotBodyWidget::LoadPixmap updatetime fail");
    }
    m_plVersionImg = new QLabel;
    m_plVersionImg->setPixmap(QPixmap(":/resource/version_txt"));
    m_plVersionImg->setFixedSize(m_ppVersion->width(), m_ppVersion->height());
    m_plUpdatetimeImg = new QLabel;
    m_plUpdatetimeImg->setPixmap(QPixmap(":/resource/update_data_txt"));
    m_plUpdatetimeImg->setFixedSize(m_ppUpdateTime->width(), m_ppUpdateTime->height());
}

void BotBodyWidget::LoadStatusWidgets()
{
    //QRect pos = rect();
    //qDebug("status pos x,y,w,h: %d  %d  %d  %d", pos.x(), pos.y(), pos.width(), pos.height());
    m_plVersion = new QLabel;
    m_plUpdatetime = new QLabel;

    if (m_pMainWindow->IsSecretMachine()) {
        m_cupbCheckUpdate = new CheckUpdatePushButton(this, ":/resource/offline_update_btn");
        m_cupbCheckUpdate->setCursor(Qt::PointingHandCursor);
    }
//    m_pcilAutoProcVirus = new CheckboxImageLabel(this, ":/resource/auto_process_txt");
//    m_pcilShutdownAfterScan = new CheckboxImageLabel(this, ":/resource/auto_shutdown_txt");
    QFont font;
    font.setPixelSize(12);
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


    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(5,0,5,10);
    layout->addSpacing(20);
    layout->addWidget(m_plVersionImg, 0, Qt::AlignLeft|Qt::AlignVCenter);
    layout->addWidget(m_plVersion, 0, Qt::AlignLeft|Qt::AlignVCenter);
    layout->addSpacing(10);
    layout->addWidget(m_plUpdatetimeImg, 0, Qt::AlignLeft|Qt::AlignVCenter);
    layout->addWidget(m_plUpdatetime, 0, Qt::AlignLeft|Qt::AlignVCenter);
    layout->addSpacing(20);
    if (m_pMainWindow->IsSecretMachine()) {
        layout->addWidget(m_cupbCheckUpdate, 0, Qt::AlignLeft|Qt::AlignVCenter);
    }
    layout->addStretch();
//    layout->addWidget(m_pcilAutoProcVirus, 0, Qt::AlignRight|Qt::AlignVCenter);
//    layout->addSpacing(10);
//    layout->addWidget(m_pcilShutdownAfterScan, 0, Qt::AlignRight|Qt::AlignVCenter);
//    layout->addSpacing(20);
    setLayout(layout);

    m_plVersion->setText("(26.03.05)");
    m_plVersion->adjustSize();
    m_plUpdatetime->setText("2018-04-24 09:13:02");
    m_plUpdatetime->adjustSize();


}

void BotBodyWidget::SetVersion(QString sVersion)
{
    if(sVersion == "")
        return;
    m_sVersion = sVersion;
    m_plVersion->setText(m_sVersion);
    m_plVersion->adjustSize();
}

void BotBodyWidget::SetUpdateTime(QString sTime)
{
    if(sTime == "")
        return;
    m_sUpdateTime = sTime;
    m_plUpdatetime->setText(m_sUpdateTime);
    m_plUpdatetime->adjustSize();
}

//bool BotBodyWidget::GetAutoProcVirus()
//{
//    return true;
//}

//bool BotBodyWidget::GetShutdownAfterScan()
//{
//    return true;
//}

void BotBodyWidget::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);

//    QPainter painter(this);
//    painter.drawPixmap(APP_BOTBODY_LEFT_EDGE,APP_BOTBODY_TOP_EDGE,m_ppVersion->width(),m_ppVersion->height(), *m_ppVersion);
//    painter.drawPixmap(APP_BOTBODY_LEFT_EDGE+m_ppVersion->width()+APP_BOTBODY_VERSION_TXT_LEN+APP_BOTBODY_IMG_TXT_SPACE,
//                       APP_BOTBODY_TOP_EDGE,m_ppUpdateTime->width(), m_ppUpdateTime->height(), *m_ppUpdateTime);
}


