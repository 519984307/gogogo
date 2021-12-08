#include "whitelistgroup.h"
#include "common/pubdef.h"

#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QStringList>
#include <QFileDialog>

#include <QDebug>

//#define WHITELIST_GROUP_WIDTH   620
#define WHITELIST_GROUP_HEIGHT  280

WhiteListGroup::WhiteListGroup(QWidget *parent)
    :QGroupBox(parent),m_bFocus(false)
{
    setTitle(u8"白名单");
    QFont font;
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
    font.setPointSize(10);
    setFont(font);

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
    setMaximumHeight(WHITELIST_GROUP_HEIGHT);

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

    InitAllWidget();
    SetFocus(m_bFocus);
}

void WhiteListGroup::InitAllWidget()
{
#ifdef QT_NEW_HEADER_54
    QFont font("Arial", 10, QFont::Thin, false);
#else
    QFont font("Arial", 10, QFont::Light, false);
#endif

    QGridLayout *glayout = new QGridLayout;

    QHBoxLayout *fdlayout = new QHBoxLayout;
    QLabel *pDFLabel = new QLabel(u8"文件/目录：", this);
    pDFLabel->setFont(font);
    m_ppbDirFile = new PushButton(this);
    m_ppbDirFile->loadPixmap(":/resource/plus_drop_btn");
    fdlayout->addWidget(pDFLabel, 0, Qt::AlignRight);
    fdlayout->addWidget(m_ppbDirFile, 0, Qt::AlignRight);
    glayout->addLayout(fdlayout, 0, 0, Qt::AlignRight);

    QVBoxLayout *all_layout = new QVBoxLayout;
    QHBoxLayout *extlayout = new QHBoxLayout;
    QLabel *pExtLabel = new QLabel(u8"文件后缀：", this);
    pExtLabel->setFont(font);
    m_pbExtName = new PushButton(this);
    m_pbExtName->loadPixmap(":/resource/plus_btn");
    extlayout->addWidget(pExtLabel, 0, Qt::AlignRight);
    extlayout->addWidget(m_pbExtName, 0, Qt::AlignRight);
    glayout->addLayout(extlayout, 0, 1, Qt::AlignRight);

    m_pdfDirFile = new DirFileTable(this);   
    glayout->addWidget(m_pdfDirFile, 1, 0);

    m_penExtName = new ExtNameTable(this);
    glayout->addWidget(m_penExtName, 1, 1);

    all_layout->addLayout(glayout);

    QLabel *pinfo = new QLabel;
    QPixmap *pxinfo = new QPixmap(":/resource/whitelist_info");
    pinfo->setPixmap(*pxinfo);
    pinfo->setFixedSize(pxinfo->width(), pxinfo->height());
    QHBoxLayout * pinfolayout = new QHBoxLayout;
    pinfolayout->addWidget(pinfo);
    pinfolayout->setAlignment(Qt::AlignLeft);

    all_layout->addLayout(pinfolayout);

    setLayout(all_layout);

    m_pMenu = new QMenu;
    m_pMenu->setStyleSheet(" QMenu::item {\
                           background-color: white;\
                           padding:2px 2px;\
                           margin:2px 2px;\
                           }\
                            QMenu::item:selected {\
                            background-color: rgb(228,221,235);\
                            }");
    m_paDirAndChild = new QAction(u8"目录+子目录", m_pMenu);
    m_paDir = new QAction(u8"目录", m_pMenu);
    m_paChild = new QAction(u8"子目录", m_pMenu);
    m_paFile = new QAction(u8"文件", m_pMenu);
    m_pMenu->addAction(m_paDirAndChild);
    m_pMenu->addAction(m_paDir);
    m_pMenu->addAction(m_paChild);
    m_pMenu->addAction(m_paFile);
    m_pMenu->setFixedWidth(100);
    connect(m_ppbDirFile, SIGNAL(clicked()), this, SLOT(PopDirFileMenu()));
    connect(m_pMenu, SIGNAL(triggered(QAction *)), this, SLOT(TrigerDirFileMenu(QAction *)));
    connect(m_pbExtName, SIGNAL(clicked()), this, SLOT(AddExtNameClicked()));

}

void WhiteListGroup::PopDirFileMenu()
{
    m_pMenu->exec(QCursor::pos());
}

void WhiteListGroup::TrigerDirFileMenu(QAction *pAction)
{
    if(pAction == m_paDirAndChild){
        MenuDirFChildClicked();
    }else if(pAction == m_paDir){
        MenuDirClicked();
    }else if(pAction == m_paChild){
        MenuChildClicked();
    }else if(pAction == m_paFile){
        MenuFileClicked();
    }
}

void WhiteListGroup::MenuDirFChildClicked()
{
    DirSelectDialog dlg(this);
    dlg.m_nType = APP_MENU_DIRSEL_TYPE_DIR_CHILD;
    if(dlg.exec() == QDialog::Accepted){
        m_pdfDirFile->AddItem(dlg.m_nType, dlg.m_sName);
    }
}

void WhiteListGroup::MenuDirClicked()
{
    DirSelectDialog dlg(this);
    dlg.m_nType = APP_MENU_DIRSEL_TYPE_DIR;
    if(dlg.exec() == QDialog::Accepted){
        m_pdfDirFile->AddItem(dlg.m_nType, dlg.m_sName);
    }
}

void WhiteListGroup::MenuChildClicked()
{
    DirSelectDialog dlg(this);
    dlg.m_nType = APP_MENU_DIRSEL_TYPE_CHILD;
    if(dlg.exec() == QDialog::Accepted){
        m_pdfDirFile->AddItem(dlg.m_nType, dlg.m_sName);
    }
}

//"选择文件"
void WhiteListGroup::MenuFileClicked()
{
    QStringList ltFilePath;
    QStringList fileNames;
    QFileDialog dialog(this);
    dialog.setWindowTitle(u8"选择目录");
    if (dialog.exec() == QDialog::Accepted){
        fileNames = dialog.selectedFiles();
        if(fileNames.size() == 0)
            return;
        for(int i=0; i<fileNames.size(); i++){
            m_pdfDirFile->AddItem(APP_MENU_DIRSEL_TYPE_FILE, fileNames.at(i));
        }
    }
    //qDebug() << fileNames;
}

void WhiteListGroup::AddExtNameClicked()
{
    //m_penExtName->setEditTriggers(QAbstractItemView::DoubleClicked);
    m_penExtName->AddExtNameStart();
    //for(int i=0; i)
}

void WhiteListGroup::SetFocus(bool bFocus)
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
