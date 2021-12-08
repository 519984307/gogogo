#include "popscreen.h"
#include "pushbutton.h"
#include "commonapi.h"
#include "baseconfig.h"
#include "communicatebundle.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>

#define POP_WIDTH   500
#define POP_HEIGHT  300

#ifdef TOP_STATUS_BAR
    #define POP_BOTTOM_MARGIN   (30)
    #define POP_RIGHT_MARGIN    (50)
    #define STATUS_BAR_HEIGHT   (0)
#else
    #define POP_BOTTOM_MARGIN   (50)
    #define POP_RIGHT_MARGIN    (-30)
    #define STATUS_BAR_HEIGHT   (40)
#endif

void ColorWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), mBackgroundColor);

    QWidget::paintEvent(event);
}

ImageLabel::ImageLabel(const QString& image, QWidget *parent) : QLabel(parent) {
    mPixmap.load(image);
}

void ImageLabel::setImage(const QString& image) {
    mPixmap.load(image);
}

void ImageLabel::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), mPixmap);

    QWidget::paintEvent(event);
}

PopScreen::PopScreen(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint| Qt::ToolTip |Qt::WindowStaysOnTopHint)
    , transparentPercent(1.0), mPopState(false), showTimer(NULL), stayTimer(NULL), closeTimer(NULL), mSelected(NOTIFY_IGNORE_SELECT)
{
    resize(POP_WIDTH, POP_HEIGHT);

    initHead(this);
    initBody(this);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(beClose()));
//    connect(closeButton, SIGNAL(clicked()), this, SLOT(onStay()));
//    connect(ignoreButton, SIGNAL(clicked()), this, SLOT(onLeftClicked()));
//    connect(confirmButton, SIGNAL(clicked()), this, SLOT(onRightClicked()));

    showTimer = new QTimer(this);
    showTimer->setInterval(5);
    stayTimer = new QTimer(this);
    stayTimer->setInterval(5000);
    closeTimer = new QTimer(this);
    closeTimer->setInterval(5);

    connect(showTimer, SIGNAL(timeout()), this, SLOT(onMove()));
    connect(stayTimer, SIGNAL(timeout()), this, SLOT(onStay()));
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(onClose()));

    this->showNormal();
    this->raise();
    this->activateWindow();

    // hide the pop windows
    QRect rect = QApplication::desktop()->availableGeometry();
    point.setX(rect.width() - width() + POP_RIGHT_MARGIN);
    point.setY(rect.height() + rect.y() + POP_BOTTOM_MARGIN);
    desktopHeight = rect.height() + POP_BOTTOM_MARGIN;
    move(point.x(), point.y());
    setWindowOpacity(0);
    setVisible(false);

    bClose = false;
    iPreNum = -1;
}

PopScreen::~PopScreen() {
    if (showTimer) {
        delete showTimer;
        showTimer = NULL;
    }

    if (stayTimer) {
        delete stayTimer;
        stayTimer = NULL;
    }
    if (closeTimer) {
        delete closeTimer;
        closeTimer = NULL;
    }
}

QWidget* PopScreen::initTitle(QWidget *parent) {
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(parent);
    layout->setSpacing(0);
    layout->setContentsMargins(10,5,5,5);

    titleLabel = initLabel(12, Qt::white);

    layout->addWidget(titleLabel, 0, Qt::AlignLeft);

    layout->addStretch();

    closeButton = new PushButton;
    closeButton->loadPixmap(":/images/sysclose_btn");
    closeButton->setFixedSize(QSize(25, 25));
    layout->addWidget(closeButton, 0, Qt::AlignRight);

    widget->setLayout(layout);
    return widget;
}

QWidget* PopScreen::initHead(QWidget *parent) {
    ColorWidget *head = new ColorWidget(parent);
    head->setFixedSize(QSize(width(), height()*2/5));
    head->SetBackgroundColor(QColor(131, 43, 166));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);

    QWidget *title = initTitle();
    layout->addWidget(title);

    QWidget *prompt = initPrompt();
    layout->addWidget(prompt);

    layout->setStretchFactor(title, 1);
    layout->setStretchFactor(prompt, 10);

    head->setLayout(layout);
}

