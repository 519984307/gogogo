#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QRect>
#include <QPixmap>
#include <QFileInfo>


#include "common/pubdef.h"
#include "dirselectdialog.h"

#define APP_DIRSELECTDIALOG_WIDTH   430
#define APP_DIRSELECTDIALOG_HEIGHT  386
#define APP_DIRSELECTDIALOG_MIN_WIDTH   420
#define APP_DIRSELECTDIALOG_MIN_HEIGHT  360


DirSelectDialog::DirSelectDialog(QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent,f)
{
    setStyleSheet("QDialog{  \
            background:rgb(212,208,200);\
            border-width: 1px 1px 1px 1px;\
            border-style: solid;\
            border-radius:1px;\
         }");
    resize(APP_DIRSELECTDIALOG_WIDTH, APP_DIRSELECTDIALOG_HEIGHT);
    setSizeGripEnabled(true);
    setWindowFlags(Qt::Popup);
    InitWidget();
    connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(reject()));
    //connect(m_pTreeView, SIGNAL(clicked()), this, SLOT(ClickedTree()));
    connect(m_pTreeView, SIGNAL(pressed(QModelIndex)), this, SLOT(ClickedTree(QModelIndex)));
}

void DirSelectDialog::InitWidget()
{
#ifdef QT_NEW_HEADER_54
    QFont font("Arial", 10, QFont::Thin, false);
#else
    QFont font("Arial", 10, QFont::Light, false);
#endif
    setFont(font);
    m_pTitle = new QLabel(u8" 浏览计算机");
    m_pTitle->setFont(font);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    m_pTitle->setPalette(pa);
    m_pTitle->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    m_pTitle->setFixedHeight(25);
    //m_pTitle->setMargin(0);
    //m_pTitle->setIndent(0);
    m_pTitle->setStyleSheet("background-color:rgb(87,118,172)");
    m_pTitle->setGeometry(0,0,width(), height());

    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    closeBtn = new QToolButton(m_pTitle);
    closeBtn->setIcon(closePix);
    closeBtn->setFixedSize(20,20);
    closeBtn->setGeometry(m_pTitle->width(),5,20,20);

    QLabel *ptxtsd = new QLabel(u8"选择目录");
    ptxtsd->setFixedHeight(26);
    m_pTreeView = new QTreeView;
    m_pDirList = new QStringList;
    m_pDirModel = new QDirModel;
    m_pDirModel->setReadOnly(true);
    m_pDirModel->setFilter(QDir::Drives|QDir::AllDirs|QDir::NoDotAndDotDot);
    //m_pDirModel->setSorting(QDir::Unsorted);

    m_pTreeView->setModel(m_pDirModel);
    m_pTreeView->header()->setVisible(false);
    QModelIndex index = m_pDirModel->index(QDir::currentPath());
    m_pTreeView->expand(index);
    m_pTreeView->scrollTo(index);
    m_pTreeView->resizeColumnToContents(0);

    QLabel * ptxtdir = new QLabel(u8"文件夹(&F)：");
    ptxtdir->setFixedSize(60, 26);
    m_pEdit = new QLineEdit;
    m_pEdit->setFixedSize(362,26);
    ptxtdir->setBuddy(m_pEdit);

    okBtn = new QPushButton(u8"确定");
    cancelBtn = new QPushButton(u8"取消");

    QHBoxLayout *ptlayout = new QHBoxLayout;
    ptlayout->setMargin(0);
    ptlayout->setSpacing(0);
    ptlayout->setContentsMargins(0,0,0,0);
    ptlayout->addWidget(m_pTitle);


    QHBoxLayout *pslayout = new QHBoxLayout;
    pslayout->addWidget(ptxtsd, 0, Qt::AlignLeft);

    QHBoxLayout *pmidlayout = new QHBoxLayout;
    pmidlayout->addWidget(ptxtdir, 0, Qt::AlignLeft);
    pmidlayout->addWidget(m_pEdit, 1, Qt::AlignLeft);

    QHBoxLayout *pendlayout = new QHBoxLayout;
    pendlayout->addStretch();
    pendlayout->addWidget(okBtn, 0, Qt::AlignRight);
    pendlayout->addWidget(cancelBtn, 0, Qt::AlignRight);

    QVBoxLayout *layout = new QVBoxLayout;
    //layout->addLayout(ptlayout);
    layout->setMargin(15);
    layout->addLayout(pslayout);
    layout->addWidget(m_pTreeView);
    layout->addLayout(pmidlayout);
    layout->addLayout(pendlayout);

    QVBoxLayout *mlayout = new QVBoxLayout;
    mlayout->setMargin(0);
    mlayout->addLayout(ptlayout);
    mlayout->addLayout(layout);

    setLayout(mlayout);

    //qDebug("width=%d title->width=%d btn-with=%d", width(), m_pTitle->width() ,closeBtn->width());
    //closeBtn->setGeometry(width()-closeBtn->width(),5,20,20);

}

void DirSelectDialog::mousePressEvent(QMouseEvent *e)
{
    m_PressPoint = e->pos();
    if(e->button()==Qt::LeftButton && m_pTitle->rect().contains(m_PressPoint)){
        m_bIsMove = true;
        return;
    }
//    if(e->button()==Qt::LeftButton && m_pTreeView->rect().contains(m_PressPoint)) {
//        QModelIndex index = m_pTreeView->currentIndex();
//        if (!index.isValid()) {
//            return;
//        }
//        qDebug("DirSelectDialog::mousePressEvent event");
//        QFileInfo fi = m_pDirModel->fileInfo(index);
//        if (fi.isDir()) {
//            m_pEdit->setText(fi.filePath());
//            m_sName = fi.filePath();
//            qDebug("Dir:%s", fi.filePath());
//        }
//    }
}

void DirSelectDialog::mouseMoveEvent(QMouseEvent *e)
{
    if((e->buttons() == Qt::LeftButton) && m_bIsMove)
    {
        QPoint pos_point = pos();
        pos_point.setX(pos_point.x() + e->x() - m_PressPoint.x());
        pos_point.setY(pos_point.y() + e->y() - m_PressPoint.y());
        move(pos_point);
    }
}

void DirSelectDialog::mouseReleaseEvent(QMouseEvent *)
{
    if(m_bIsMove)
    {
        m_bIsMove = false;
    }
}

void DirSelectDialog::ClickedTree(QModelIndex)
{
    //qDebug("DirSelectDialo ClickedTree begin");
    QModelIndex index = m_pTreeView->currentIndex();
    if (!index.isValid()) {
        return;
    }
    //qDebug("DirSelectDialog::ClickedTree event");
    QFileInfo fi = m_pDirModel->fileInfo(index);
    if (fi.isDir()) {
        m_pEdit->setText(fi.filePath());
        m_sName = fi.filePath();
        //qDebug("Dir:%s", fi.filePath());
    }

}
