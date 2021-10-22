#ifndef ADDSCROLLWIDGET_H
#define ADDSCROLLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>

#include "pubdef.h"
#include "portgroup.h"
#include "mainwindow.h"


class PortGroup;


class AddScrollWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddScrollWidget(QWidget *parent, MainWindow *pMainWindow);

signals:

public slots:

public:
    MainWindow  *m_pMainWindow;

    QVBoxLayout *m_pLayout;

	PortGroup *m_pIpWhiteList;
	PortGroup *m_pIpBlackList;
	PortGroup *m_pIpPort;
};

#endif // ADDSCROLLWIDGET_H
