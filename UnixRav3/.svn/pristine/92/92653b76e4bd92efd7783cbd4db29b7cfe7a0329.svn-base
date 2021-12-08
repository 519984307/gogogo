#include "offlineupdateinfodialog.h"

#include <QPainter>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>

#include <QPushButton>
#include <QLabel>

OfflineUpdateInfoDialog::OfflineUpdateInfoDialog(QWidget *parent)
    :QDialog(parent)
{
    setFixedSize(300, 200);
    Qt::WindowFlags flags = this->windowFlags();
    setWindowFlags(flags|Qt::Popup);

    m_pbtnOK = new QPushButton(this);
    m_pbtnOK->setText(QString::fromUtf8("确定"));
    m_plInfo = new QLabel(this);
    m_plInfo->setText(QString::fromUtf8("请稍等..."));

    m_pbtnOK->setGeometry(140, 150, 30, 20);
    m_plInfo->setGeometry(130, 100, 50, 20);

    connect(m_pbtnOK, SIGNAL(clicked(bool)), this, SLOT(onClickedOK()));
}

void OfflineUpdateInfoDialog::onClickedOK()
{
    this->setWindowFlags(this->windowFlags() &~ Qt::WindowStaysOnTopHint);
    this->hide();
}

void OfflineUpdateInfoDialog::onUpdateOK()
{
    m_plInfo->setText(QString::fromUtf8("升级完成！"));
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    update();
}

void OfflineUpdateInfoDialog::onUpdateFail()
{
    m_plInfo->setText(QString::fromUtf8("格式错误！"));
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    update();
}

void OfflineUpdateInfoDialog::onUpdateWait()
{
    m_plInfo->setText(QString::fromUtf8("请稍等..."));
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    update();
}
