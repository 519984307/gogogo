#include <QVBoxLayout>
#include "dirselectbotbodywidget.h"

DirSelectBotBodyWidget::DirSelectBotBodyWidget(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(DIR_SELECT_BOT_BODY_WIDGET_HEIGHT-1);

    QPalette pal(palette());

    pal.setColor(QPalette::Background, QColor(237,237,237));
    setAutoFillBackground(true);
    setPalette(pal);

    m_pBtn = new ImageButton(":/resource/start_btn", this);
    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(m_pBtn);
    layout->setAlignment(m_pBtn, Qt::AlignHCenter);
    setLayout(layout);
}
