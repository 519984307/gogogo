#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTreeView>
#include <QResizeEvent>
#include <QVector>

#include "common/imgtxtlabel.h"
#include "common/pubdef.h"
#include "common/globalconf.h"
#include "mainwindow.h"

class ImgTxtLabel;
class GlobalConf;


typedef enum IMG_TXT_LABEL_TYPE{
    IMG_TXT_LABEL_TYPE_KILL = 0,
    IMG_TXT_LABEL_TYPE_NETDEFEND,
    IMG_TXT_LABEL_TYPE_SETUP,
    IMG_TXT_LABEL_TYPE_ORI,
    IMG_TXT_LABEL_TYPE_WHITELIST,
    IMG_TXT_LABEL_TYPE_BACKUP,
    IMG_TXT_LABEL_TYPE_KV,
    IMG_TXT_LABEL_TYPE_TIMER,
    IMG_TXT_LABEL_TYPE_FILE,
    IMG_TXT_LABEL_TYPE_UDISK,
    //IMG_TXT_LABEL_TYPE_SYSTEM,
    //IMG_TXT_LABEL_TYPE_APP,
    //IMG_TXT_LABEL_TYPE_NETBLACKLIST,
    //IMG_TXT_LABEL_TYPE_ADMIN,
//    IMG_TXT_LABEL_TYPE_TRAY,
//    IMG_TXT_LABEL_TYPE_LICENSE,
    IMG_TXT_LABEL_TYPE_UPGRADE,
    IMG_TXT_LABEL_TYPE_DISCOVERYCENTER
} ImgTxtLabelType;

struct stru_table_text{
    ImgTxtLabelType type;
    QString text;
};


class LeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftWidget(QWidget *parent = nullptr, int nFunction = 0x3F, GlobalConf *pConf = NULL, int nHeight = APP_SCREEN_HEIGHT);

    void InitTree();

    void OnClickLabel(ImgTxtLabel *pLabel);
    ImgTxtLabel *TopSelectedLabel();
//    ImgTxtLabel *MidSelectedLabel();
    ImgTxtLabel *EndSelectLabel();

    void SetTopDisable();
//    void SetMidDisable();
    void SetEndDisable();

    bool HideAdvanceFunction();

protected:
    void paintEvent(QPaintEvent *pEvent);

signals:

public slots:

public:
    MainWindow  *m_pMainWindow;

    ImgTxtLabel *m_pitlKill;
    //ImgTxtLabel *m_pitlNetDefend;
    ImgTxtLabel *m_pitlSetup;

    ImgTxtLabel *m_pitlOri;
    ImgTxtLabel *m_pitlWhiteList;
    ImgTxtLabel *m_pitlBackup;
    ImgTxtLabel *m_pitlKV;
    ImgTxtLabel *m_pitlTimer;
    ImgTxtLabel *m_pitlFile;
    ImgTxtLabel *m_pitlUDisk;
    //ImgTxtLabel *m_pitlSystem;
    //ImgTxtLabel *m_pitlApp;

//    ImgTxtLabel *m_pitlNetBlacklist;

    //ImgTxtLabel *m_pitlAdmin;
//    ImgTxtLabel *m_pitlTray;
//    ImgTxtLabel *m_pitlLicense;
    ImgTxtLabel *m_pitlUpgrade;
    ImgTxtLabel *m_pitlCenterDiscovery;

    QVector<ImgTxtLabel*>  m_TopLabels;
    QVector<ImgTxtLabel*>  m_EndLabels;

    GlobalConf  *m_pConf;
    int m_nFunction;
};

#endif // LEFTWIDGET_H
