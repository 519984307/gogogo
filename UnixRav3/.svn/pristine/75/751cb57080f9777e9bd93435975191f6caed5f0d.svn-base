#ifndef DEFENDCENTERMIDWIDGET_H
#define DEFENDCENTERMIDWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "common.h"

class PushButton;
class IpItemModel;
class IpIconDelegate;
class BlacklistIPTable;
class UrlItemModel;
class UrlIconDelegate;
class BlacklistUrlTable;
class MainWindow;

#define DEFENDCENTER_MID_WIDGET_HEIGHT      270//258

class DefendCenterItem;

class DefendCenterMidWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DefendCenterMidWidget(MainWindow *pMainWindow, QWidget *parent = nullptr);

    void SetMainWindow(MainWindow *pMainWindow);
    void InitTable();

signals:

public slots:
    void OnClickedIpAdd();
    void OnClickedUrlAdd();



public:
    QLabel  *m_plIcon;
    //CheckboxLabel   *m_pclStartBlacklist;

    PushButton      *m_ppbIpAdd;
    PushButton      *m_ppbUrlAdd;

    BlacklistIPTable    *m_pIPTable;
    BlacklistUrlTable   *m_pUrlTable;


    QPushButton  *m_ppbApply;

    MainWindow  *m_pMainWindow;

//    DefendCenterItem    *m_pItemFeiKe;
//    DefendCenterItem    *m_pItemYuYun;
//    DefendCenterItem    *m_pItemWeiKe;
//    DefendCenterItem    *m_pItemDll;
};

#endif // DEFENDCENTERMIDWIDGET_H
