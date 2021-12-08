#include <QPalette>
#include <QPainter>
#include <QProcess>
#include <QCoreApplication>
#include <QtGlobal>

#include "common.h"
#include "pubfunc.h"

#include "virusmessagebox.h"

VMessageTitleWidget::VMessageTitleWidget(QWidget *parent):QWidget(parent)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(43, 18, 79));
    pal.setColor(QPalette::WindowText,Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    QFont font;
    font.setPixelSize(11);
    font.setWeight(QFont::Normal);
    m_plTitle = new QLabel(QString::fromUtf8("发现病毒"));
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

VMessageTitleWidget::~VMessageTitleWidget()
{
    delete m_plTitle;
    m_plTitle = NULL;
}

void VMessageTitleWidget::SetTitle(QString sTitle)
{
    m_plTitle->setText(sTitle);
    update();
}

VirusMessageDialog::VirusMessageDialog(QWidget *parent)
    :QDialog(parent)
{

    setStyleSheet("QDialog{  \
            background:rgb(237,237,237);\
            border-width: 1px 1px 1px 1px;\
            border-style: solid;\
            border-radius:1px;\
         }");
    setFixedSize(400, 270);

    setWindowFlags(Qt::FramelessWindowHint);
    //setStyleSheet("background-image:url(:/resource/warn_bk);");
    m_sTitle = "";
    m_sVirus = "";
    m_sFile = "";
    InitDialog();
}

VirusMessageDialog::~VirusMessageDialog()
{

    delete m_pHeader;
    m_pHeader = NULL;
    delete m_plVirus;
    m_plVirus = NULL;
    delete m_plFile;
    m_plFile = NULL;
    delete m_plWarning;
    m_plWarning = NULL;
    delete m_plIcon;
    m_plIcon = NULL;
    delete m_ppbKill;
    m_ppbKill = NULL;
    delete m_ppbIgnore;
    m_ppbIgnore = NULL;

    delete vlayout;
    vlayout = NULL;
//    delete h0layout;
//    h0layout = NULL;
//    delete h1layout;
//    h1layout = NULL;
//    delete h2layout;
//    h2layout = NULL;
//    delete h3layout;
//    h3layout = NULL;
}

void VirusMessageDialog::SetMessage(QString sTitle, QString sVName, QString sFile)
{
    m_sTitle = sTitle;
    m_sVirus = sVName;
    m_sFile = sFile;
    m_pHeader->SetTitle(sTitle);
    m_plVirus->setText(sVName);
    m_plFile->setText(sFile);

    update();
}

int VirusMessageDialog::Value()
{
    return m_nRetValue;
}

bool VirusMessageDialog::SameAction()
{
    return m_pclSameAction->IsChecked();
}

void VirusMessageDialog::OnClickedKill()
{
    m_nRetValue = 1;
    accept();
}

void VirusMessageDialog::OnClickedIgnore()
{
    m_nRetValue = 0;
    reject();
}

void VirusMessageDialog::InitDialog()
{
    m_pHeader = new VMessageTitleWidget;
    m_pHeader->setFixedWidth(width());

    m_plIcon = new QLabel;
    m_plVirus = new QLabel;
    m_plFile = new QLabel;
    m_plVirus->adjustSize();
    //m_plVirus->setWordWrap(true);
    m_plFile->adjustSize();
    m_plFile->setFixedWidth(width() - 100);
    m_plFile->setWordWrap(true);
    m_plWarning = new QLabel;
    m_plWarning->adjustSize();
    m_plWarning->setText(QString::fromUtf8("发现病毒!"));
    m_plIcon->setPixmap(QPixmap(":/resource/warning_icon"));
    m_ppbKill = new QPushButton(QString::fromUtf8("立即查杀"));
    m_ppbIgnore = new QPushButton(QString::fromUtf8("忽略"));
    m_pclSameAction = new CheckboxLabel(this, u8"以后采用同此次一样的行为方式处理");

    vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0, 0, 0, 0);

    h0layout = new QHBoxLayout;
    h0layout->addSpacing(80);
    h0layout->addWidget(m_plIcon, 0, Qt::AlignLeft);
    h0layout->addSpacing(10);
    h0layout->addWidget(m_plWarning, 0, Qt::AlignLeft);
    h0layout->addStretch();

    h1layout = new QHBoxLayout;
    h1layout->setContentsMargins(20, 10, 20, 0);
    h1layout->addWidget(m_plVirus, 0, Qt::AlignLeft);
    h2layout = new QHBoxLayout;
    h2layout->setContentsMargins(20, 0, 20, 10);
    h2layout->addWidget(m_plFile, 0, Qt::AlignLeft);
    h3layout = new QHBoxLayout;
    h3layout->addStretch();
    h3layout->addWidget(m_ppbKill);
    h3layout->addSpacing(70);
    h3layout->addWidget(m_ppbIgnore);
    h3layout->addStretch();

    vlayout->addWidget(m_pHeader, 0, Qt::AlignTop);
    vlayout->addLayout(h0layout);
    vlayout->addSpacing(10);
    vlayout->addLayout(h1layout);
    vlayout->addLayout(h2layout);
    vlayout->addLayout(h3layout);
    vlayout->addSpacing(10);
    vlayout->addWidget(m_pclSameAction, 0, Qt::AlignHCenter);
    vlayout->addSpacing(20);
    setLayout(vlayout);

    connect(m_ppbKill, SIGNAL(clicked(bool)), this, SLOT(OnClickedKill()));
    connect(m_ppbIgnore, SIGNAL(clicked(bool)), this, SLOT(OnClickedIgnore()));
}

void VirusMessageDialog::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);
//    QPainter painter(this);
//    painter.setPen(Qt::white);
//    painter.setFont(QFont( "Timers" , 8 ,  QFont::Thin));

}
