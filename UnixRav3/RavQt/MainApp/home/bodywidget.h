#ifndef BODYWIDGET_H
#define BODYWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>

#include "topbodywidget.h"
#include "midbodywidget.h"
#include "botbodywidget.h"
#include "scantopbodywidget.h"
#include "scanmidbodywidget.h"
#include "scanbotbodywidget.h"
#include "stoppedtopbodywidget.h"
#include "mainwindow.h"

#include <string>

using namespace std;

class TopBodyWidget;
class MidBodyWidget;
class BotBodyWidget;
class ScanTopBodyWidget;
class ScanMidBodyWidget;
class ScanBotBodyWidget;
class StoppedTopBodyWidget;

class BodyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BodyWidget(QWidget *parent);
    ~BodyWidget();
    void SetMainWindowPtr(MainWindow *parent){
        m_pMainWindow = parent;
    }

    void SetVScanState(int nState);
    void AddVirusLog(const string &sVName, const string &sType, const string &sPath, const string &sStatus);
    void ResetVirusLog();
    void UpdateScanInfo(int nThreads, int nLocalThreaten, int nCloudThreaten, QString &sFile);
    void UpdateThreadInfo(int tag, int count, string &filename);
    void UpdateScaningThreadCount(int count);
    void UpdateStoppedInfo(int nThreatens, int nCount, int nUsingTime);

private:
    void setBackgroud(QString image);
    void SetNormalState();
    void SetScaningState();
    void SetStopState();

protected:
    void resizeEvent(QResizeEvent *event);


public:
    MainWindow *m_pMainWindow;

    TopBodyWidget   *m_pTopBody;
    MidBodyWidget   *m_pMidBody;
    QFrame  *m_pLine;
    BotBodyWidget   *m_pBotBody;

    ScanTopBodyWidget   *m_pScanTopBody;
    ScanMidBodyWidget   *m_pScanMidBody;
    ScanBotBodyWidget   *m_pScanBotBody;

    StoppedTopBodyWidget    *m_pStoppedTopBody;

    int m_nScanState;


};

#endif // BODYWIDGET_H