QWidget* PopScreen::initPrompt(QWidget *parent) {
    QWidget *widget = new QWidget(parent);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);

    iconLabel = new ImageLabel(":/images/usbkey");
    iconLabel->setFixedSize(60, 60);
    layout->addWidget(iconLabel, 0, Qt::AlignRight);

    layout->addSpacing(20);

    tipLabel = initLabel(24, Qt::white);

    layout->addWidget(tipLabel);

    layout->setStretchFactor(iconLabel, 2);
    layout->setStretchFactor(tipLabel, 5);

    widget->setLayout(layout);
    return widget;
}

QLabel* PopScreen::initLabel(int fontSize, const QColor& fontColor) {
    QLabel *label = new QLabel;
    QFont ft;
    ft.setPointSize(fontSize);
    label->setFont(ft);

    QPalette pa;
    pa.setColor(QPalette::WindowText, fontColor);
    label->setPalette(pa);
    return label;
}

QWidget* PopScreen::initContent(QWidget *parent) {
    QWidget *widget = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout;

    mContentLabel[0] = initLabel(16, QColor(62,62,62));
    mContentLabel[1] = initLabel(12, QColor(135,135,135));
    mContentLabel[2] = initLabel(12, QColor(135,135,135));

    layout->addWidget(mContentLabel[0]);
    layout->addWidget(mContentLabel[1]);
    layout->addWidget(mContentLabel[2]);

    widget->setLayout(layout);

    return widget;
}

QWidget* PopScreen::initFoot(QWidget *parent) {
    QWidget *container = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout;

    mDefaultSet = new QCheckBox;

    QPalette pa;
    pa.setColor(QPalette::WindowText, QColor(133, 133, 133));
    mDefaultSet->setPalette(pa);

    mDefaultSet->setText("记住我的选择，以后不再提示");

    layout->addWidget(mDefaultSet);

    ignoreButton = new PushButton;
    ignoreButton->loadPixmap(":/images/tbutton");
    ignoreButton->setText("暂不查杀", QColor(47,179,225), QColor(80,200,242), QColor(18, 162, 212), QColor(47,179,225));
    layout->addWidget(ignoreButton);

    confirmButton = new PushButton;
    confirmButton->loadPixmap(":/images/button");
    confirmButton->setText("立即查杀", QColor(110,58,129), QColor(140,75,164), QColor(68,36,80), QColor(110,58,129));

    layout->addWidget(confirmButton);

    container->setLayout(layout);

    return container;
}

QWidget* PopScreen::initBody(QWidget *parent) {
    ColorWidget *widget = new ColorWidget(parent);
    widget->setFixedSize(QSize(width(), height() - height()*2/5));
    widget->SetBackgroundColor(QColor(254,254,254));
    widget->move(0, height()*2/5);

    QVBoxLayout *layout = new QVBoxLayout;

    QWidget *content = initContent();
    layout->addWidget(content);

    QWidget *foot = initFoot();
    layout->addWidget(foot);

    layout->setStretchFactor(content, 4);
    layout->setStretchFactor(foot, 1);

    widget->setLayout(layout);
    return widget;
}

