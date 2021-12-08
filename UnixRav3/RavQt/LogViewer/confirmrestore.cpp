#include <QPalette>
#include <QPainter>
#include <QProcess>
#include <QCoreApplication>
#include <QtGlobal>

#include "pubdef.h"
#include "pubfunc.h"

#include "confirmrestore.h"

ConfirmTitleWidget::ConfirmTitleWidget(QWidget *parent):QWidget(parent)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(43, 18, 79));
    pal.setColor(QPalette::WindowText,Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    QFont font;
    font.setPixelSize(11);
    font.setWeight(QFont::Normal);
    m_plTitle = new QLabel(QString::fromUtf8("从隔离区恢复"));
    m_plTitle->setFont(font);
    m_plTitle->setPalette(pal);
    m_plTitle->adjustSize();
    setFixedHeight(38);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(10,0,0,0);
    layout->addWidget(m_plTitle, 0, Qt::AlignVCenter|Qt::AlignLeft);
    layout->addStretch();
    setLayout(layout);
}

ConfirmTitleWidget::~ConfirmTitleWidget()
{
    delete m_plTitle;
    m_plTitle = NULL;
}

void ConfirmTitleWidget::SetTitle(QString sTitle)
{
    m_plTitle->setText(sTitle);
    update();
}

ConfirmRestoreWidget::ConfirmRestoreWidget(QWidget *parent)
    :QDialog(parent)
{

    setStyleSheet("QDialog{  \
            background:rgb(237,237,237);\
            border-width: 1px 1px 1px 1px;\
            border-style: solid;\
            border-radius:1px;\
         }");
    setFixedSize(300, 160);

    setWindowFlags(Qt::FramelessWindowHint);
    m_sTitle = "从隔离区恢复";

    InitDialog();
}

ConfirmRestoreWidget::~ConfirmRestoreWidget()
{

    delete m_pHeader;
    m_pHeader = NULL;
    delete m_plWarning;
    m_plWarning = NULL;
    delete m_ppbOK;
    m_ppbOK = NULL;
    delete m_ppbCancle;
    m_ppbCancle = NULL;

    delete vlayout;
    vlayout = NULL;
}

void ConfirmRestoreWidget::SetMessage(QString sTitle, QString sVName, QString sFile)
{
    m_sTitle = sTitle;
    m_pHeader->SetTitle(sTitle);
    update();
}

int ConfirmRestoreWidget::Value()
{
    return m_nRetValue;
}

bool ConfirmRestoreWidget::IfAddWhiteList()
{
    return m_pclSameAction->m_bChecked;
}

void ConfirmRestoreWidget::OnClickedOK()
{
    m_nRetValue = 1;
    close();
}

void ConfirmRestoreWidget::OnClickedIgCancle()
{
    m_nRetValue = 0;
    close();
}

void ConfirmRestoreWidget::InitDialog()
{
    m_pHeader = new ConfirmTitleWidget;
    m_pHeader->setFixedWidth(width());

    m_plWarning = new QLabel;
    m_plWarning->adjustSize();
    m_plWarning->setText(QString::fromUtf8("确定要把选中的文件从隔离区恢复吗？"));
    m_ppbOK = new QPushButton(QString::fromUtf8("恢复"));
    m_ppbCancle = new QPushButton(QString::fromUtf8("取消"));
    m_pclSameAction = new CheckboxLabel(this, u8"同时把文件加入白名单");

    vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0, 0, 0, 0);

    h0layout = new QHBoxLayout;
    h0layout->addSpacing(30);
    h0layout->addWidget(m_plWarning, 0, Qt::AlignLeft);
    h0layout->addStretch();

    h3layout = new QHBoxLayout;
    h3layout->addStretch();
    h3layout->addWidget(m_ppbOK);
    h3layout->addSpacing(70);
    h3layout->addWidget(m_ppbCancle);
    h3layout->addStretch();

    vlayout->addWidget(m_pHeader, 0, Qt::AlignTop);
    vlayout->addLayout(h0layout);
    vlayout->addSpacing(10);
    vlayout->addLayout(h3layout);
    vlayout->addSpacing(10);
    vlayout->addWidget(m_pclSameAction, 0, Qt::AlignHCenter);
    vlayout->addSpacing(20);
    setLayout(vlayout);

    connect(m_ppbOK, SIGNAL(clicked(bool)), this, SLOT(OnClickedOK()));
    connect(m_ppbCancle, SIGNAL(clicked(bool)), this, SLOT(OnClickedIgCancle()));
}

void ConfirmRestoreWidget::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);
}
