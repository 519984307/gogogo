#include <QGridLayout>
#include <QVBoxLayout>

#include <QPainter>
#include <QPalette>

#include "defenditemwidget.h"
#include "defendwidget.h"

const QString sMenuTxt[] = { u8"重启前开启", u8"重启前关闭", u8"永久开启", u8"永久关闭"};

DefendTopWidget::DefendTopWidget(QWidget *parent) : QWidget(parent)
{
//    QPalette pal(palette());
//    pal.setColor(QPalette::Background, Qt::white);
//    setAutoFillBackground(true);
//    setPalette(pal);
    setFixedHeight(APP_BODY_HEIGHT - 50);

    m_plDesc =new QLabel(this);
    m_pTorjan = new DefendItemWidget(RAV_QT_DEFEND_TYPE_TROJAN);
    m_pFishing = new DefendItemWidget(RAV_QT_DEFEND_TYPE_FISHING);
    m_pDownload = new DefendItemWidget(RAV_QT_DEFEND_TYPE_EVIL_DOWNLOAD);
    m_pHacker = new DefendItemWidget(RAV_QT_DEFEND_TYPE_ANTI_HACKER);
    m_pScripting = new DefendItemWidget(RAV_QT_DEFEND_TYPE_SITE_SCRIPTING);
    m_pSearchEngine = new DefendItemWidget(RAV_QT_DEFEND_TYPE_SEARCH_ENGINE);
    m_pAdFilter = new DefendItemWidget(RAV_QT_DEFEND_TYPE_AD_FILTER);
    m_pOuterAttack = new DefendItemWidget(RAV_QT_DEFEND_TYPE_OUTER_ATTACK);
    m_pAntiArp = new DefendItemWidget(RAV_QT_DEFEND_TYPE_ANTI_ARP);

    m_nItemCount = 9;
    m_nOpenedItem = 0;
    //上网防护项()
    QGridLayout  *layout = new QGridLayout;
    layout->setContentsMargins(6, 4, 6,4);
    layout->setSpacing(4);
    //layout->addWidget(m_plDesc, 0, 0, 1, 5, Qt::AlignLeft|Qt::AlignVCenter);
    layout->addWidget(m_pTorjan, 0, 0, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(m_pFishing, 0, 1, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(m_pDownload, 0, 2, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(m_pHacker, 0, 3, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(m_pScripting, 0, 4, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(m_pSearchEngine, 1, 0, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(m_pAdFilter, 1, 1, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(m_pOuterAttack, 1, 2, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);
    layout->addWidget(m_pAntiArp, 1, 3, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);

    m_lWidgets.append(m_pTorjan);
    m_lWidgets.append(m_pFishing);
    m_lWidgets.append(m_pDownload);
    m_lWidgets.append(m_pHacker);
    m_lWidgets.append(m_pScripting);
    m_lWidgets.append(m_pSearchEngine);
    m_lWidgets.append(m_pAdFilter);
    m_lWidgets.append(m_pOuterAttack);
    m_lWidgets.append(m_pAntiArp);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(10,10, 10, 6);
    vlayout->addSpacing(5);
    vlayout->addLayout(layout);
    setLayout(vlayout);

}

void DefendTopWidget::UpdateDesc()
{
    QString stext = QString("<font color=black size=4>  上网防护项(</font><font color=red size=4>%1</font><font color=black size=4>/%2)</font>")
                    .arg(m_nOpenedItem,0,10).arg(m_nItemCount,0,10);
    m_plDesc->setText(stext);

    m_plDesc->setGeometry(20, 0, 150, 20);
    //qDebug("DefendTopWidget::UpdateDesc occur");
    //update();
}

void DefendTopWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    UpdateDesc();
}

void DefendTopWidget::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);
//    QPainter painter(this);
//    QRect textPos(15, 0, 200, 20);

//    QString stext = QString("<font color=black size=8>  上网防护项(</font><font color=red size=8>%1</font><font color=black size=8>/%2)</font>")
//                    .arg(m_nOpenedItem,0,10).arg(m_nItemCount,0,10);
//    painter.drawText(textPos, Qt::AlignVCenter|Qt::AlignLeft, stext);
}


DefendBotWidget::DefendBotWidget(QWidget *parent):QWidget(parent)
{
    m_sMalDBVersion = "24.00.56.15";
    m_sLastUpdateTime = "2018-04-04 11:19";
    setFixedHeight(APP_BOT_BODY_HEIGHT);

    m_pLabel = new QLabel(this);
    UpdateInfo();
}

void DefendBotWidget::UpdateInfo()
{
    QString stext = QString("恶意网址库版本：(%1)   上次更新：%2 ").arg(m_sMalDBVersion).arg(m_sLastUpdateTime);
    m_pLabel->setText(stext);
    m_pLabel->adjustSize();
    QFont font;
    font.setPixelSize(10);
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#ifdef QT_NEW_HEADER_54
    font.setWeight(0);
    m_pLabel->setFont(font);
#else
    font.setWeight(25);
    m_pLabel->setFont(font);
#endif
#else //QT_VERSION
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
    m_pLabel->setFont(font);
#else
    font.setWeight(QFont::Light);
    m_pLabel->setFont(font);
#endif
#endif
    m_pLabel->setGeometry(30, 0, 400, 35);
}

void DefendBotWidget::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);
//    QString stext = QString("恶意网址库版本：(%1)   上次更新：%2 ").arg(m_sMalDBVersion).arg(m_sLastUpdateTime);
//    QPainter painter(this);
//    painter.setFont(QFont( "Timers" , 10 ,  QFont::Thin));
//    QRect txtPos(20, 0, width()-12, height());
//    painter.drawText(txtPos, Qt::AlignVCenter|Qt::AlignLeft, stext);
}

DefendWidget::DefendWidget(QWidget *parent):QWidget(parent),m_pCurrentItem(NULL)
{
    setFixedHeight(APP_BODY_HEIGHT);

    m_pTopWidget = new DefendTopWidget(this);
    m_pBotWidget = new DefendBotWidget(this);

    m_pLine = new QFrame(this);
    m_pLine->setObjectName(QString("line"));
    m_pLine->setFrameShape(QFrame::HLine);
    m_pLine->setFrameShadow(QFrame::Raised);

    QVBoxLayout * layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_pTopWidget);
    //layout->addWidget(m_pLine);
    layout->addWidget(m_pBotWidget);
    setLayout(layout);

    connect(m_pTopWidget->m_pTorjan, SIGNAL(clicked()), this, SLOT(slotUpdateStatus()));
    connect(m_pTopWidget->m_pFishing, SIGNAL(clicked()), this, SLOT(slotUpdateStatus()));
    connect(m_pTopWidget->m_pDownload, SIGNAL(clicked()), this, SLOT(slotUpdateStatus()));
    connect(m_pTopWidget->m_pHacker, SIGNAL(clicked()), this, SLOT(slotUpdateStatus()));
    connect(m_pTopWidget->m_pScripting, SIGNAL(clicked()), this, SLOT(slotUpdateStatus()));
    connect(m_pTopWidget->m_pSearchEngine, SIGNAL(clicked()), this, SLOT(slotUpdateStatus()));
    connect(m_pTopWidget->m_pAdFilter, SIGNAL(clicked()), this, SLOT(slotUpdateStatus()));
    connect(m_pTopWidget->m_pOuterAttack, SIGNAL(clicked()), this, SLOT(slotUpdateStatus()));
    connect(m_pTopWidget->m_pAntiArp, SIGNAL(clicked()), this, SLOT(slotUpdateStatus()));

    connect(m_pTopWidget->m_pTorjan, SIGNAL(valueChanged(int)), this, SLOT(slotDropMenu(int)));
    connect(m_pTopWidget->m_pFishing, SIGNAL(valueChanged(int)), this, SLOT(slotDropMenu(int)));
    connect(m_pTopWidget->m_pDownload, SIGNAL(valueChanged(int)), this, SLOT(slotDropMenu(int)));
    connect(m_pTopWidget->m_pHacker, SIGNAL(valueChanged(int)), this, SLOT(slotDropMenu(int)));
    connect(m_pTopWidget->m_pScripting, SIGNAL(valueChanged(int)), this, SLOT(slotDropMenu(int)));
    connect(m_pTopWidget->m_pSearchEngine, SIGNAL(valueChanged(int)), this, SLOT(slotDropMenu(int)));
    connect(m_pTopWidget->m_pAdFilter, SIGNAL(valueChanged(int)), this, SLOT(slotDropMenu(int)));
    connect(m_pTopWidget->m_pOuterAttack, SIGNAL(valueChanged(int)), this, SLOT(slotDropMenu(int)));
    connect(m_pTopWidget->m_pAntiArp, SIGNAL(valueChanged(int)), this, SLOT(slotDropMenu(int)));

    CreateMenu();
}

void DefendWidget::CreateMenu()
{
    //重启前开启 重启前关闭 永久开启 永久关闭
    m_pMenu = new QMenu(this);
    m_pGroupAct = new QActionGroup(this);
    m_pGroupAct->setExclusive(true);

    m_pActOpenClose = new QAction(m_pGroupAct);
    m_pActPermanentOpenClose = new QAction(m_pGroupAct);
    m_pMenu->addAction(m_pActOpenClose);
    m_pMenu->addAction(m_pActPermanentOpenClose);

    connect(m_pGroupAct, SIGNAL(triggered(QAction*)), this, SLOT(trigerMenu(QAction*)));
    connect(m_pMenu, SIGNAL(aboutToHide()), this, SLOT(slotMenuHide()));
}

void DefendWidget::OpenAllDefendItem()
{
    QList<DefendItemWidget*>::iterator it = m_pTopWidget->m_lWidgets.begin();
    while(it != m_pTopWidget->m_lWidgets.end()){
        DefendItemWidget *pwidget = *it;
        pwidget->UpdateStatus(true);
        it++;
    }
    m_pTopWidget->m_nOpenedItem = m_pTopWidget->m_nItemCount;
    m_pTopWidget->UpdateDesc();
}

void DefendWidget::setBackgroud(QString image)
{
    setAutoFillBackground(true);
    QPixmap pixmap = QPixmap(image).scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(this->backgroundRole(), QBrush(pixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

    this->setPalette(palette);
    m_pLine->setGeometry(QRect(this->x()+10, this->height()-50, this->width()-20, 3));

}

void DefendWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    setBackgroud(":/resource/body_bg");
}

void DefendWidget::slotUpdateStatus()
{
    int nopened = 0;

    QList<DefendItemWidget*>::iterator it = m_pTopWidget->m_lWidgets.begin();
    while(it != m_pTopWidget->m_lWidgets.end()){
        DefendItemWidget *pwidget = *it;
        if(pwidget->m_bEnabled)
            nopened++;
        it++;
    }
    m_pTopWidget->m_nOpenedItem = nopened;
    m_pTopWidget->UpdateDesc();
    emit valueChanged(nopened == m_pTopWidget->m_nItemCount?1:0);
}

void DefendWidget::slotDropMenu(int nType)
{
    //qDebug("DefendWidget::slotDropMenu nType=%d", nType);
    QList<DefendItemWidget*>::iterator it = m_pTopWidget->m_lWidgets.begin();
    while(it != m_pTopWidget->m_lWidgets.end()){
        DefendItemWidget *pwidget = *it;
        if(pwidget->m_nDefendType == nType){
            m_pActOpenClose->setText(sMenuTxt[pwidget->m_bEnabled?1:0]);
            m_pActPermanentOpenClose->setText(sMenuTxt[pwidget->m_bEnabled?3:2]);
            m_pCurrentItem = pwidget;
            //qDebug("xxxx 0");
            QPoint pt(mapToGlobal(QPoint(pwidget->x(), pwidget->y()+pwidget->height())));
            m_pMenu->setFixedWidth(pwidget->width());
            m_pMenu->move(pt);
            m_pMenu->show();
        }else{
            pwidget->ResetDropMenu();
        }
        it++;
    }
}

void DefendWidget::trigerMenu(QAction *action)
{
    //qDebug("DefendWidget::trigerMenu occur");
    if(m_pCurrentItem == NULL)
        return;

    if(action == m_pActOpenClose){
        m_pCurrentItem->UpdateStatus(!m_pCurrentItem->m_bEnabled);
    }else if(action == m_pActPermanentOpenClose){
        m_pCurrentItem->UpdateStatus(!m_pCurrentItem->m_bEnabled);
    }

    m_pCurrentItem = NULL;
}

void DefendWidget::slotMenuHide()
{
    QList<DefendItemWidget*>::iterator it = m_pTopWidget->m_lWidgets.begin();
    while(it != m_pTopWidget->m_lWidgets.end()){
        DefendItemWidget *pwidget = *it;
        pwidget->ResetDropMenu();
        it++;
    }
}
