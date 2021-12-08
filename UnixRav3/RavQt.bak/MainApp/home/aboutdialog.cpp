#include <QPalette>
#include <QPainter>
#include <QProcess>
#include <QCoreApplication>
#include <QtGlobal>
#include <QInputDialog>

#include "common.h"
#include "aboutdialog.h"
#include "mainwindow.h"

AboutDialog::AboutDialog(QWidget *parent)
    :QDialog(parent)
{
    // TODO 在这里做一个强转，狗屎一样的设计，但也没办法，后期改进
    mParent = (MainWindow*)parent;

    //setWindowFlags(Qt::WStyle_Customize | WStyle_NormalBorder);
    LoadPixmap(":/resource/about_bk");
    setFixedSize(m_nWidth, m_nHeight);

    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background-image:url(:/resource/about_bk);");

//    m_sVersion = "3.0.0.73";
//    m_sVirusVersion = "29.1128.0001";
//    m_sUpdatedTime = "2017-11-26 09:13:02";
//    m_sVDBUpdatedTime = "2018-11-11 11:11:11";

    InitDialog();
}

void AboutDialog::SetProductName(const char* sMajorName, const char* sMinorName)
{
    m_sMajorName = sMajorName;
    m_sMinorName = sMinorName;
    m_plProductName->setText(m_sMajorName);
    m_plProductMinorName->setText(m_sMinorName);
    m_plProductName->adjustSize();
    m_plProductMinorName->adjustSize();
    update();
}

void AboutDialog::SetVersion(QString &sVersion, QString &sUpdatedTime, QString &sVVersion, QString &sVDBUpdatedTime)
{
    m_sVersion = sVersion;
    m_sUpdatedTime = sUpdatedTime;
    m_sVirusVersion = sVVersion;
    m_sVDBUpdatedTime = sVDBUpdatedTime;
    update();
}

void AboutDialog::SetVersion(QString &sVersion, QString &sUpdatedTime, QString &sVVersion, QString &sVDBUpdatedTime, QString &serial)
{
    m_sVersion = sVersion;
    m_sUpdatedTime = sUpdatedTime;
    m_sVirusVersion = sVVersion;
    m_sVDBUpdatedTime = sVDBUpdatedTime;
    mSerialNumber = serial;

    update();
}


void AboutDialog::LicenseClicked()
{
    //qDebug("AboutDialog::LicenseClicked");
#ifdef Q_OS_LINUX   // linux
//    QProcess *process = new QProcess;
//    QString scmd("/usr/lib/firefox-esr/firefox-esr ");
//    scmd += QCoreApplication::applicationDirPath() + "/license.txt";
//    process->start(scmd);


#endif
}

void AboutDialog::LoadPixmap(QString sBackgroundImage)
{
    m_ppBackgroundImage = new QPixmap;
    bool bret = m_ppBackgroundImage->load(sBackgroundImage);
    if(!bret){
//        qDebug("AboutDailog::LoadPixmap sBackgroundImage fail");
    }
    m_nWidth = m_ppBackgroundImage->width();
    m_nHeight = m_ppBackgroundImage->height();
}

void AboutDialog::InitDialog()
{
    m_plProductName = new QLabel(this);
    m_plProductMinorName = new QLabel(this);
    mSerialNumberLabel = new QLabel(this);

    QFont font;
    font.setPixelSize(16);
    font.setWeight(QFont::Light);
    m_plProductName->setFont(font);

    QPalette pal = palette();
    pal.setColor(QPalette::WindowText,Qt::white);
    m_plProductName->setPalette(pal);

    //set font size and color
    m_plProductName->setText(m_sMajorName);
    m_plProductName->adjustSize();
    m_plProductName->setGeometry(215, 65, m_plProductName->width(), m_plProductName->height());

    QFont font1;
    font1.setPixelSize(12);
    QPalette pal1 = palette();
    pal1.setColor(QPalette::WindowText, Qt::white);
    m_plProductMinorName->setFont(font1);
    m_plProductMinorName->setPalette(pal1);
    m_plProductMinorName->setText(m_sMinorName);
    m_plProductMinorName->adjustSize();
    m_plProductMinorName->setGeometry(215, 90, m_plProductMinorName->width(), m_plProductMinorName->height());

    mSerialNumberLabel->setText(mSerialNumber);
//    m_plProductMinorName->setGeometry(215, 115, m_plProductMinorName->width(), m_plProductMinorName->height());

    m_ppbClose = new PushButton;
    m_ppbClose->setParent(this);
    m_ppbClose->loadPixmap(":/resource/close_btn");
    m_ppbClose->setGeometry(m_nWidth-m_ppbClose->width()-4,4, m_ppbClose->width(), m_ppbClose->height());

    connect(m_ppbClose, SIGNAL(clicked(bool)), this, SLOT(close()));

    m_pibnOK = new ImageButton(":/resource/ok", this);
    m_pibnOK->setGeometry(m_nWidth-m_pibnOK->width()-25, m_nHeight-m_pibnOK->height()-15, m_pibnOK->width(), m_pibnOK->height());

    connect(m_pibnOK, SIGNAL(clicked(bool)), this, SLOT(close()));

//    if (mSerialNumber.isEmpty()) {
//        m_pLicenseBtn = new LicenseButton(":/resource/license_btn", this);
//        m_pLicenseBtn->setGeometry(255,m_nHeight-31,m_pLicenseBtn->width(), m_pLicenseBtn->height());
//        connect(m_pLicenseBtn, SIGNAL(clicked(bool)), this, SLOT(LicenseClicked()));
//    }
}

void AboutDialog::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);
    QPainter painter(this);
    painter.setPen(Qt::white);
    QFont font;
    font.setPixelSize(10);
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#ifdef QT_NEW_HEADER_54
    font.setWeight(0);
    painter.setFont(font);
#else
    font.setWeight(25);
    painter.setFont(font);
#endif
#else
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
    painter.setFont(font);
#else
    font.setWeight(QFont::Light);
    painter.setFont(font);
#endif
#endif
    painter.drawText(256, 139, u8"当前版本：");
    painter.drawText(312,139, m_sVersion);
    painter.drawText(256, 164, u8"升级时间：");
    painter.drawText(312,164, m_sUpdatedTime);
    painter.drawText(244, 189, u8"病毒库版本：");
    painter.drawText(312,189, m_sVirusVersion);
    painter.drawText(220, 214, u8"病毒库升级时间：");
    painter.drawText(312,214, m_sVDBUpdatedTime);

    if (!mSerialNumber.isEmpty()) {
        painter.drawText(244, 239, u8"产品序列号：");
        painter.drawText(312,239, mSerialNumber);
    }

}
