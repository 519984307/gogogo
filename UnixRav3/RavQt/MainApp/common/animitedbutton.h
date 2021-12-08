#ifndef ANIMITEDBUTTON_H
#define ANIMITEDBUTTON_H

#include <QObject>
#include <QWidget>
#include <QList>
#include <QPixmap>
#include <QPushButton>
#include <QPropertyAnimation>

#include "common.h"


class AnimitedButton : public QPushButton
{
    Q_OBJECT
public:
    AnimitedButton();
    //set image
    AnimitedButton(QString sBackgroundImage, QString sCenterAnimitedImage, QString sText=u8"", int nFontSize=12);

public slots:
    void enterImageChange(QVariant qv);

protected:
    void enterEvent(QEvent *pEvent);
    void leaveEvent(QEvent *pEvent);
    void paintEvent(QPaintEvent *pPEvent);

private:
    //load static image
    int loadImage(QString sBackgroundImage, QString sAniminationImage);


private:
    bool m_bEntered;
    bool m_bLeave;

    QString m_sText;
    int m_nFontSize;

    int m_nCenterImgIndex;
    int m_nBackgroundImageWidth;
    int m_nBackgroundImageHeight;
    int m_nCenterImgWidth;
    int m_nCenterImgHeight;

    QPropertyAnimation *m_ppaAnimination;

    QPixmap *m_ppBackgroundImage;
    QPixmap *m_ppCenterAnimitedImage;

    QList<QPixmap> m_lstCenterImage;

};

#endif // ANIMITEDBUTTON_H
