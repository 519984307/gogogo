#ifndef DIRSELECTHEADERWIDGET_H
#define DIRSELECTHEADERWIDGET_H

#include <QObject>
#include <QWidget>

#include "common.h"

#include "pushbutton.h"

#define DIR_SELECT_HEADER_WIDGET_HEIGHT     36

class PushButton;


class DirSelectHeaderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DirSelectHeaderWidget(QWidget *parent = nullptr);

signals:

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *pPEvent);

public slots:

public:
    PushButton *m_pCloseBtn;

    QPoint  m_ptPressPoint;
    bool    m_bIsMove;
};

#endif // DIRSELECTHEADERWIDGET_H
