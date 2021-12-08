#ifndef LICENSEBUTTON_H
#define LICENSEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QPushButton>

#include "common.h"


class LicenseButton : public QPushButton
{
    Q_OBJECT
public:
    LicenseButton(QString sStateImage, QWidget *parent = Q_NULLPTR);

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

#endif // LICENSEBUTTON_H
