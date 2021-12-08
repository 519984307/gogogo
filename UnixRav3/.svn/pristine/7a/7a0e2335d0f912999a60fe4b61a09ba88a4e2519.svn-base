#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "defenditemwidget.h"
#include "common.h"


DefendTopLabel::DefendTopLabel(int nDefendType, QWidget *parent)
    :QLabel(parent),m_nDefendType(nDefendType),m_bEnabled(false),m_bEntered(false),m_nScreenCount(0)
{
    m_ppTypeIcon = new QPixmap(":/resource/defend_item_icon");
    m_nIconWidth = m_ppTypeIcon->width()/2;
    m_nIconHeight = m_ppTypeIcon->height()/10;

    setFixedSize(DEFEND_ITEM_WIDGET_WIDTH-2, DEFEND_ITEM_WIDGET_HEIGHT-32);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet("background-color:white");
    setFrameStyle(QFrame::NoFrame);
}

void DefendTopLabel::SetEnabled(bool bEnable)
{
    m_bEnabled = bEnable;
    update();
}

void DefendTopLabel::SetCount(int nCount)
{
    m_nScreenCount = nCount;
    update();
}

void DefendTopLabel::enterEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = true;
    update();
}

void DefendTopLabel::leaveEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered =false;
    update();
}

void DefendTopLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        m_bEnabled = !m_bEnabled;
        emit clicked();
        update();
    }
}

void DefendTopLabel::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);
    QRect textPos(6, 0, width()-12, 25);
    QRect countPos(6, 95, width()-12, 16);
    QRect imgPos((width()-m_nIconWidth)/2, 28, m_nIconWidth, m_nIconHeight);
    QColor color(76,32,107);
    QFont font;
    font.setPixelSize(10);
    font.setWeight(QFont::Normal);
    QPainter painter(this);
    painter.setFont(font);
    QString sCount;
    sCount.sprintf("%d 次", m_nScreenCount);

    if(m_bEntered){
        painter.setPen(color);
    }else{
        painter.setPen(Qt::black);
    }

    if(m_nDefendType == RAV_QT_DEFENT_TYPE_MORE){
        textPos.setX(45);
        textPos.setY(105);
        painter.drawText(textPos, Qt::AlignHCenter|Qt::AlignVCenter, sDefendTypeDesc[m_nDefendType]);
    }else{
        painter.drawText(textPos, Qt::AlignLeft|Qt::AlignVCenter, sDefendTypeDesc[m_nDefendType]);
        painter.drawText(countPos, Qt::AlignHCenter|Qt::AlignVCenter, sCount);
    }

    int imgx = 0;
    int imgy = m_nIconHeight * m_nDefendType;

    if(!m_bEnabled){
        imgx = m_nIconWidth+1;
    }
    painter.drawPixmap(imgPos, m_ppTypeIcon->copy(imgx, imgy, m_nIconWidth, m_nIconHeight));
}

DefendBotLabel::DefendBotLabel(QWidget *parent):QLabel(parent),m_bEnabled(false),m_bDropped(false)
{
//已开启 已关闭 重启前关闭 重启前开启 永久关闭 永久开启
    setFixedSize(DEFEND_ITEM_WIDGET_WIDTH-2, 30);
    setStyleSheet("background-color:white");
    setFrameStyle(QFrame::NoFrame);

    m_ppDropItemArrow = new QPixmap(":/resource/defend_item_arrow");
}

void DefendBotLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        m_bDropped = !m_bDropped;
        emit clicked();
        update();
        //qDebug("DefendBotLabel::mousePressEvent occur");
    }
}

void DefendBotLabel::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);
    QPainter painter(this);
    painter.setPen(Qt::black);
    QRect textPos(1, 1, width()-2, height()-1);
    painter.drawText(textPos, Qt::AlignVCenter|Qt::AlignHCenter, sDefendStatusText[m_bEnabled?0:1]);

    QRect imgPos(width()-m_ppDropItemArrow->width()/3, 0, m_ppDropItemArrow->width()/3, m_ppDropItemArrow->height());
    //qDebug("DefendBotLabel::paintEvent  x,y,w,h = %d %d %d %d", imgPos.x(), imgPos.y(), imgPos.width(), imgPos.height());
    painter.drawPixmap(imgPos, m_ppDropItemArrow->copy((m_ppDropItemArrow->width()/3)*(m_bDropped?2:0), 0, m_ppDropItemArrow->width()/3, m_ppDropItemArrow->height()));

}

DefendItemWidget::DefendItemWidget(int nDefendType, QWidget *parent)
    : QWidget(parent),m_nDefendType(nDefendType),m_bEnabled(false),m_bEntered(false)
{
    setFixedSize(DEFEND_ITEM_WIDGET_WIDTH, DEFEND_ITEM_WIDGET_HEIGHT);
    m_pTopLabel = new DefendTopLabel(nDefendType, this);
    m_pBotLabel = new DefendBotLabel(this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(1);
    layout->setSpacing(0);
    layout->addWidget(m_pTopLabel, 0, Qt::AlignHCenter);
    layout->addWidget(m_pBotLabel, 0, Qt::AlignHCenter);
    setLayout(layout);

    connect(m_pTopLabel, SIGNAL(clicked()), this, SLOT(slotUpdateStatus()));
    connect(m_pBotLabel, SIGNAL(clicked()), this, SLOT(slotDropMenu()));
}

void DefendItemWidget::ResetDropMenu()
{
    m_pBotLabel->m_bDropped = false;
    m_pBotLabel->update();
}

void DefendItemWidget::UpdateStatus(bool bEnable)
{
    m_bEnabled = bEnable;
    m_pTopLabel->m_bEnabled = bEnable;
    m_pBotLabel->m_bEnabled = bEnable;
    m_pTopLabel->update();
    m_pBotLabel->update();
}

void DefendItemWidget::enterEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = true;
    update();
}

void DefendItemWidget::leaveEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bEntered = false;
    update();
}

void DefendItemWidget::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);
    QPainter painter(this);

    if(m_bEntered){
        painter.setPen(Qt::black);
        painter.drawRect(0, 0, DEFEND_ITEM_WIDGET_WIDTH-1, DEFEND_ITEM_WIDGET_HEIGHT-1);
    }else{
        painter.setPen(Qt::white);
        painter.drawRect(0, 0, DEFEND_ITEM_WIDGET_WIDTH-1, DEFEND_ITEM_WIDGET_HEIGHT-1);
    }
}

void DefendItemWidget::slotUpdateStatus()
{
    //qDebug("DefendItemWidget::slotUpdateStatus() occured");
    m_bEnabled = m_pTopLabel->m_bEnabled;
    m_pBotLabel->m_bEnabled = m_bEnabled;
    m_pBotLabel->update();
    emit clicked();
}

void DefendItemWidget::slotDropMenu()
{
    //qDebug("DefendItemWidget::slotDropMenu occured");
    emit valueChanged(m_nDefendType);
}
