#ifndef SCANMIDBODYWIDGET_H
#define SCANMIDBODYWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPixmap>

#include "common.h"


#include <string>

using namespace std;

class ImageButton;
class PushButton;
class ScanResultTable;
class ScanChart;


class ScanMidBodyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScanMidBodyWidget(QWidget *parent = nullptr);
    ~ScanMidBodyWidget();

    void SetStoppedState();
    void SetScaningState();
    void UpdateThreadInfo(int tag, int count, string &filename);
    void UpdateScaningThreadCount(int count);

    void AddVirusLog(const string &sVName, const string &sType, const string &sPath, const string &sStatus);
    void ResetVirusLog();

signals:

public slots:


protected:


private:

public:
    ScanResultTable *m_pTable;
    ImageButton  *m_pBtnQuit;

    ScanChart   *m_pChart1;
    ScanChart   *m_pChart2;
    ScanChart   *m_pChart3;
    ScanChart   *m_pChart4;

    int mScanThreadCount;
};

#endif // SCANMIDBODYWIDGET_H
