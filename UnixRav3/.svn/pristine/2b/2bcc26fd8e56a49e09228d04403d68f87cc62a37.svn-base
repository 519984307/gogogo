#include <QHBoxLayout>


#include "common.h"
#include "defendcentertitlewidget.h"

DefendCenterTitleWidget::DefendCenterTitleWidget(QWidget *parent) : QWidget(parent)
{
    QPalette pal(palette());

    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    setFixedSize(APP_DEFEND_CENTER_DIALOG_WIDTH-2, APP_DEFEND_CENTER_DIALOG_TITLE_HEIGHT);

    m_plTitle = new QLabel;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 6, 0);
    layout->addWidget(m_plTitle, 0, Qt::AlignRight|Qt::AlignVCenter);
    setLayout(layout);

    UpdateInfo(8, 2);
}

void DefendCenterTitleWidget::UpdateInfo(int nTotal, int nOpened)
{//
    const QString sDesc[] = {u8"安全防护未全部开启！", u8"安全防护已全部开启！"};
    bool bsafe = nTotal==nOpened?true:false;

    QString stext = QString("<font color=black size=4>  %1(</font><font color=red size=4>%2</font><font color=black size=4>/%3)</font>")
                        .arg(sDesc[bsafe?1:0]).arg(nOpened,0,10).arg(nTotal,0,10);
    m_plTitle->setText(stext);

}
