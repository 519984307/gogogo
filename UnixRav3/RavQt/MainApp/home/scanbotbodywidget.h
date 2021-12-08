#ifndef SCANBOTBODYWIDGET_H
#define SCANBOTBODYWIDGET_H

#include <QObject>
#include <QWidget>
#include <QDateTime>
#include <QLabel>
#include <QPixmap>

#include "common.h"
#include "checkboximagelabel.h"
#include "checkupdatepushbutton.h"


class QDatetime;
class QPixmap;
class CheckboxImageLabel;


class ScanBotBodyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScanBotBodyWidget(QWidget *parent = nullptr);

    void LoadPixmap();
    void LoadStatusWidgets();

    void SetVersion(QString sVersion);
    void SetUpdateTime(QString sTime);
    bool GetAutoProcVirus();
    bool GetShutdownAfterScan();

protected:
    void paintEvent(QPaintEvent *pPEvent);

signals:

public slots:
    void ClickUpdate(bool bChecked);

private:
    QString m_sVersion;
    QString   m_sUpdateTime;

    //bool    m_bAutoProcVirus;
    //bool    m_bShutdownAfterScan;

    QPixmap *m_ppVersion;
    QPixmap *m_ppUpdateTime;
    //QPixmap *m_ppCheckUpdateTxt;
    QLabel  *m_plVersion;
    QLabel  *m_plUpdatetime;

//    CheckUpdatePushButton  *m_cupbCheckUpdate;

//    CheckboxImageLabel  *m_pcilAutoProcVirus;
//    CheckboxImageLabel  *m_pcilShutdownAfterScan;

};

#endif // SCANBOTBODYWIDGET_H
