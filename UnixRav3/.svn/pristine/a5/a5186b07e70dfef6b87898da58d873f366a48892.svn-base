#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QPixmap>
#include <QLabel>

#include "pushbutton.h"
#include "imagebutton.h"
#include "licensebutton.h"



class ImageButton;
class PushButton;
class LicenseButton;

// TODO 在此处引入父类，是极其失败的设计，后期改进
class MainWindow;

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    AboutDialog(QWidget *parent = Q_NULLPTR);
    void SetProductName(const char* sMajorName, const char* sMinorName);
    void SetVersion(QString &sVersion, QString &sUpdatedTime, QString &sVVersion, QString &sVDBUpdatedTime);
    void SetVersion(QString &sVersion, QString &sUpdatedTime, QString &sVVersion, QString &sVDBUpdatedTime, QString &serial);

public slots:
    void LicenseClicked();


private:
    void LoadPixmap(QString sBackgroundImage);
    void InitDialog();

protected:
    void paintEvent(QPaintEvent *pPEvent);

public:
    int m_nWidth;
    int m_nHeight;

    QString m_sMajorName;
    QString m_sMinorName;

    QString m_sVersion;
    QString m_sUpdatedTime;
    QString m_sVirusVersion;
    QString m_sVDBUpdatedTime;
    QString mSerialNumber;

    QPixmap *m_ppBackgroundImage;

    QLabel *m_plProductName;
    QLabel *m_plProductMinorName;

    QLabel *mSerialNumberLabel;

    PushButton  *m_ppbClose;
    ImageButton *m_pibnOK;
    LicenseButton   *m_pLicenseBtn;

    MainWindow *mParent;

};

#endif // ABOUTDIALOG_H
