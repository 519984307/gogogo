#ifndef DefendItemWidget_H
#define DefendItemWidget_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QMouseEvent>
#include <QPaintEvent>

#include "common.h"


#define DEFEND_ITEM_WIDGET_WIDTH     145
#define DEFEND_ITEM_WIDGET_HEIGHT    144

const QString sDefendTypeDesc[]={
    u8"拦截恶意木马网址",
    u8"拦截钓鱼网址",
    u8"拦截恶意下载",
    u8"防黑客攻击",
    u8"拦截跨站脚本攻击",
    u8"搜索引擎结果检查",
    u8"广告过滤",
    u8"拦截对外攻击",
    u8"反ARP欺诈",
    u8"更多防护"
};

const QString sDefendStatusText[]={
    u8"已开启",
    u8"已关闭",
    u8"重启前关闭",
    u8"重启前开启",
    u8"永久关闭",
    u8"永久开启"
};


class DefendTopLabel:public QLabel
{
    Q_OBJECT
public:
    explicit DefendTopLabel(int nDefendType, QWidget *parent = nullptr);

    void SetEnabled(bool bEnable);
    void SetCount(int nCount);

protected:
    void enterEvent(QEvent *pEvent);
    void leaveEvent(QEvent *pEvent);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *pEvent);

signals:
    void clicked();

public:
    int m_nDefendType;
    bool    m_bEnabled;
    bool    m_bEntered;

    QPixmap     *m_ppTypeIcon;
    int     m_nIconWidth;
    int     m_nIconHeight;

    int     m_nScreenCount;
};

class DefendBotLabel:public QLabel
{
    Q_OBJECT
public:
    explicit DefendBotLabel(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *pEvent);

signals:
    void clicked();

public:
    bool    m_bEnabled;
    bool    m_bDropped;
    QPixmap     *m_ppDropItemArrow;

};

class DefendItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DefendItemWidget(int nDefendType, QWidget *parent = nullptr);

    int DefendType(){return m_nDefendType;}
    void ResetDropMenu();
    void UpdateStatus(bool bEnable);

protected:
    void enterEvent(QEvent *pEvent);
    void leaveEvent(QEvent *pEvent);
    void paintEvent(QPaintEvent *pEvent);


signals:
    void clicked();
    void valueChanged(int);

public slots:
    void slotUpdateStatus();
    void slotDropMenu();

public:
    int     m_nDefendType;
    bool    m_bEnabled;
    bool    m_bEntered;

    DefendTopLabel  *m_pTopLabel;
    DefendBotLabel  *m_pBotLabel;

};

#endif // DefendItemWidget_H
