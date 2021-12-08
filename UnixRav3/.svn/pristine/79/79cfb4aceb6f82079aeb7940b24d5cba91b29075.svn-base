#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>

#include "filedirbutton.h"

FileDirButton::FileDirButton(QWidget *parent)
    :QPushButton(parent)
{
    mStatus = NORMAL;
    m_bMousePress = false;
}

void FileDirButton::loadPixmap(QString sImgName)
{
    bool ret = mPixmap.load(sImgName);
    if(ret) {
        m_nBtnWidth = mPixmap.width()/4;
        m_nBtnHeight = mPixmap.height();
        setFixedSize(m_nBtnWidth, m_nBtnHeight);
    }
}

void FileDirButton::enterEvent(QEvent *)
{
    mStatus = ENTER;
    update();
}

void FileDirButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bMousePress = true;
        mStatus = PRESS;
        update();
    }
}

void FileDirButton::mouseReleaseEvent(QMouseEvent *)
{
    if(m_bMousePress)
    {
        m_bMousePress = false;
        mStatus = ENTER;
        update();
        emit clicked();
    }
}

void FileDirButton::leaveEvent(QEvent *)
{
    mStatus = NORMAL;
    update();
}

void FileDirButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), mPixmap.copy(m_nBtnWidth * mStatus, 0, m_nBtnWidth, m_nBtnHeight));

}

