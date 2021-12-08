#ifndef UPDATERIMAGEBUTTON_H
#define UPDATERIMAGEBUTTON_H

//#include "common.h"

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QPaintEvent>


class QPushButton;

//typedef enum APP_ENUM_BUTTON_STATE{
//    BUTTON_STATE_NORMAL,
//    BUTTON_STATE_HOVER,
//    BUTTON_STATE_DOWN
//}ButtonState;

class UpdaterImageButton : public QPushButton
{
    Q_OBJECT
public:
    UpdaterImageButton(QString sStateImage, QWidget *parent = Q_NULLPTR);
    void SetButtonText(QString sText);

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
    QString m_sText;

};

#endif // UPDATERIMAGEBUTTON_H
