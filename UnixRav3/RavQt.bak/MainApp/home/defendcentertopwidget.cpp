#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPalette>

#include "common.h"
#include "defendcenteritems.h"
#include "defendcentertopwidget.h"
#include "appconfig.h"

DefendCenterTopWidget::DefendCenterTopWidget(QWidget *parent, int functions) : QWidget(parent)
{
    QPalette pal(palette());

    pal.setColor(QPalette::Background, QColor(252,252,252));
    //setAutoFillBackground(true);
    //setPalette(pal);

    //setStyleSheet("DefendCenterTopWidget {background: red}");

    setFixedSize(APP_DEFEND_CENTER_DIALOG_WIDTH-2, DEFENDCENTER_TOP_WIDGET_HEIGHT);

    m_plIcon = new QLabel;
    QPixmap picon(":/resource/defendcenter_sub_title");
    m_plIcon->setFixedSize(picon.width(), picon.height()/3);
    m_plIcon->setPixmap(picon.copy(0, 0, picon.width(), picon.height()/3));

    m_pItemFile = new DefendCenterItem(RAV_QT_DEFENDCENTER_ITEM_TYPE_FILE);
//    m_pItemMail = new DefendCenterItem(RAV_QT_DEFENDCENTER_ITEM_TYPE_MAIL);
//    m_pItemShare = new DefendCenterItem(RAV_QT_DEFENDCENTER_ITEM_TYPE_SHARE);
    m_pItemUDisk = new DefendCenterItem(RAV_QT_DEFENDCENTER_ITEM_TYPE_UDISK);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->setSpacing(0);
    hlayout->addSpacing(40);
    if (0 != (functions & AppConfig::FILE_DEFEN)) {
        hlayout->addWidget(m_pItemFile, 0, Qt::AlignLeft|Qt::AlignVCenter);
    }
//    hlayout->addWidget(m_pItemMail, 0, Qt::AlignLeft|Qt::AlignVCenter);
//    hlayout->addWidget(m_pItemShare, 0, Qt::AlignLeft|Qt::AlignVCenter);
    if (0 != (functions & AppConfig::PLUGIN_CHECKER)) {
        hlayout->addWidget(m_pItemUDisk, 0, Qt::AlignLeft|Qt::AlignVCenter);
    }
    hlayout->addStretch();

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setSpacing(0);
    vlayout->addWidget(m_plIcon, 0, Qt::AlignLeft);
    vlayout->addLayout(hlayout);

    setLayout(vlayout);
}
