#ifndef TOPSCROLLWIDGET_H
#define TOPSCROLLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>

class OriGroup;
class WhiteListGroup;
class BackupGroup;
class KVGroup;
class TimerGroup;
class FileGroup;
class UDiskGroup;
class SystemGroup;
class AppGroup;

#include "pubdef.h"
#include "mainwindow.h"


class TopScrollWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopScrollWidget(QWidget *parent, MainWindow *pMainWindow, int function = 0x3F);

signals:

public slots:

public:
    MainWindow  *m_pMainWindow;

    QVBoxLayout *m_pLayout;

    OriGroup        *m_pOri;
    WhiteListGroup  *m_pWhiteList;
    BackupGroup     *m_pBackup;
    KVGroup         *m_pKV;
    TimerGroup      *m_pTimer;
    FileGroup       *m_pFile;
    UDiskGroup      *m_pUDisk;
//    SystemGroup     *m_pSystem;
//    AppGroup        *m_pApp;

};

#endif // TOPSCROLLWIDGET_H
