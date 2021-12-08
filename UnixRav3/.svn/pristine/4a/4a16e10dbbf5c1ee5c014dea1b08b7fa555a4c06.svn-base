#include <QPixmap>
#include <QPalette>
#include <QHBoxLayout>

#include <QDebug>

#include "topwidget.h"

TopWidget::TopWidget(QWidget *parent) : QWidget(parent)
{
    m_pIcon = new QIcon(":/resource/config");
    m_pCloseBtn = new PushButton;
    //m_pMinBtn = new PushButton;

    m_pCloseBtn->loadPixmap(":/resource/close_btn");
    //m_pMinBtn->loadPixmap(":/resource/sysmin_btn");

    setFixedSize(APP_SCREEN_WIDTH, m_pCloseBtn->height());

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(57,23,92));
    setPalette(palette);
    setAutoFillBackground(true);
    //qDebug("TopWidget::TopWidget x,y,w,h:%d  %d  %d %d", x(),y(),width(),height());
    m_sTitle = u8"设置中心";
    InitWidget();
}

void TopWidget::InitWidget()
{
    //m_pCloseBtn->setGeometry(width()-m_pCloseBtn->width(),0,m_pCloseBtn->width(),m_pCloseBtn->height());
    //m_pCloseBtn->setGeometry(30,0,m_pCloseBtn->width(),m_pCloseBtn->height());
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setMargin(0);
    hlayout->setContentsMargins(0,0,0,0);
    hlayout->setSpacing(0);
    hlayout->addStretch();

    //hlayout->addWidget(m_pMinBtn);
    hlayout->addWidget(m_pCloseBtn);
    setLayout(hlayout);
}

void TopWidget::SetTitle(QString &sTitle)
{
    m_sTitle = sTitle;
    update();
}

void TopWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::white);
    QFont font;
    font.setPointSize(12);
    font.setWeight(QFont::Normal);
    painter.setFont(font);
    QSize size(24, 24);
    QPixmap pm = m_pIcon->pixmap(size, QIcon::Normal);
    painter.drawPixmap(6, 6, pm.width(), pm.height(), pm, 0, 0, pm.width(), pm.height());
    painter.drawText(36,0,width()-80,m_pCloseBtn->height(),Qt::AlignLeft|Qt::AlignVCenter, m_sTitle);
}
