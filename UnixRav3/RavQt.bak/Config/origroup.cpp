#include <QGridLayout>
#include <QHBoxLayout>
#include <QLineEdit>

#include "common/pubdef.h"
#include "origroup.h"
#include "mainwindow.h"


#define ORI_LEFT_LABEL_WIDTH    150
#define ORI_RIGHT_CHKBOX_WIDTH  180

OriGroup::OriGroup(QWidget *parent, MainWindow *pMainWindow):QGroupBox(parent),m_bFocus(false)
{
    m_pMainWindow = pMainWindow;

    m_CenterAddr = NULL;

    setTitle(u8"常规项");
    QFont font;
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
    font.setPointSize(10);
    setFont(font);

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
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

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setAlignment(Qt::AlignLeft);
    hlayout->addSpacing(15);
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setAlignment(Qt::AlignLeft);
    vlayout->addSpacing(10);

    QGridLayout *layout = new QGridLayout;
    layout->setHorizontalSpacing(10);

    QLabel  *lRunEnv = new QLabel(u8"运行环境：", this);
    lRunEnv->setFont(font);
    lRunEnv->setFixedWidth(ORI_LEFT_LABEL_WIDTH);
    m_pRunEnv = new CheckboxLabel(this, u8"运行环境智能判断");
    //m_pRunEnv->setFixedWidth(ORI_RIGHT_CHKBOX_WIDTH);
    layout->addWidget(lRunEnv, 0, 0, 1, 0,Qt::AlignLeft);
    layout->addWidget(m_pRunEnv, 0, 1, 1, 0, Qt::AlignLeft);

    QLabel  *lVirusTrace = new QLabel(u8"病毒跟踪：", this);
    lVirusTrace->setFont(font);
    lVirusTrace->setFixedWidth(ORI_LEFT_LABEL_WIDTH);
    m_pVirusTrace = new CheckboxLabel(this, u8"启动病毒跟踪");
    //m_pVirusTrace->setFixedWidth(ORI_RIGHT_CHKBOX_WIDTH);
    layout->addWidget(lVirusTrace, 1, 0, 1, 0, Qt::AlignLeft);
    layout->addWidget(m_pVirusTrace, 1, 1, 1, 0, Qt::AlignLeft);

    QLabel  *lVirusLog = new QLabel(u8"病毒日志：", this);
    lVirusLog->setFont(font);
    lVirusLog->setFixedWidth(ORI_LEFT_LABEL_WIDTH);
    m_pVirusLog = new CheckboxLabel(this, u8"记录病毒日志");
    //m_pVirusLog->setFixedWidth(ORI_RIGHT_CHKBOX_WIDTH);
    layout->addWidget(lVirusLog, 2, 0, 1, 0, Qt::AlignLeft);
    layout->addWidget(m_pVirusLog, 2, 1, 1, 0, Qt::AlignLeft);

    QLabel  *lBuffer = new QLabel(u8"扫描缓存：", this);
    lBuffer->setFont(font);
    lBuffer->setFixedWidth(ORI_LEFT_LABEL_WIDTH);
    m_pBuffer = new CheckboxLabel(this, u8"二次扫描加速");
    layout->addWidget(lBuffer, 3, 0, 1, 0, Qt::AlignLeft);
    layout->addWidget(m_pBuffer, 3, 1, 1, 0, Qt::AlignLeft);

    QLabel  *lCloud = NULL;
    if (m_pMainWindow->IsSecretMachine()) {
        lCloud = new QLabel(u8"中心地址：", this);
        lCloud->setFont(font);

        m_CenterAddr = new QLineEdit(this);
        m_CenterAddr->setAlignment(Qt::AlignLeft);
        m_CenterAddr->setFont(font);

        layout->addWidget(lCloud, 4, 0, 1, 0, Qt::AlignLeft);
        layout->addWidget(m_CenterAddr, 5, 0, 1, 0, Qt::AlignLeft);
    }
// 设置中去掉了公有云或者私有云的设置项， 如果需要把云扫描加上， 则把此处放开
//    else {
//        lCloud = new QLabel(u8"云引擎设置：", this);
//        lCloud->setFont(font);
//        lCloud->setFixedWidth(ORI_LEFT_LABEL_WIDTH);

//        m_pCloud = new CheckboxLabel(this, u8"开启公有云");
//        //m_pCloud->setFixedWidth(ORI_RIGHT_CHKBOX_WIDTH);
//        layout->addWidget(lCloud, 4, 0, 1, 0, Qt::AlignLeft);
//        layout->addWidget(m_pCloud, 4, 1, 1, 0, Qt::AlignLeft);
//    }

    vlayout->addLayout(layout);
    hlayout->addLayout(vlayout);

    // 设置中去掉了公有云或者私有云的设置项， 如果需要把云扫描加上， 则把此处放开
//    QLabel  *lCloud1 = NULL;
//    QHBoxLayout * haddlayout = NULL;
//    QHBoxLayout *htablelayout = NULL;
//    if (!(m_pMainWindow->IsSecretMachine() || m_pMainWindow->IsCenterLess())) {
//        lCloud1 = new QLabel(u8"云引擎设置：", this);
//        lCloud1->setFont(font);
//        lCloud1->adjustSize();
//        QLabel *lPrivate = new QLabel(u8"私有云", this);
//        lPrivate->setFont(font);
//        lPrivate->adjustSize();

//        m_ppbAdd = new PushButton(this);
//        m_ppbAdd->loadPixmap(":/resource/plus_btn");
//        haddlayout = new QHBoxLayout;
//        haddlayout->setSpacing(0);
//        haddlayout->addSpacing(15);
//        haddlayout->addWidget(lCloud1, 0, Qt::AlignLeft);
//        haddlayout->addWidget(lPrivate, 0, Qt::AlignLeft);
//        haddlayout->addStretch();
//        haddlayout->addWidget(m_ppbAdd, 0, Qt::AlignRight);
//        haddlayout->addSpacing(10);

//        m_pPrivateTable = new PrivateCloudTable(this);
//        m_pPrivateTable->setFixedWidth(width() - 40);
//        htablelayout = new QHBoxLayout;
//        htablelayout->addWidget(m_pPrivateTable, 0, Qt::AlignCenter);
//    }

    QVBoxLayout *valllayout = new QVBoxLayout;
    valllayout->addLayout(hlayout);

    // 设置中去掉了公有云或者私有云的设置项， 如果需要把云扫描加上， 则把此处放开
//    if (!m_pMainWindow->IsSecretMachine()) {
//        valllayout->addLayout(haddlayout);
//        valllayout->addLayout(htablelayout);
//    }

    setLayout(valllayout);

    SetFocus(m_bFocus);

    // 设置中去掉了公有云或者私有云的设置项， 如果需要把云扫描加上， 则把此处放开
//    if (!(m_pMainWindow->IsSecretMachine() || m_pMainWindow->IsCenterLess())) {
//        connect(m_ppbAdd, SIGNAL(clicked(bool)), this, SLOT(OnClickedAdd()));
//    }
}

void OriGroup::SetFocus(bool bFocus)
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

void OriGroup::OnClickedAdd()
{
    m_pPrivateTable->AddCloudStart();
}
