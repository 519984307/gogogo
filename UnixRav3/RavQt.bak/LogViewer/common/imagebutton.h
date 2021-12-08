#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QPaintEvent>

#include "pubdef.h"


class QPushButton;

//typedef enum APP_ENUM_BUTTON_STATE{
//    BUTTON_STATE_NORMAL,
//    BUTTON_STATE_HOVER,
//    BUTTON_STATE_DOWN
//}ButtonState;

class ImageButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ImageButton(QString sStateImage, QWidget *parent = Q_NULLPTR);
    ~ImageButton();

    void ResetImage(QString sStateImage);

protected:
    void enterEvent(QEvent *pEvent);
    void leaveEvent(QEvent *pEvent);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *pPEvent);

private:
    void LoadImage(QString sStateImage);

private:
    bool m_bEntered;
    bool m_bLeave;
    bool m_bMousePress;

    QPixmap *m_ppStateImage;

};

#endif // IMAGEBUTTON_H
