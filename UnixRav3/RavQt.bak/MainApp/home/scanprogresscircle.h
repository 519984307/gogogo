#ifndef SCANPROGRESSCIRCLE_H
#define SCANPROGRESSCIRCLE_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QTimer>
#include <pthread.h>
#include "common.h"



class ScanProgressCircle : public QWidget
{
    Q_OBJECT
public:
    enum CircleType{
        ALL_KILL = 0,
        FAST_KILL,
        CUSTOM_KILL,
        ADMIN_KILL
    };
    enum WarnLevel{
        NORMAL = 0,
        WARNING,
        CRITICLE
    };

    explicit ScanProgressCircle(QWidget *parent = nullptr, CircleType type = ALL_KILL);
    ~ScanProgressCircle();

    void StartMotion();
    void StopMotion();
    void Reset();
    CircleType GetCircleType(){
        return m_ctType;
    }
    WarnLevel GetWarnLevel(){
        return m_wlLevel;
    }
    void SetCircleType(CircleType type);
    void SetWarnLevel(WarnLevel level);
    void SetProgress(int progress); //0~100

protected:
    void paintEvent(QPaintEvent *pEvent);

    void Lock();
    void UnLock();
signals:

public slots:
    void timerEvent();

private:
    QPixmap *m_ppmCenter;
    QPixmap *m_ppmMotion;
    //QLabel  *m_pLabel;

    QTimer *m_pTimer;
    int m_nProgress;
    int m_nDegree;   //0~360

    CircleType  m_ctType;
    WarnLevel   m_wlLevel;
    pthread_mutex_t   m_tLock;
};

#endif // SCANPROGRESSCIRCLE_H
