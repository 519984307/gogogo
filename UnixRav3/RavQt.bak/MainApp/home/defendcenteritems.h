#ifndef DEFENDCENTERITEMS_H
#define DEFENDCENTERITEMS_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QPushButton>

#include "common.h"

#define DEFENDCENTERTOPITEM_TOP_RECT_WIDTH  120
#define DEFENDCENTERTOPITEM_TOP_RECT_HEIGHT 120
#define DEFENDCENTERTOPITEM_HEIGHT          163



class DefendCenterItemRect: public QWidget
{
  Q_OBJECT
public:
    explicit DefendCenterItemRect(int nType, QWidget *parent = nullptr);

    void UpdateInfo(bool bEnabled);

protected:
    void enterEvent(QEvent *pEvent);
    void leaveEvent(QEvent *pEvent);
    void paintEvent(QPaintEvent *pEvent);


public:
    QPixmap *m_ppTopIcon;
    QPixmap *m_ppBotTxt;

    int     m_nType;
    bool    m_bEntered;
    bool    m_bEnabled;
};

class DefendCenterItemButton: public QPushButton
{
    Q_OBJECT
public:
    explicit DefendCenterItemButton(QWidget *parent = nullptr);

    void UpdateInfo(bool bEnabled);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *);

signals:
    void clicked();

public:
    QPixmap *m_ppIcon;

    bool    m_bEntered;
    bool    m_bEnabled;
    bool    m_bMousePressed;

};

class DefendCenterItem : public QWidget
{
    Q_OBJECT
public:
    explicit DefendCenterItem(int nType, QWidget *parent = nullptr);

    void UpdateInfo(bool bEnabled);

signals:
    void clicked();

public slots:
    void OnClickedBtn();

public:
    DefendCenterItemRect     *m_pTopRect;
    DefendCenterItemButton   *m_pBtn;

    int     m_nType;
    bool    m_bEnabled;
};




#endif // DEFENDCENTERITEMS_H
