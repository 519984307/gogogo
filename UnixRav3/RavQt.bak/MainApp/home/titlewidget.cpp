#include "titlewidget.h"
#include "showbutton.h"
#include "pushbutton.h"
#include "common.h"

#include <QHBoxLayout>
#include <QSignalMapper>
#include <QMouseEvent>
#include <QPalette>

TitleWidget::TitleWidget(QWidget *parent) : QWidget(parent)
{
    // 构造各个对象
    mLogo = new ShowButton();
    mLogo->loadPixmap(":/resource/logo");
    setFixedHeight(mLogo->height());
    //qDebug("TitleWidet::TitleWidget mLogo->height=%d", mLogo->height());
    //mTitle = new ShowButton();
    //mTitle->loadPixmap(":/resource/title");
    mTitleMajor = new QLabel();
    mTitleMinor = new QLabel();
	QPalette pal = palette(); 
	pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00)); 
    pal.setColor(QPalette::WindowText, QColor(Qt::white));
    mTitleMajor->setPalette(pal);
    mTitleMinor->setPalette(pal);
    QFont font;
    font.setPixelSize(14);
    font.setWeight(QFont::Bold);
    mTitleMajor->setFont(font);
    mTitleMajor->setText("Rising ESM");
    font.setPixelSize(13);
    font.setWeight(QFont::Normal);
    mTitleMinor->setFont(font);
    mTitleMinor->setText("");

    //mOrganization = new MoreStateTextButton();
    //mOrganization->loadPixmap(":/resource/organization");
    //mOrganization->setText("Hello");

    QHBoxLayout* toolsLayout = initToolArea();
    // logo
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    mainLayout->addSpacing(LOGO_LEFT_MARGIN);
    mainLayout->addWidget(mLogo, 0, Qt::AlignLeft);

    //mainLayout->addWidget(mOrganization, 0, Qt::AlignCenter);

    mainLayout->addSpacing(10);
    mainLayout->addWidget(mTitleMajor, 0, Qt::AlignLeft);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(mTitleMinor, 0, Qt::AlignLeft);
    mainLayout->addStretch();
    mainLayout->addLayout(toolsLayout);

    setLayout(mainLayout);
}

void TitleWidget::UpdateTitle(const char *majorTitle, const char *minorTitle)
{
    if (majorTitle == NULL || minorTitle == NULL) {
       return;
    }
    mTitleMajor->setText(majorTitle);
    mTitleMinor->setText(minorTitle);
    update();
}

void TitleWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(QColor(210,122,62)));
    setPalette(pal);
}

QHBoxLayout* TitleWidget::initToolArea()
{
    //mMiniBtn  = new PushButton();
    mCloseBtn = new PushButton();
    mSettingBtn = new PushButton();
    mMenuBtn = new PushButton();

    //mMiniBtn->loadPixmap(":/resource/mini_btn");
    mCloseBtn->loadPixmap(":/resource/close_btn");
    mSettingBtn->loadPixmap(":/resource/setting_btn");
    mMenuBtn->loadPixmap(":/resource/menu_btn");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,APP_EDGE_WIDTH,0,0);
    layout->addWidget(mSettingBtn, 0, Qt::AlignTop|Qt::AlignRight);
    layout->addWidget(mMenuBtn, 0, Qt::AlignTop|Qt::AlignRight);
    //layout->addWidget(mMiniBtn, 0, Qt::AlignTop|Qt::AlignRight);
    layout->addWidget(mCloseBtn, 0, Qt::AlignTop|Qt::AlignRight);

//    QWidget *widget = new QWidget();
//    widget->setLayout(layout);

//    return widget;
    return layout;
}

TitleWidget::~TitleWidget()
{

}


