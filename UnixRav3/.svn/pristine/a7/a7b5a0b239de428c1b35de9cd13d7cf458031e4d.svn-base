#include <QGridLayout>
#include <QHBoxLayout>

#include "common/pubdef.h"
#include "licensegroup.h"


LicenseGroup::LicenseGroup(QWidget *parent):QGroupBox(parent),m_bFocus(false)
{
    setTitle(u8"激活授权");
    QFont font("Arial", 10, QFont::Thin, false);
    setFont(font);

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
    setFixedHeight(120);
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

    QLabel *lActiveNO = new QLabel(u8"授权码：", this);
    lActiveNO->setFont(font);
    lActiveNO->adjustSize();
    m_pleLicenseNumber = new QLineEdit(this);
    m_pleLicenseNumber->setFont(font);
    m_pleLicenseNumber->setPlaceholderText(u8"请输入授权码");

    lActiveNO->setGeometry(25, 30, lActiveNO->width(), 25);
    m_pleLicenseNumber->setGeometry(110, 30, 250, 25);

    m_plInfoIco = new QLabel("xxxxx", this);
    m_plInfoIco->setPixmap(QPixmap(":/Images/info_ico"));
    //m_plInfoIco->adjustSize();
    m_plInfoText = new QLabel(this);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    m_plInfoText->setPalette(pa);
    QFont font1("Arial", 8, QFont::Thin, false);
    m_plInfoText->setFont(font1);
    m_plInfoText->setText(u8"授权码错误，请重新输入！");
    m_plInfoText->adjustSize();

    m_plInfoIco->setGeometry(110+m_pleLicenseNumber->width()+5, 30, m_plInfoIco->width(), lActiveNO->height());
    m_plInfoText->setGeometry(110+m_pleLicenseNumber->width()+5, 30, m_plInfoText->width(), lActiveNO->height());

//    m_plInfoIco->hide();
//    m_plInfoText->hide();

    m_pibActiveLicense = new ImgButton(this);
    m_pibActiveLicense->setGeometry(130, 70, m_pibActiveLicense->width(), m_pibActiveLicense->height());
    m_pibActiveLicense->setDisabled(true);

    SetFocus(m_bFocus);
}

void LicenseGroup::SetFocus(bool bFocus)
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

void LicenseGroup::SetSizeEditValid()
{

}
