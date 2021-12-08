#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>

#include "common.h"
#include "defendcenteritems.h"


DefendCenterItemRect::DefendCenterItemRect(int nType, QWidget *parent):QWidget(parent),m_nType(nType), m_bEntered(false)
{
    setFixedSize(DEFENDCENTERTOPITEM_TOP_RECT_WIDTH, DEFENDCENTERTOPITEM_TOP_RECT_HEIGHT);

    m_ppTopIcon = new QPixmap(":/resource/defendcenter_item_icon");
    m_ppBotTxt = new QPixmap(":/resource/defendcenter_item_text");
}

void DefendCenterItemRect::UpdateInfo(bool bEnabled)
{
    m_bEnabled = bEnabled;
    update();
}

void DefendCenterItemRect::enterEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = true;
    update();
}

void DefendCenterItemRect::leaveEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = false;
    update();
}

void DefendCenterItemRect::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);
    QPainter painter(this);

    QRect rectIconPos((DEFENDCENTERTOPITEM_TOP_RECT_WIDTH - m_ppTopIcon->width()/10)/2, 15,
                      m_ppTopIcon->width()/10, m_ppTopIcon->height()/2);
    QRect rectTxtPos((DEFENDCENTERTOPITEM_TOP_RECT_WIDTH - m_ppBotTxt->width())/2, (DEFENDCENTERTOPITEM_TOP_RECT_HEIGHT - m_ppTopIcon->height()/2)/2 + m_ppTopIcon->height()/2,
                     m_ppBotTxt->width(), m_ppBotTxt->height()/10);

    if(m_bEnabled){
        painter.drawPixmap(rectIconPos, m_ppTopIcon->copy(m_ppTopIcon->width()/10*m_nType, 0, m_ppTopIcon->width()/10, m_ppTopIcon->height()/2));
    }else{
        painter.drawPixmap(rectIconPos, m_ppTopIcon->copy(m_ppTopIcon->width()/10*m_nType, m_ppTopIcon->height()/2, m_ppTopIcon->width()/10, m_ppTopIcon->height()/2));
    }
    painter.drawPixmap(rectTxtPos, m_ppBotTxt->copy(0, m_ppBotTxt->height()/10*m_nType, m_ppBotTxt->width(), m_ppBotTxt->height()/10));

    if(m_bEntered && m_nType < RAV_QT_DEFENDCENTER_ITEM_TYPE_FEIKE){
        painter.setPen(Qt::black);
        painter.drawRect(0, 0, DEFENDCENTERTOPITEM_TOP_RECT_WIDTH-1, DEFENDCENTERTOPITEM_TOP_RECT_HEIGHT-1);
    }else{
        painter.setPen(Qt::white);
        painter.drawRect(0, 0, DEFENDCENTERTOPITEM_TOP_RECT_WIDTH-1, DEFENDCENTERTOPITEM_TOP_RECT_HEIGHT-1);
    }
}

DefendCenterItemButton::DefendCenterItemButton(QWidget *parent):QPushButton(parent),m_bEntered(false),m_bEnabled(false),m_bMousePressed(false)
{
    m_ppIcon = new QPixmap(":/resource/switch_new_btn");

    setFixedSize(DEFENDCENTERTOPITEM_TOP_RECT_WIDTH, DEFENDCENTERTOPITEM_HEIGHT - DEFENDCENTERTOPITEM_TOP_RECT_HEIGHT);

    setCursor(Qt::PointingHandCursor);
}

void DefendCenterItemButton::UpdateInfo(bool bEnabled)
{
    m_bEnabled = bEnabled;
    update();
}


void DefendCenterItemButton::enterEvent(QEvent *)
{
    m_bEntered = true;
    update();
}

void DefendCenterItemButton::leaveEvent(QEvent *)
{
    m_bEntered = false;
    update();
}

void DefendCenterItemButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_bMousePressed = true;
        update();
    }
}

void DefendCenterItemButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(m_bMousePressed){
        m_bMousePressed = false;
        update();
        emit clicked();
    }
}

void DefendCenterItemButton::paintEvent(QPaintEvent *)
{
    QRect rectPos((width()-m_ppIcon->width()/3)/2, (height()-m_ppIcon->height()/2)/2, m_ppIcon->width()/3, m_ppIcon->height()/2);

    QPainter painter(this);
    if(m_bEnabled){
        if(m_bMousePressed){
            painter.drawPixmap(rectPos, m_ppIcon->copy(m_ppIcon->width()/3*2, 0, m_ppIcon->width()/3, m_ppIcon->height()/2));
        }else{
            if(m_bEntered){
                painter.drawPixmap(rectPos, m_ppIcon->copy(m_ppIcon->width()/3, 0, m_ppIcon->width()/3, m_ppIcon->height()/2));
            }else{
                painter.drawPixmap(rectPos, m_ppIcon->copy(0, 0, m_ppIcon->width()/3, m_ppIcon->height()/2));
            }
        }
    }else{
        if(m_bMousePressed){
            painter.drawPixmap(rectPos, m_ppIcon->copy(m_ppIcon->width()/3*2, m_ppIcon->height()/2, m_ppIcon->width()/3, m_ppIcon->height()/2));
        }else{
            if(m_bEntered){
                painter.drawPixmap(rectPos, m_ppIcon->copy(m_ppIcon->width()/3, m_ppIcon->height()/2, m_ppIcon->width()/3, m_ppIcon->height()/2));
            }else{
                painter.drawPixmap(rectPos, m_ppIcon->copy(0, m_ppIcon->height()/2, m_ppIcon->width()/3, m_ppIcon->height()/2));
            }
        }
    }

}


DefendCenterItem::DefendCenterItem(int nType, QWidget *parent) : QWidget(parent),m_nType(nType),m_bEnabled(false)
{
    setFixedSize(DEFENDCENTERTOPITEM_TOP_RECT_WIDTH, DEFENDCENTERTOPITEM_HEIGHT);

    m_pTopRect = new DefendCenterItemRect(nType, this);
    m_pBtn = new DefendCenterItemButton(this);

    m_pTopRect->m_bEnabled = m_bEnabled;
    m_pBtn->m_bEnabled = m_bEnabled;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(m_pTopRect);
    layout->addWidget(m_pBtn);

    setLayout(layout);

    connect(m_pBtn, SIGNAL(clicked()), this, SLOT(OnClickedBtn()));
}

void DefendCenterItem::UpdateInfo(bool bEnabled)
{
    m_bEnabled = bEnabled;
    m_pTopRect->UpdateInfo(bEnabled);
    m_pBtn->UpdateInfo(bEnabled);
}

void DefendCenterItem::OnClickedBtn()
{
    m_bEnabled = !m_pBtn->m_bEnabled;
    m_pTopRect->UpdateInfo(m_bEnabled);
    m_pBtn->UpdateInfo(m_bEnabled);
    emit clicked();
}


