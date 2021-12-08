#ifndef BOTBODYWIDGET_H
#define BOTBODYWIDGET_H

#include <QObject>
#include <QWidget>
#include <QDateTime>
#include <QLabel>
#include <QPixmap>
#include <string>

#include "common.h"
#include "checkboximagelabel.h"
#include "checkupdatepushbutton.h"
#include "mainwindow.h"

class QDatetime;
class QPixmap;
class CheckboxImageLabel;

using namespace std;

class BotBodyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BotBodyWidget(QWidget *parent, MainWindow *pMainWindow);

    void LoadPixmap();
    void LoadStatusWidgets();

    void SetVersion(QString sVersion);
    void SetUpdateTime(QString sTime);
//    bool GetAutoProcVirus();
//    bool GetShutdownAfterScan();

protected:
    void paintEvent(QPaintEvent *pPEvent);

signals:

public slots:


private:
    MainWindow *m_pMainWindow;

    QString m_sVersion;
    QString   m_sUpdateTime;

    //bool    m_bAutoProcVirus;
    //bool    m_bShutdownAfterScan;

    QPixmap *m_ppVersion;
    QPixmap *m_ppUpdateTime;
//    QPixmap *m_ppCheckUpdateTxt;
    QLabel  *m_plVersionImg;
    QLabel  *m_plVersion;
    QLabel  *m_plUpdatetimeImg;
    QLabel  *m_plUpdatetime;

public:
    CheckUpdatePushButton  *m_cupbCheckUpdate; //offline update

    //CheckboxImageLabel  *m_pcilAutoProcVirus;
    //CheckboxImageLabel  *m_pcilShutdownAfterScan;

};

#endif // BOTBODYWIDGET_H
