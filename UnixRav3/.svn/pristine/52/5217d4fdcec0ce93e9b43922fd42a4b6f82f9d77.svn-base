#include "dialog.h"
#include "ui_dialog.h"
#include "guiuninstaller.h"

#include <QPushButton>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>

#include <string>

using namespace std;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QApplication::setApplicationName("Uninstall");
    setWindowTitle("程序卸载");

    m_bVerifyPasswdOK = false;
    m_pbtnOK = ui->btnOK;
    m_plePasswd = ui->lePasswd;
    m_pUninstaller = new GuiUninstaller;
    m_pUninstaller->GetInstallInfo();

    connect(m_pbtnOK, SIGNAL(clicked(bool)), this, SLOT(OnClickedOK()));

    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
}

Dialog::~Dialog()
{
    delete m_pUninstaller;
    m_pUninstaller = NULL;
    delete ui;
}

void Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Dialog::OnClickedOK()
{
    qDebug() << "Dialog::OnClickedOK() text=" << m_plePasswd->text() << endl;

    string spass = m_plePasswd->text().toStdString();
    if (m_pUninstaller->GuiUninstallPermit(spass.c_str())){
        m_pUninstaller->QuietUninstall();
        QMessageBox::information(this,"卸载","卸载成功！");
    }else{
        QMessageBox::information(this,"卸载","卸载失败！");
    }
    close();
}


