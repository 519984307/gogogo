#ifndef MIDBODYWIDGET_H
#define MIDBODYWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPixmap>

#include "defendbutton.h"
#include "imgtipslabel.h"
#include "common.h"
#include "appconfig.h"

class DefendButton;
class ImgTipsLabel;

class MidBodyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MidBodyWidget(QWidget *parent = nullptr);

    void LoadMidTextArcImage();
    void LoadButton();
    void LoadImgTipsLabel();

    void SetBtnValue(int nTotal, int nCurrent);
    void SetFunctions(int nFunctions);

signals:

public slots:
    //void showToolTip(const QModelIndex &index);

protected:
    bool event(QEvent *pEvent);
    void paintEvent(QPaintEvent *pPEvent);

private:
    bool m_bTipsEventOccur;

    QRect m_rLastTipsRect;
    QPixmap m_pixLastTipsBackground;
    QLabel *m_pTipsLabel;

public:
    DefendButton    *m_pDefendButton;

    QPixmap *m_ppTextImage;
    QPixmap *m_ppArcImage;
    QPixmap *m_ppEngineNameText;

    ImgTipsLabel    *m_pBasicEngine;
    QLabel          *m_pBasicEngineText;
    ImgTipsLabel    *m_pAdminEngine;
    QLabel          *m_pAdminEngineText;
    ImgTipsLabel    *m_pCloudEngine;
    QLabel          *m_pCloudEngineText;
    ImgTipsLabel    *m_pGeneEngine;
    QLabel          *m_pGeneEngineText;

};

#endif // MIDBODYWIDGET_H
