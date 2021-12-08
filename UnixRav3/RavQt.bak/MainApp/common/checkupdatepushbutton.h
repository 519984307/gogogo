#ifndef CHECKUPDATEPUSHBUTTON_H
#define CHECKUPDATEPUSHBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>

class QPushButton;

class CheckUpdatePushButton : public QPushButton
{
    Q_OBJECT
public:
    CheckUpdatePushButton();
    //set image
    CheckUpdatePushButton(QWidget *parent, QString sBackgroundImage);


public slots:
//    void clicked(bool checked = false);

protected:
    void enterEvent(QEvent *pEvent);
    void leaveEvent(QEvent *pEvent);
    void paintEvent(QPaintEvent *pPEvent);

private:
    //load static image
    int loadImage(QString sBackgroundImage);


public:
    bool m_bEntered;
    bool m_bLeave;

    QPixmap *m_ppBackgroundImage;
};

#endif // CHECKUPDATEPUSHBUTTON_H
