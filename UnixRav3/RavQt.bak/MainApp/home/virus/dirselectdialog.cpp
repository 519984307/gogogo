#include <QVBoxLayout>
#include <QDesktopWidget>


#include "dirselectdialog.h"
#include "mainwindow.h"

DirSelectDialog::DirSelectDialog(QWidget *parent) : QDialog(parent)
{
    m_pMainWindow = (MainWindow*)parent;
    setFixedSize(DIR_SELECT_DIALOG_WODTH, DIR_SELECT_DIALOG_HEIGHT);

    Qt::WindowFlags flags = this->windowFlags();
    setWindowFlags(flags|Qt::FramelessWindowHint|Qt::Popup);
    //setWindowModality(Qt::ApplicationModal);
    //setStyleSheet("background-image:url(:/resource/about_bk);");
    //setBackgroundRole(QPalette::Mid);
    //setWindowTitle("Dir select dialog");

    m_pHeader = new DirSelectHeaderWidget(this);
    m_pMidBody = new DirSelectMidBodyWidget(this, m_pMainWindow);
    m_pBotBody = new DirSelectBotBodyWidget(this);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(1,1,1,1);



    layout->addWidget(m_pHeader);
    layout->addWidget(m_pMidBody);
    layout->addWidget(m_pBotBody);
    setLayout(layout);


    connect(m_pHeader->m_pCloseBtn, SIGNAL(clicked(bool)), this, SLOT(CancelDialog()));
    connect(m_pBotBody->m_pBtn, SIGNAL(clicked(bool)), this, SLOT(CloseDialog()));

    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
}

void DirSelectDialog::CancelDialog()
{
    close();
}

void DirSelectDialog::CloseDialog()
{
    m_pMidBody->GetSelectedDir(&m_lDirs);
//    qDebug("DirSelectDialog size=%d ", m_lDirs.size());
    //setResult(QDialog::Accepted);
    accept();
    close();
}

void DirSelectDialog::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);
    QPainter painter(this);
    painter.drawRect(0, 0, DIR_SELECT_DIALOG_WODTH-1, DIR_SELECT_DIALOG_HEIGHT-1);
}
