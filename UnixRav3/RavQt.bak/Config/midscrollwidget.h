#ifndef MIDSCROLLWIDGET_H
#define MIDSCROLLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>

#include "pubdef.h"

class NetBlacklistGroup;

class AppGroup;



class MidScrollWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MidScrollWidget(QWidget *parent = nullptr);

signals:

public slots:

public:
    QVBoxLayout *m_pLayout;

    NetBlacklistGroup        *m_pBlacklist;


};

#endif // MIDSCROLLWIDGET_H
