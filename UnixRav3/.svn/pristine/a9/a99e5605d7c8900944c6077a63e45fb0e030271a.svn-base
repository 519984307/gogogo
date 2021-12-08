#include "dirselectheaderwidget.h"

DirSelectHeaderWidget::DirSelectHeaderWidget(QWidget *parent) : QWidget(parent)
{
    //setFixedHeight(DIR_SELECT_HEADER_WIDGET_HEIGHT);
    setFixedSize(DIR_SELECT_DIALOG_WODTH, DIR_SELECT_HEADER_WIDGET_HEIGHT);
    //setStyleSheet("background-color:black;");
    QPalette pal(palette());

    pal.setColor(QPalette::Background, QColor(57,23,92));
    setAutoFillBackground(true);
    setPalette(pal);

    m_pCloseBtn = new PushButton(this);
    m_pCloseBtn->loadPixmap(":/resource/close_btn");

    m_pCloseBtn->setGeometry(DIR_SELECT_DIALOG_WODTH-36, 0, 36, 36);
}


void DirSelectHeaderWidget::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);
    
    QPainter painter(this);
    QFont font = painter.font();
    font.setPixelSize(14);
    painter.setFont(font);
    painter.setPen(QColor(Qt::white));
    painter.drawText(10,25, u8"选择查杀目录");
}

void DirSelectHeaderWidget::mousePressEvent(QMouseEvent *e)
{
    m_ptPressPoint = e->pos();
    m_bIsMove = true;
}

void DirSelectHeaderWidget::mouseMoveEvent(QMouseEvent *e)
{
    if((e->buttons() == Qt::LeftButton) && m_bIsMove)
    {
        QWidget* parent_widget = this->parentWidget();
        if (parent_widget == NULL) {
            return;
        }
        QPoint parent_point = parent_widget->pos();
        parent_point.setX(parent_point.x() + e->x() - m_ptPressPoint.x());
        parent_point.setY(parent_point.y() + e->y() - m_ptPressPoint.y());
        parent_widget->move(parent_point);
    }
}

void DirSelectHeaderWidget::mouseReleaseEvent(QMouseEvent *)
{
    if(m_bIsMove)
    {
        m_bIsMove = false;
    }
}