void PopScreen::UpdateScreen(int type, const char* title, int icon, const char *message, const char *left, const char *right) {
    mDefaultSet->setVisible(true);
    switch (icon) {
    case CommunicateBundle::NOTIFY_AUTH:
        iconLabel->setImage(":/images/authration");
        mDefaultSet->setVisible(false);
        break;
    case CommunicateBundle::NOTIFY_USB_PLUGIN:
        iconLabel->setImage(":/images/usbkey");
        break;
    case CommunicateBundle::NOTIFY_VIRUS:
        iconLabel->setImage(":/images/syspath");
        break;
    case CommunicateBundle::NOTIFY_FIREWALL:
        iconLabel->setImage(":/images/firewall");
    default:
        iconLabel->setImage(":/images/syspath");
        mDefaultSet->setVisible(false);
        break;
    }

    mLeftButtonTitle = left;
    mRightButtonTitle = right;

    titleLabel->setText(title);
    std::vector<std::string> contents;
    Utils::String::Split(message, ";", &contents);

    tipLabel->setText("");
    mContentLabel[0]->setText("");
    mContentLabel[1]->setText("");
    mContentLabel[2]->setText("");

    std::vector<std::string>::iterator iter;
    int i = 0;
    for (iter=contents.begin();iter!=contents.end();++iter,++i) {
        if(i==0) {
            tipLabel->setText(iter->c_str());
        }
        else {
            mContentLabel[i-1]->setText(iter->c_str());
        }
    }

    if ((mLeftButtonTitle.isNull()||mLeftButtonTitle.isEmpty())
            && (mRightButtonTitle.isNull()||mRightButtonTitle.isEmpty())) {
        ignoreButton->setVisible(false);
        confirmButton->setVisible(false);
    }
    else if ((!mLeftButtonTitle.isNull()||!mLeftButtonTitle.isEmpty())
             && (!mRightButtonTitle.isNull()||!mRightButtonTitle.isEmpty())) {
        ignoreButton->setVisible(true);
        confirmButton->setVisible(true);

        ignoreButton->setText(left, QColor(110,58,129), QColor(140,75,164), QColor(68,36,80), QColor(110,58,129));
        confirmButton->setText(right, QColor(110,58,129), QColor(140,75,164), QColor(68,36,80), QColor(110,58,129));
    }
    else {
        ignoreButton->setVisible(true);
        confirmButton->setVisible(false);

        confirmButton->setText(right, QColor(110,58,129), QColor(140,75,164), QColor(68,36,80), QColor(110,58,129));
    }

    update(this->rect());
}

void PopScreen::showMessage(int flag, const char* title, int type, const char *message, const char *left, const char *right) {
    if(this->bClose){
        beClose(message);
        return;
    }

    setWindowOpacity(1);
    setVisible(true);

    setFocusPolicy(Qt::StrongFocus);
    UpdateScreen(flag, title, type, message, left, right);
    mPopFlag = flag;
    mPopType = type;

    LOG_RJJH("pop widget start show...");
    if (!mPopState) {
        if (flag == CommunicateBundle::CONFIRM) {
            // confirm
            stayTimer->setInterval(10000);
        }
        else if (flag == CommunicateBundle::TOAST) {
            // toast
            LOG_RJJH("toast, after 3 sec dispare");
            stayTimer->setInterval(3000);
        }
        else if (flag == CommunicateBundle::QUERY) {
            //query
            stayTimer->setInterval(10000);
        }
        else if (flag == CommunicateBundle::INFO) {
            // scanning, do nothing
            emit end(mSelected);
        }

        LOG_RJJH("show the message %d", (flag==CommunicateBundle::INFO));
        showMessageType(flag == CommunicateBundle::INFO);
    }
    else {
        this->setFocus();
        if (flag == CommunicateBundle::INFO) {
            emit end(mSelected);

            if (type == CommunicateBundle::NOTIFY_SCAN_FINISHED) {
                LOG_RJJH("ending .... the NOTIFY_SCAN_FINISHED");
            }
            else if (type == CommunicateBundle::NOTIFY_VIRUS) {
                LOG_RJJH("ending .... the NOTIFY_VIRUS");
            }
            else if (type == CommunicateBundle::NOTIFY_UPDATE_FINISHED) {
                LOG_RJJH("ending .... the NOTIFY_UPDATE_FINISHED");
            }
            else {
                LOG_RJJH("close the pop");
            }
        }
        if (flag == CommunicateBundle::CONFIRM) {
            // confirm
            stayTimer->setInterval(3000);
        }
        else if (flag == CommunicateBundle::TOAST) {
            // toast
            LOG_RJJH("toast, after 5 sec dispare");
            stayTimer->start();
        }
        else if (flag == CommunicateBundle::QUERY) {
            //query
            stayTimer->start();
        }
    }

    raise();
    activateWindow();
}

