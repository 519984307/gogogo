#include <QFont>
#include <QPalette>
#include <QHBoxLayout>

#include "common.h"
#include "common/pushbutton.h"
#include "defendcenterheaderwidget.h"

DefendCenterHeaderWidget::DefendCenterHeaderWidget(QWidget *parent) : QWidget(parent)
{
    QPalette pal(palette());

    pal.setColor(QPalette::Background, QColor(57,23,92));
    setAutoFillBackground(true);
    setPalette(pal);

    m_plTitle = new QLabel(u8"防护中心");

    QFont ft;
    ft.setPixelSize(16);
    m_plTitle->setFont(ft);
    m_plTitle->adjustSize();

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    m_plTitle->setPalette(pa);


    m_pbtnClose = new PushButton;
    m_pbtnClose->loadPixmap(":/resource/close_btn");

    setFixedSize(APP_DEFEND_CENTER_DIALOG_WIDTH-2 ,m_pbtnClose->height());

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(6, 0, 0, 0);
    //layout->setMargin(0);
    layout->addWidget(m_plTitle, 0, Qt::AlignLeft|Qt::AlignVCenter);
    layout->addStretch();
    layout->addWidget(m_pbtnClose, 0, Qt::AlignRight|Qt::AlignVCenter);

    setLayout(layout);

}

void DefendCenterHeaderWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons() != Qt::LeftButton) {
        return;
    }
    m_ptPressPoint = e->pos();
    m_bIsMove = true;
}

void DefendCenterHeaderWidget::mouseMoveEvent(QMouseEvent *e)
{
    if((e->buttons() == Qt::LeftButton) && m_bIsMove)
    {
        QWidget* parent_widget = this->parentWidget();
        QPoint parent_point = parent_widget->pos();
        parent_point.setX(parent_point.x() + e->x() - m_ptPressPoint.x());
        parent_point.setY(parent_point.y() + e->y() - m_ptPressPoint.y());
        parent_widget->move(parent_point);
    }
}

void DefendCenterHeaderWidget::mouseReleaseEvent(QMouseEvent *)
{
    if(m_bIsMove)
    {
        m_bIsMove = false;
    }
}
