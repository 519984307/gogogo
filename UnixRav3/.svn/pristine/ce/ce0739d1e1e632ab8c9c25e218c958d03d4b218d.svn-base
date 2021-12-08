#include <QHBoxLayout>

#include "pushbutton.h"
#include "searchedit.h"



SearchEdit::SearchEdit(QWidget *parent):QLineEdit(parent)
{
    m_pbtnSearch = new PushButton(this);
    m_pbtnSearch->loadPixmap(":/resource/search");
    //m_pbtnSearch->setGeometry(width()-25, 0, 22, 22);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setMargin(1);
    hlayout->addWidget(m_pbtnSearch, 0, Qt::AlignRight | Qt::AlignVCenter);
    setLayout(hlayout);
}

//void SearchEdit::enterEvent(QEvent *pEvent)
//{

//}

//void SearchEdit::leaveEvent(QEvent *pEvent)
//{

//}


//void SearchEdit::mouseMoveEvent(QMouseEvent *pEvent)
//{

//}

//void SearchEdit::mousePressEvent(QMouseEvent *pEvent)
//{

//}

//void SearchEdit::mouseReleaseEvent(QMouseEvent *pEvent)
//{

//}


//void SearchEdit::paintEvent(QPaintEvent *pEvent)
//{

//}
