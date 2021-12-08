#ifndef DEFENDBUTTON_H
#define DEFENDBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QPropertyAnimation>
#include "common.h"


class DefendButton : public QPushButton
{
    Q_OBJECT
public:
    DefendButton();
    //set image
    DefendButton(QString sBackgroundImage, QString sText=u8"", int nFontSize=10);

    void SetBtnText(QString sText, int nTotal, int nCurrent);

public slots:


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

    QString m_sText;
    int m_nTotal;
    int m_nCurrent;
    int m_nFontSize;

    QString m_sBackgroundImage;

    int m_nBackgroundImageWidth;
    int m_nBackgroundImageHeight;

    QPixmap *m_ppBackgroundImage;

};

#endif // DEFENDBUTTON_H
