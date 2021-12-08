#include "clickedlabel.h"


ClickedLabel::ClickedLabel(QWidget *parent, Qt::WindowFlags f)
    :QLabel(parent, f)
{

}

ClickedLabel::ClickedLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    :QLabel(text, parent, f)
{

}

void ClickedLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        emit clicked();
    }
}
