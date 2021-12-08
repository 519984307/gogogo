#ifndef IMGTXTLABEL_H
#define IMGTXTLABEL_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <Qt>
#include <QPixmap>

#include "common.h"

class ImgTxtLabel : public QPushButton
{
public:
    //ImgTxtLabel(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ImgTxtLabel(QWidget *parent = Q_NULLPTR);
    ~ImgTxtLabel();

    void SetText(QString sText);    //called after LoadPixmap function

    void SetDisable(bool bDisable);
    bool IsDisable(){return m_bDisabled;}

    void SetSelected(bool bSelected);
    bool IsSelected(){return m_bSelected;}

signals:
    //void Clicked(ImgTxtLabel* pLabel);

protected:
    void enterEvent(QEvent *pEvent);
    void leaveEvent(QEvent *pEvent);


    void mouseMoveEvent(QMouseEvent *pEvent);
    void mousePressEvent(QMouseEvent *pEvent);
    void mouseReleaseEvent(QMouseEvent *pEvent);


    void paintEvent(QPaintEvent *pEvent);

public:
    bool    m_bDisabled;
    bool    m_bSelected;

    bool    m_bEntered;
    bool    m_bMousePressed;


    QString m_sText;

    QPixmap *m_ppImage;

};

#endif // IMGTXTLABEL_H
