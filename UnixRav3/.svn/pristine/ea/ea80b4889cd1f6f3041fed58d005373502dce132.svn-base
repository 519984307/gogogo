#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

#include "pubdef.h"

class ClickedLabel : public QLabel
{
    Q_OBJECT
public:
    ClickedLabel(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ClickedLabel(const QString &text, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

signals:
    void clicked();


protected:
    void mousePressEvent(QMouseEvent *event);


};

#endif // CLICKEDLABEL_H
