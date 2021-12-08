#ifndef DEFENDCENTERTITLEWIDGET_H
#define DEFENDCENTERTITLEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>

#include "common.h"

class DefendCenterTitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DefendCenterTitleWidget(QWidget *parent = nullptr);

    void UpdateInfo(int nTotal, int nOpened);

signals:

public slots:
public:
    QLabel  *m_plTitle;

    int m_nTotalCount;
    int m_nOpenedCount;
};

#endif // DEFENDCENTERTITLEWIDGET_H
