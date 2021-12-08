#ifndef WEEKDAYSELECTOR_H
#define WEEKDAYSELECTOR_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <vector>

#include "pubdef.h"

const QString WeekTxt[] = {u8"一", u8"二", u8"三", u8"四", u8"五", u8"六", u8"日"};


class WeekDayLabel : public QLabel
{
    Q_OBJECT
public:
    //nWeekDay: 1 2 3 4 5 6 0
    explicit WeekDayLabel(int nWeekDay, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    void SetSelected(bool bSelected);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);

signals:
    void clicked();

public:
    bool    m_bEntered;
    bool    m_bMouseDown;
    bool    m_bSelected;
    int     m_nWeekDay;
};


class WeekDaySelector : public QWidget
{
    Q_OBJECT
public:
    explicit WeekDaySelector(QWidget *parent = nullptr);

    void Reset();
    void GetSelected(std::vector<int>& days);
    void SetSelected(std::vector<int>& days);

signals:

public slots:
    void OnClickedDay();

public:
    WeekDayLabel    *m_pMonday;
    WeekDayLabel    *m_pTuesday;
    WeekDayLabel    *m_pWednesday;
    WeekDayLabel    *m_pThursday;
    WeekDayLabel    *m_pFriday;
    WeekDayLabel    *m_pSaturday;
    WeekDayLabel    *m_pSunday;
};

#endif // WEEKDAYSELECTOR_H
