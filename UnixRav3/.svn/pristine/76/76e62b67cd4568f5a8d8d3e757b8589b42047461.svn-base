#include "showbutton.h"

#include <QPainter>

ShowButton::ShowButton(QWidget *parent) :
    QPushButton(parent)
{
    mPage = -1;
    mIndex = -1;
}

ShowButton::ShowButton(int page, int index, QWidget *parent) : QPushButton(parent)
{
    mPage = page;
    mIndex = index;

    connect(this, SIGNAL(clicked()), this, SLOT(onClicked()));
}

ShowButton::~ShowButton()
{

}

void ShowButton::setButtonState()
{

}

void ShowButton::loadPixmap(QString pic_name)
{
    bool ret = mImage.load(pic_name);
    if(ret) {
        mBtnWidth = mImage.width();
        mBtnHeight = mImage.height();
        setFixedSize(mBtnWidth, mBtnHeight);
    }
}

void ShowButton::loadPixmap(QString pic_name, QSize size)
{
    QPixmap *img = new QPixmap;
    if(img->load(pic_name)) {
        mImage = img->scaled(size);

        mBtnWidth = mImage.width();
        mBtnHeight = mImage.height();
        setFixedSize(mBtnWidth, mBtnHeight);
    }
    delete img;
}

void ShowButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), mImage);
}

void ShowButton::onClicked()
{
    emit itemclicked(mPage, mIndex);
}

