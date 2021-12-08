#include <QGridLayout>
#include <QHBoxLayout>

#include "common/pubdef.h"
#include "upgradegroup.h"



UpgradeGroup::UpgradeGroup(QWidget *parent):QGroupBox(parent),m_bFocus(false)
{
    setTitle(u8"软件更新");
    QFont font("Arial", 10, QFont::Thin, false);
    setFont(font);

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
    setFixedHeight(215);
    setFlat(true);

    setStyleSheet("QGroupBox {\
                  font-size:13px; \
                  border: 2px solid #7B599F;\
                  margin-top: 1ex; \
                  }\
                  QGroupBox::title {\
                  top: 1px;\
                  left: 20px;\
                  subcontrol-origin: margin;\
                  subcontrol-position: top left;}");

    QLabel *lUpgradeMode = new QLabel(u8"升级模式：", this);
    lUpgradeMode->setFont(font);
    lUpgradeMode->adjustSize();
    m_pcbUpgradeMode = new ComboBox(this);
    m_pcbUpgradeMode->setFont(font);
    m_pcbUpgradeMode->insertItem(0, u8"自动升级");
    m_pcbUpgradeMode->insertItem(1, u8"手动升级");
    lUpgradeMode->setGeometry(25, 30, lUpgradeMode->width(), lUpgradeMode->height());
    m_pcbUpgradeMode->setGeometry(110, 30, m_pcbUpgradeMode->width(), lUpgradeMode->height());

    m_pbgUpgradeType = new QButtonGroup(this);

    QLabel *lUpgradeType = new QLabel(u8"升级内容：", this);
    lUpgradeType->setFont(font);
    lUpgradeType->adjustSize();
    m_pbgUpgradeType = new QButtonGroup(this);
    m_prbUpgradeAll = new RadioButton(u8"升级所有组件", this);
    m_prbUpgradeVirusDB = new RadioButton(u8"仅升级病毒库", this);
    m_pbgUpgradeType->addButton(m_prbUpgradeAll, APP_UPGRADE_ALL);
    m_pbgUpgradeType->addButton(m_prbUpgradeVirusDB, APP_UPGRADE_VIRUSDB);
    m_prbUpgradeAll->setFont(font);
    m_prbUpgradeVirusDB->setFont(font);
    lUpgradeType->setGeometry(25, 65, lUpgradeType->width(), lUpgradeType->height());
    m_prbUpgradeAll->setGeometry(110, 65, m_prbUpgradeAll->width(), lUpgradeType->height());
    m_prbUpgradeVirusDB->setGeometry(110, 90, m_prbUpgradeVirusDB->width(), lUpgradeType->height());

    QLabel *lProxySet = new QLabel(u8"代理设置：", this);
    lProxySet->setFont(font);
    lProxySet->adjustSize();
    m_pcbProxyType = new ComboBox(this);
    m_pcbProxyType->setFont(font);
    m_pcbProxyType->insertItem(0, u8"使用浏览器设置");
    m_pcbProxyType->insertItem(1, u8"直接连接");
    m_pcbProxyType->insertItem(2, u8"使用代理设置");
    m_pcbProxyType->adjustSize();
    lProxySet->setGeometry(25, 125, lProxySet->width(), lProxySet->height());
    m_pcbProxyType->setGeometry(110, 125, m_pcbProxyType->width(), lProxySet->height());

    QLabel *lAddress = new QLabel(u8"地址：", this);
    lAddress->adjustSize();
    m_pleAddress = new QLineEdit(this);
    m_pleAddress->setFont(font);
    QLabel *lPort = new QLabel(u8"端口：", this);
    lPort->adjustSize();
    m_plePort = new QLineEdit(this);
    m_plePort->setFont(font);
    QLabel *lUser = new QLabel(u8"账号：", this);
    lUser->adjustSize();
    m_pleUser = new QLineEdit(this);
    m_pleUser->setFont(font);
    QLabel * lPasswd = new QLabel(u8"密码：", this);
    lPasswd->adjustSize();
    m_plePassword = new QLineEdit(this);
    m_plePassword->setFont(font);
    lAddress->setGeometry(110, 150, lAddress->width(), lAddress->height());
    m_pleAddress->setGeometry(110+lAddress->width(), 150, 150, lAddress->height());
    lPort->setGeometry(110+lAddress->width()+20+150, 150, lAddress->width(), lAddress->height());
    m_plePort->setGeometry(110+lAddress->width()+20+150+lAddress->width(), 150, 150, lAddress->height());
    lUser->setGeometry(110, 175, lAddress->width(), lAddress->height());
    m_pleUser->setGeometry(110+lAddress->width(), 175, 150, lAddress->height());
    lPasswd->setGeometry(110+lAddress->width()+150+20, 175, lAddress->width(), lAddress->height());
    m_plePassword->setGeometry(110+lAddress->width()+150+20+lAddress->width(), 175, 150, lAddress->height());

    SetFocus(m_bFocus);
}

void UpgradeGroup::SetFocus(bool bFocus)
{
    m_bFocus = bFocus;
    if(m_bFocus){
        setStyleSheet("QGroupBox {\
              font-size:13px; \
              border: 2px solid #7B599F;\
              margin-top: 1ex; \
              }\
              QGroupBox::title {\
              top: 1px;\
              left: 20px;\
              subcontrol-origin: margin;\
              subcontrol-position: top left;}");

    }else{
        setStyleSheet("QGroupBox {\
              font-size:13px; \
              border: 2px solid #EFEBE7;\
              margin-top: 1ex; \
              }\
              QGroupBox::title {\
              top: 1px;\
              left: 20px;\
              subcontrol-origin: margin;\
              subcontrol-position: top left;}");
    }
    update();
}

void UpgradeGroup::SetSizeEditValid()
{
//    if(m_pclProcCompressed->m_bChecked == false){
//        m_pleSize->setDisabled(true);
//    }else{
//        m_pleSize->setDisabled(false);
//    }

}
