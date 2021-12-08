#ifndef ScanChart_H
#define ScanChart_H

#include <QObject>
#include <QWidget>
#include <QMutex>

#include <vector>
#include <QList>
#include "common.h"


//QT_CHARTS_USE_NAMESPACE
using namespace std;

class QLabel;

//size 205*47
class CurvePainter : public QWidget
{
    Q_OBJECT
public:
    explicit CurvePainter(int ymax = 100, QWidget *parent = nullptr);
    ~CurvePainter();

    void UpdateData(int data);
    void Reset();

protected:
    void paintEvent(QPaintEvent *event);

protected:
    QList<QPointF>  lstPointF;

    int nTopSpace;     //on the top of the curve yMax+nTopSpace+nBottomSpace=height()
    int nBottomSpace;
    double xFactor;//, xPrevFactor;  //20  for value=value-20
    double yFactor, yPrevFactor;  //value=yDataMax/(height()-nTopSpace) for y-positon transform
    int yDataMax;
    QMutex mutex;
};

class ScanChart : public QWidget
{
    Q_OBJECT
public:
    explicit ScanChart(int id, QWidget *parent = nullptr);
    ~ScanChart();

    void Start();
    void Stop();
    void Reset();
    bool IsStopped(){
        return !m_bRunning;
    }

    void UpdateData(int count, const string &filename);


protected:
    void InitChart();
    int SimpleFileName(const string &filename, string &outStr, int maxLen = 17);
    void paintEvent(QPaintEvent *event);

signals:

public slots:

protected:
    int     m_nID;
    QLabel      * m_plTitleCount;
    QLabel      *m_plTitleFileName;

    CurvePainter *m_pcpPainter;

    int     m_nDataMax;
    int     m_nFileCount;

    bool    m_bRunning;
};

#endif // ScanChart_H
