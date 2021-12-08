#ifndef DEFENDCENTERDIALOG_H
#define DEFENDCENTERDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>

#include "Entry/urlruleentry.h"

#include "common.h"

class PushButton;
class DefendCenterItem;
class DefendCenterHeaderWidget;
class DefendCenterTitleWidget;
class DefendCenterTopWidget;
class DefendCenterMidWidget;
class DefendCenterBotWidget;
class MainWindow;


class DefendCenterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DefendCenterDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    void SetMainWindow(MainWindow *pMainWindow);

    bool InUrlList(list<UrlRuleEntry> &ulist, UrlRuleEntry & uentry);

public slots:
    void OnClickedItem();
    void OnClickedApply();

protected:
    void paintEvent(QPaintEvent *pEvent);

public:
    DefendCenterHeaderWidget    *m_pHeaderWidget;
    //DefendCenterTitleWidget     *m_pTitleWidget;
    DefendCenterTopWidget       *m_pTopWidget;
    DefendCenterMidWidget       *m_pMidWidget;
    //DefendCenterBotWidget       *m_pBotWidget;

    MainWindow  *m_pMainWindow;

};

#endif // DEFENDCENTERDIALOG_H
