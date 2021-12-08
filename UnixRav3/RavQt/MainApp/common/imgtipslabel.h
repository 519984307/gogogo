#ifndef ImgTipsLabel_H
#define ImgTipsLabel_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QModelIndex>
#include <QPropertyAnimation>

#include "common.h"

class QLabel;


class ImgTipsLabel : public QLabel
{
    Q_OBJECT
public:
    ImgTipsLabel();
    ImgTipsLabel(QString sIcoImage, QString sTipsImage, ImgLabelType eType);


protected:
//    bool event(QEvent *pEvent);

    void enterEvent(QEvent *pEvent);
    void leaveEvent(QEvent *pEvent);
    void paintEvent(QPaintEvent *pPEvent);

public slots:
    void enterTipsChange(QVariant qv);

private:
    int LoadImage(QString sIcoImage, QString sTipsImage);

private:
    ImgLabelType    m_eType;

public:
    bool m_bEntered;
    bool m_bLeave;
    bool m_bTipsEventOccur;


    QPixmap *m_ppIcoImage;
    //QPixmap *m_ppTextImage;
    QPixmap *m_ppTipsImage;

    QPropertyAnimation *m_ppaAnimination;
};

#endif // ImgTipsLabel_H
