#include <QHBoxLayout>
#include <QFont>
#include <QPalette>


#include "common/himagebutton.h"
#include "common/showbutton.h"

#include "defendstatewidget.h"

const QString sDefendStateTxt[] = {u8"上网防护未全部开启，可能存在风险！", u8"上网防护正在保护您的上网安全！"};



DefendStateWidget::DefendStateWidget(QWidget *parent) : QWidget(parent)
{
    //m_ppIcon = new QPixmap(":/resource/netdefend_icon");
    m_plIcon = new ShowButton(this);
    m_plIcon->loadPixmap(":/resource/netdefend_icon");
    //m_plIcon->setFixedSize(m_ppIcon->size());
    m_plDesc = new QLabel;
    m_pbtnOpenAll = new HImageButton(":/resource/open_all_defend");

    SetNormalState(true);
}

void DefendStateWidget::SetNormalState(bool bEnable)
{
    QFont ft;
    ft.setPixelSize(24);
    m_plDesc->setFont(ft);
    m_plDesc->adjustSize();

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    m_plDesc->setPalette(pa);

    if(bEnable){
        m_plDesc->setText(sDefendStateTxt[1]);
        m_pbtnOpenAll->hide();
    }else{
        m_plDesc->setText(sDefendStateTxt[0]);
        m_pbtnOpenAll->show();
    }
    m_plDesc->adjustSize();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addSpacing(70);
    layout->addWidget(m_plIcon, 0, Qt::AlignLeft|Qt::AlignVCenter);
    layout->addWidget(m_plDesc, 0, Qt::AlignLeft|Qt::AlignVCenter);
    layout->addWidget(m_pbtnOpenAll, 0, Qt::AlignLeft|Qt::AlignVCenter);
    layout->addStretch();
    setLayout(layout);
}
