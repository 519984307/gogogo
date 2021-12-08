#ifndef SCANTOPBODYWIDGET_H
#define SCANTOPBODYWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include "common.h"

class AnimitedButton;
class ImgTxtLabel;

class ScanTopBodyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScanTopBodyWidget(QWidget *parent = nullptr, bool scanIntelligent=true);
    ~ScanTopBodyWidget();

    void UpdateScanInfo(int nThreads, int nLocalThreaten, int nCloudThreaten, QString sFile);
    void ResetInfo();

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *event);

    void UpdateInfo();

public:
    //QLabel  *lbl1;
    QProgressBar *progressbar;

    QLabel  *lbl3;
    QLabel  *lbl4;

    //QLabel  *m_pLFileName;
    //QLabel  *m_pLModeTxt;

    QLabel  *m_pLModeImg;
    QLabel  *m_pLLocalThreatens;
    QLabel  *m_pLCloudThreatens;

    QPixmap *m_pIDropDown;

    int m_nThreads;
    int m_nMode;
    int m_nLocalThreatens;
    int m_nCloudThreatens;
    QString m_sFileName;

public:
        QLabel  *lbl2;
        ImgTxtLabel *m_pModeSelector;
private:
    bool mIntelligentMode;

};

#endif // SCANTOPBODYWIDGET_H
