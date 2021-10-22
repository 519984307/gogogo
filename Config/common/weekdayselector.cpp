#include <QMouseEvent>
#include <QPainter>
#include <QPalette>
#include <QHBoxLayout>

#include "weekdayselector.h"


//QPalette pal(palette());

//pal.setColor(QPalette::Background, QColor(57,23,92));
//setAutoFillBackground(true);
//setPalette(pal);

//m_plTitle = new QLabel(u8"防护中心");

//QFont ft;
//ft.setPointSize(16);
//m_plTitle->setFont(ft);
//m_plTitle->adjustSize();

//QPalette pa;
//pa.setColor(QPalette::WindowText,Qt::white);
//m_plTitle->setPalette(pa);
WeekDayLabel::WeekDayLabel(int nWeekDay, QWidget *parent, Qt::WindowFlags f) :QLabel(parent, f), m_bEntered(false)
{
    //QPalette pa(palette());
    m_nWeekDay = nWeekDay;
    setText(WeekTxt[nWeekDay]);

    m_bSelected = false;
    setFixedSize(20, 20);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet("background-color:rgb(230,230,230)");
    //setAutoFillBackground(true);
}

void WeekDayLabel::SetSelected(bool bSelected)
{
    m_bSelected = bSelected;
    if(m_bSelected)
        setStyleSheet("background-color:rgb(123,89,157)");
    update();
}

void WeekDayLabel::enterEvent(QEvent *)
{
    if (isEnabled() == false) {
        return;
    }
    m_bEntered = true;
    setStyleSheet("background-color:white");
    //update();
}

void WeekDayLabel::mousePressEvent(QMouseEvent *event)
{
    if (isEnabled() == false) {
        return;
    }
    if(event->button() == Qt::LeftButton)
    {
        m_bMouseDown = true;
        setStyleSheet("background-color:rgb(123,89,157)");
    }
}

void WeekDayLabel::mouseReleaseEvent(QMouseEvent *)
{
    if(isEnabled() == false) {
        return;
    }
    if(m_bMouseDown)
    {
        m_bMouseDown = false;
        m_bSelected = !m_bSelected;
        setStyleSheet("background-color:white");
        emit clicked();
    }
}

void WeekDayLabel::leaveEvent(QEvent *)
{
    if (isEnabled() == false) {
        return;
    }
    m_bEntered = false;
    if(m_bSelected){
        setStyleSheet("background-color:rgb(123,89,157)");
    }else{
        setStyleSheet("background-color:rgb(230,230,230)");
    }
}

void WeekDayLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPalette pa(palette());

    if(m_bEntered){
        if(m_bMouseDown){
            painter.setPen(Qt::white);
        }else{
            if(m_nWeekDay > 4){
                painter.setPen(Qt::red);
            }else{
                painter.setPen(Qt::black);
            }
        }
    }else{
        if(m_bSelected){
            painter.setPen(Qt::white);
        }else{
            if(m_bMouseDown){
                painter.setPen(Qt::black);
            }else{
                if(m_nWeekDay > 4){
                    painter.setPen(Qt::red);
                }else{
                    painter.setPen(Qt::black);
                }
            }
        }
    }
    painter.drawText(QRect(0,0,20,20), Qt::AlignCenter, WeekTxt[m_nWeekDay]);
//    painter.setPen(Qt::black);
//    painter.drawRect(0, 0, 19, 19);
//    QFont font("Arial", 12, QFont::Normal, false);
//    painter.setFont(font);
//    painter.setPen(Qt::white);
//    painter.setFont(QFont( "Timers" , 8 ,  QFont::Thin));
//    pa.setColor(QPalette::WindowText,Qt::black);

    painter.setPen(Qt::black);
    painter.drawRect(0,0,19,19);
}


WeekDaySelector::WeekDaySelector(QWidget *parent) : QWidget(parent)
{
    setFixedSize(180, 20);

    m_pMonday = new WeekDayLabel(0, this);
    m_pTuesday = new WeekDayLabel(1, this);
    m_pWednesday = new WeekDayLabel(2, this);
    m_pThursday = new WeekDayLabel(3, this);
    m_pFriday = new WeekDayLabel(4, this);
    m_pSaturday = new WeekDayLabel(5, this);
    m_pSunday = new WeekDayLabel(6, this);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);
    layout->addWidget(m_pMonday);
    layout->addWidget(m_pTuesday);
    layout->addWidget(m_pWednesday);
    layout->addWidget(m_pThursday);
    layout->addWidget(m_pFriday);
    layout->addWidget(m_pSaturday);
    layout->addWidget(m_pSunday);

    setLayout(layout);

    connect(m_pMonday, SIGNAL(clicked()), this, SLOT(OnClickedDay()));
    connect(m_pTuesday, SIGNAL(clicked()), this, SLOT(OnClickedDay()));
    connect(m_pWednesday, SIGNAL(clicked()), this, SLOT(OnClickedDay()));
    connect(m_pThursday, SIGNAL(clicked()), this, SLOT(OnClickedDay()));
    connect(m_pFriday, SIGNAL(clicked()), this, SLOT(OnClickedDay()));
    connect(m_pSaturday, SIGNAL(clicked()), this, SLOT(OnClickedDay()));
    connect(m_pSunday, SIGNAL(clicked()), this, SLOT(OnClickedDay()));
}

void WeekDaySelector::Reset()
{
    m_pMonday->SetSelected(false);
    m_pTuesday->SetSelected(false);
    m_pWednesday->SetSelected(false);
    m_pThursday->SetSelected(false);
    m_pFriday->SetSelected(false);
    m_pSaturday->SetSelected(false);
    m_pSunday->SetSelected(false);
}

void WeekDaySelector::GetSelected(std::vector<int>& days)
{
    days.clear();
    if(m_pMonday->m_bSelected)
        days.push_back(1);
    if(m_pTuesday->m_bSelected)
        days.push_back(2);
    if(m_pWednesday->m_bSelected)
        days.push_back(3);
    if(m_pThursday->m_bSelected)
        days.push_back(4);
    if(m_pFriday->m_bSelected)
        days.push_back(5);
    if(m_pSaturday->m_bSelected)
        days.push_back(6);
    if(m_pSunday->m_bSelected)
        days.push_back(0);
}

void WeekDaySelector::SetSelected(std::vector<int>& days)
{
    int nday;
    Reset();
    std::vector<int>::iterator it = days.begin();
    while(it != days.end()){
        nday = *it;
        switch(nday){
        case 1:
            m_pMonday->SetSelected(true);
            break;
        case 2:
            m_pTuesday->SetSelected(true);
            break;
        case 3:
            m_pWednesday->SetSelected(true);
            break;
        case 4:
            m_pThursday->SetSelected(true);
            break;
        case 5:
            m_pFriday->SetSelected(true);
            break;
        case 6:
            m_pSaturday->SetSelected(true);
            break;
        case 0:
            m_pSunday->SetSelected(true);
            break;
        }
        it++;
    }
}


void WeekDaySelector::OnClickedDay()
{
    qDebug("WeekDaySelector::OnClickedDay() occur");
}
