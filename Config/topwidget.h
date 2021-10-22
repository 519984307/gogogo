#ifndef TOPWIDGET_H
#define TOPWIDGET_H

#include <QObject>
#include <QWidget>
#include <QIcon>

#include "common/pubdef.h"
#include "common/pushbutton.h"

class PushButton;

class TopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopWidget(QWidget *parent = nullptr);

    void InitWidget();
    void SetTitle(QString &sTitle);

protected:
    //void enterEvent(QEvent *pEvent);
    //void leaveEvent(QEvent *pEvent);
    void paintEvent(QPaintEvent *pPEvent);

signals:

public slots:

public:
    QIcon  *m_pIcon;
    PushButton  *m_pCloseBtn;
    //PushButton  *m_pMinBtn;
    QString m_sTitle;
};

#endif // TOPWIDGET_H