void PopScreen::showMessageType(bool immediatly) {
    transparentPercent = 1;
    setWindowOpacity(1);
    setVisible(true);
    mPopState = true;
    QRect rect = QApplication::desktop()->availableGeometry();

    connect(closeButton, SIGNAL(clicked()), this, SLOT(beClose()));
    connect(ignoreButton, SIGNAL(clicked()), this, SLOT(onLeftClicked()));
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(onRightClicked()));

    LOG_RJJH("screen is %d -%d -%d- %d", rect.x(), rect.y(), rect.width(), rect.height());
    if (immediatly) {
        point.setX(rect.width() - width() + POP_RIGHT_MARGIN);
        point.setY(rect.height() + rect.y() + POP_BOTTOM_MARGIN);

        desktopHeight = rect.height() - height() - POP_BOTTOM_MARGIN;

        LOG_RJJH("show the pop at %d -%d", point.x(), desktopHeight);
        move(point.x(), desktopHeight);
    }
    else {
        point.setX(rect.width() - width() + POP_RIGHT_MARGIN);
        point.setY(rect.height() + rect.y() + POP_BOTTOM_MARGIN);

        desktopHeight = rect.height() + rect.y() + POP_BOTTOM_MARGIN;
        move(point.x(), point.y());

        LOG_RJJH("show the pop at %d -%d", point.x(), point.y());
        LOG_RJJH("start the animate...");
        showTimer->start();
    }
}

void PopScreen::onMove() {
    desktopHeight--;

    move(point.x(), desktopHeight);
    if (desktopHeight <= point.y() - (POP_HEIGHT+STATUS_BAR_HEIGHT)) {
        showTimer->stop();
        LOG_RJJH("start close the pop windows");
        if (mPopFlag != CommunicateBundle::INFO) {
            stayTimer->start();
        }
    }
}

void PopScreen::onStay() {
    LOG_RJJH("receive stay sigal, will dispare...");
    stayTimer->stop();
    closeTimer->start();
}

void PopScreen::onClose() {
//    transparentPercent -= 0.1;
    disconnect(closeButton, SIGNAL(clicked()), this, NULL);
    disconnect(ignoreButton, SIGNAL(clicked()), this, NULL);
    disconnect(confirmButton, SIGNAL(clicked()), this, NULL);

//    if (transparentPercent <= 0.0) {
//        closeTimer->stop();
//        onExit();
//    }
//    else {
//        setWindowOpacity(transparentPercent);
//    }

    desktopHeight++;
    move(point.x(), desktopHeight);

    if (desktopHeight >= point.y()) {
        LOG_RJJH("close the timer, and so on...");
        setWindowOpacity(0);
        setVisible(false);

        closeTimer->stop();
        onExit();
    }
}

void PopScreen::onExit() {
    mPopState = false;

    emit end(mSelected);
}

void PopScreen::onLeftClicked() {
    mSelected = mDefaultSet->checkState()<<16|NOTIFY_IGNORE_SELECT;

    if (CommunicateBundle::INFO == mPopFlag
            && CommunicateBundle::NOTIFY_VIRUS == mPopType) {
        emit end(mSelected);
    }
    else {
        onStay();
    }
}

void PopScreen::onRightClicked() {
    mSelected = mDefaultSet->checkState()<<16|NOTIFY_CONFIRM_SELECT;
    if (CommunicateBundle::INFO == mPopFlag
            && CommunicateBundle::NOTIFY_VIRUS == mPopType) {
        emit end(mSelected);
    }
    else {
        onStay();
    }
}


int PopScreen::getMsgNum(const std::string msg)
{
    if(msg.empty())
        return -1;

    std::string::size_type nPos1 = msg.find(complete_str);
    if(nPos1 != std::string::npos){
        bClose = false;
        this->iPreNum = -1;
    }

    std::string::size_type nPos2 = std::string::npos;
    nPos1 = msg.find(pecent_beg_str);
    nPos2 = msg.find(pecent_end_str);

    std::string tmp= "-1";
    if(nPos1 !=-1 && nPos2 != -1){
        tmp = msg.substr(nPos1 + strlen(pecent_beg_str), nPos2 - nPos1 - strlen(pecent_beg_str));
        printf("%d, %d tmp:%s\n",nPos1, nPos2, tmp.c_str());
    }

    return atoi(tmp.c_str());
}

void PopScreen::beClose(const char * msg)
{
    if(!msg){
        bClose = true;
        onStay();
    }else{
        int ret = getMsgNum(msg);
        if(ret < 0 ){
            bClose = false;
        }
        if(ret < iPreNum){
            bClose = false;
        }
        iPreNum = ret;

        LOG_RJJH("getMsgNum %d, iPreNum:%d, bClose=%s", ret, iPreNum, bClose?"true":"false");

        emit end(mSelected);
    }
}

