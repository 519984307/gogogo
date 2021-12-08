#include <QHBoxLayout>
#include <QLabel>

#include "pushbutton.h"


PushButton::PushButton(QWidget *parent)
    :QPushButton(parent)
{
    mStatus = NORMAL;
    mMousePress = false;
}

PushButton::~PushButton()
{

}

void PushButton::loadPixmap(const QString& path)
{
    bool ret = mPixmap.load(path);
    if(ret) {
        mBtnWidth = mPixmap.width()/4;
        mBtnHeight = mPixmap.height();
        setFixedSize(mBtnWidth, mBtnHeight);
    }
}

void PushButton::enterEvent(QEvent *)
{
    mStatus = ENTER;
    update();
}

void PushButton::mousePressEvent(QMouseEvent *event)
{
    //若点击鼠标左键
    if(event->button() == Qt::LeftButton)
    {
        mMousePress = true;
        mStatus = PRESS;
        update();
    }
}

void PushButton::mouseReleaseEvent(QMouseEvent *)
{
    //若点击鼠标左键
    if(mMousePress)
    {
        mMousePress = false;
        mStatus = ENTER;
        update();
        emit clicked();
    }
}

void PushButton::leaveEvent(QEvent *)
{
    mStatus = NORMAL;
    update();
}

void PushButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), mPixmap.copy(mBtnWidth * mStatus, 0, mBtnWidth, mBtnHeight));

    painter.setPen(mTextColor[mStatus]);
    painter.drawText(this->rect(), Qt::AlignCenter, mText);
}

/**
 * @brief PushButton::PushButton
 * @param parent
 */
IconTextButton::IconTextButton(QString iconPath, QString txt, QWidget *parent) : PushButton(parent)
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);

    QLabel *icon = new QLabel();
    QPixmap iconMap;
    iconMap.load(iconPath);
    icon->setPixmap(iconMap/*.scaled(20, 20)*/);

    QLabel *text = new QLabel;
    text->setText(txt);

    layout->addStretch();
    layout->addWidget(icon, 0, Qt::AlignVCenter);
    layout->addSpacing(5);
    layout->addWidget(text, 0, Qt::AlignVCenter);
    layout->addStretch();

    this->setLayout(layout);
    this->setFixedWidth(100);
}

IconTextButton::~IconTextButton()
{

}

void IconTextButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(mMousePress)
    {
        painter.setBrush(Qt::gray);
    }
    else
    {
        painter.setBrush(Qt::transparent);
    }

    painter.setPen(QPen(QColor(148, 148, 148)));
    QRect rc = this->rect();
    rc.adjust(1,1,-1,-1);
    painter.drawRect(rc);
}

void MoreStateTextButton::paintEvent(QPaintEvent *event)
{
    PushButton::paintEvent(event);

    QPainter painter(this);
    switch(mStatus) {
    case ENTER:
        painter.setPen(QPen(QColor(255, 0, 0)));
        break;
    case PRESS:
        painter.setPen(QPen(QColor(0, 255, 0)));
        break;
    case NORMAL:
        painter.setPen(QPen(QColor(0, 0, 255)));
        break;
    default:
        break;
    }
    QRect rc = this->rect();
    painter.drawText(rc, mTitle);
}

