#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>

#include "communicatebundle.h"

class PopScreen;
class CondLocker;
class WidgetObserver;
class AppConfig;
class ScreenWidget : public QWidget
{
     Q_OBJECT
public:
    ScreenWidget(AppConfig *config, QWidget *parent = NULL);
    virtual ~ScreenWidget();

    void Start(bool tray);

    void ShowTips(CommunicateBundle& bundle);

    void SetTrayIcon(bool state);

    void paintEvent(QPaintEvent *event);

    void SetObserver(WidgetObserver *observer) {
        mObserver = observer;
    }

    int GetSelected() const {
        return mCommunicateBundle.mSelected;
    }

private:
    int InitTrayIcon();
    int InitMenu();

private slots:
    void ShowMessage();
    void onPopEnd(int selected);

    void OpenMainScreen();
    void OpenSettings();
    void OpenLogView();
    void Update();
    void ExitAll();

    void IconActived(QSystemTrayIcon::ActivationReason reson);

signals:
    void tip();

private:
    QSystemTrayIcon *mTrayIcon;
    QMenu           *mMenu;
    PopScreen       *mPopWidget;

private:
    CommunicateBundle mCommunicateBundle;
    CondLocker  *mCondLocker;

private:
    WidgetObserver  *mObserver;
    AppConfig       *mAppConfig;
};

#endif // SCREENWIDGET_H
