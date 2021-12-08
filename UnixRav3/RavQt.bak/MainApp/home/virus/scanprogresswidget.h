#ifndef SCANPROGRESSWIDGET_H
#define SCANPROGRESSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>

#include "common.h"

class ShowButton;
class ScanPauseBtn;
class HImageButton;
class ScanProgressCircle;


class ScanProgressWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScanProgressWidget(QWidget *parent = 0);


private:
    void paintEvent(QPaintEvent *);

signals:

public slots:
};

class ScanNormalWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScanNormalWidget(QWidget *parent = 0);

public:
    ShowButton  *mNormalIcon;
    QLabel      *mLabel;
};

class ScanStoppedlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScanStoppedlWidget(QWidget *parent = 0);

    void UpdateInfo(int nThreatens);

private:
    ShowButton  *mStoppedIcon;
    QLabel      *mLabel;
};


class ScaningStateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScaningStateWidget(QWidget *parent = 0);

    void UpdateScanInfo(int nTotal, int nSpeed, int nThreatens, int nProccessed, int nUsingTimes);
    void UpdateScaningState(int nState);
    void UpdateScaningProgress(int progress);

public:
//    void resizeEvent(QResizeEvent *event);
    void UpdateInfo();
private:
    //ScanProgressWidget *mScaningIcon;

    //ShowButton  *m_pScaningIcon;
    ScanProgressCircle *m_Circle;
    QLabel      *m_pLabel;

    QPixmap *m_pPStaticText;
    QLabel *lbl1;
    QLabel *lbl2;
    QLabel *lbl3;
    QLabel *lbl4;
    QLabel *lbl5;

    QLabel   *m_pLTotal;
    QLabel   *m_pLSpeed;
    QLabel   *m_pLThreaten;
    QLabel   *m_pLProcessed;
    QLabel   *m_pLUsingTime;
    QProgressBar *progressbar;


public:
    int     m_nTotal;
    int     m_nSpeed;
    int     m_nThreaten;
    int     m_nProcessed;
    int     m_nUsingTime;

    ScanPauseBtn     *m_pPauseBtn;
    HImageButton    *m_pStopBtn;

    int     m_nScaningState;
    int     m_nPrevScaningState;
};

class ScanWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScanWidget(QWidget *parent = 0);

    void SetNormalState();
    void SetScaningState();
    void SetQuickScaningState();
    void SetCustomScaningState();
    void SetPauseScaningState();
    void SetReadyToStopState();
    void SetStoppedState();
    void SetReadyState();

    int ScanState(){return m_nVScanState;}

    void UpdateScanInfo(int nTotal, int nSpeed, int nThreatens, int nProccessed, int nUsingTimes);
    void UpdateScaningProgress(int progress);

private:
    void resizeEvent(QResizeEvent *event);


public:
    ScanNormalWidget    *mScanNormalWidget;
    ScaningStateWidget  *mScaningWidget;
    ScanStoppedlWidget  *mScanStoppedWidget;

    int m_nVScanState;

};

#endif // SCANPROGRESSWIDGET_H
