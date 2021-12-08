#include <QLabel>
#include <QScrollBar>
#include <QEvent>
#include <QMouseEvent>


#include "midscrollwidget.h"
#include "rightmidwidget.h"



RightMidWidget::RightMidWidget(QWidget *parent, int nHeight) : QScrollArea(parent),m_pMidWidget(NULL)
{
    setFixedSize(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH, nHeight);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(248,248,248));
    setPalette(palette);
    setAutoFillBackground(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_pMidWidget = new MidScrollWidget;
    setWidget(m_pMidWidget);
}

void RightMidWidget::SwitchTopWidget()
{
//    m_pEndWidget->setVisible(false);
//    m_pTopWidget->setVisible(true);
//    setWidget(m_pTopWidget);

}

void RightMidWidget::SwitchEndWidget()
{
//    m_pTopWidget->setVisible(false);
//    m_pEndWidget->setVisible(true);
//    setWidget(m_pEndWidget);
}

bool RightMidWidget::eventFilter(QObject *obj, QEvent *evt)
{
    if(evt->type() == QEvent::MouseMove)
    {
        QMouseEvent *me = (QMouseEvent*) evt;
        if(me->buttons() & Qt::LeftButton)
        {
            if(!m_bMoveStart)
            {
                m_bMoveStart = true;
            }
            else
            {
                int val = verticalScrollBar()->value();
                if(m_bScrollStart){
                    emit valueChanged(val);
                    //qDebug("RightTopWidget::eventFilter val=%d\n", val);
                }
            }
        }
    }
    else if(evt->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *me = (QMouseEvent*) evt;
        if(me->button() == Qt::LeftButton){
            QPoint pt = QCursor::pos();
            QPoint ptchild = verticalScrollBar()->mapToGlobal(verticalScrollBar()->pos());
            QRect rect = QRect(ptchild.x(),ptchild.y(), verticalScrollBar()->width(),verticalScrollBar()->height());
            //qDebug("RightTopWidget::eventFilter left button pressed");
            if(rect.contains(pt)){
                m_bScrollStart = true;
            }
        }
    }
    else if(evt->type() == QEvent::MouseButtonRelease)
    {
        m_bMoveStart = false;
        m_bScrollStart = false;
    }
    return QScrollArea::eventFilter(obj,evt);
}
