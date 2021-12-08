#ifndef DEFENDWIDGET_H
#define DEFENDWIDGET_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QList>
#include <QMenu>
#include <QActionGroup>
#include <QAction>

#include "common.h"

class DefendTopWidget;
class DefendBotWidget;
class DefendItemWidget;

class DefendTopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DefendTopWidget(QWidget *parent = nullptr);

    void UpdateDesc();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *pEvent);

signals:

public slots:

public:
    QLabel  *m_plDesc;
    int     m_nItemCount;
    int     m_nOpenedItem;

    DefendItemWidget    *m_pTorjan;
    DefendItemWidget    *m_pFishing;
    DefendItemWidget    *m_pDownload;
    DefendItemWidget    *m_pHacker;
    DefendItemWidget    *m_pScripting;
    DefendItemWidget    *m_pSearchEngine;
    DefendItemWidget    *m_pAdFilter;
    DefendItemWidget    *m_pOuterAttack;
    DefendItemWidget    *m_pAntiArp;

    QList<DefendItemWidget*> m_lWidgets;

};

class DefendBotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DefendBotWidget(QWidget *parent = nullptr);

    void UpdateInfo();

protected:
    void paintEvent(QPaintEvent *pEvent);

public:
    QString m_sMalDBVersion;
    QString m_sLastUpdateTime;

    QLabel  * m_pLabel;
};

class DefendWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DefendWidget(QWidget *parent = nullptr);
    void CreateMenu();

    void OpenAllDefendItem();

public slots:
    void slotUpdateStatus();
    void slotDropMenu(int);
    void trigerMenu(QAction *action);
    void slotMenuHide();

private:
    void setBackgroud(QString image);

protected:
    void resizeEvent(QResizeEvent *event);

signals:
    void valueChanged(int nEnabled);

public:
    DefendTopWidget *m_pTopWidget;
    DefendBotWidget *m_pBotWidget;

    QFrame  *m_pLine;

    QMenu   *m_pMenu;
    QActionGroup   *m_pGroupAct;
    QAction *m_pActOpenClose;
    QAction *m_pActPermanentOpenClose;

    DefendItemWidget    *m_pCurrentItem;
};


#endif // DEFENDWIDGET_H
