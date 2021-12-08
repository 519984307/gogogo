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

class ImgTxtLabel;
class GlobalConf;


typedef enum IMG_TXT_LABEL_TYPE{
    IMG_TXT_LABEL_TYPE_KILL = 0,
    IMG_TXT_LABEL_TYPE_VIRUS_DETAIL,
    IMG_TXT_LABEL_TYPE_SCAN_EVENT,
    IMG_TXT_LABEL_TYPE_ISOLATION,
    IMG_TXT_LABEL_TYPE_NET_DEFENCE,
    IMG_TXT_LABEL_TYPE_URL_ACCESS,
    IMG_TXT_LABEL_TYPE_IP_RULE,
    IMG_TXT_LABEL_TYPE_BASIC_LOG,
    IMG_TXT_LABEL_TYPE_INSTALLATION,
    IMG_TXT_LABEL_TYPE_PLATFORM_LOG
} ImgTxtLabelType;

struct stru_table_text{
    ImgTxtLabelType type;
    QString text;
};


class LeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftWidget(QWidget *parent = nullptr, GlobalConf *pConf = NULL, int nFunctions = 0x3F, bool centerless=false);

    void InitTree(bool centerless);

    void OnClickLabel(ImgTxtLabel *pLabel);
    ImgTxtLabel *TopSelectedLabel();
    ImgTxtLabel *MidSelectedLabel();
    ImgTxtLabel *EndSelectLabel();

    void SetTopDisable();
    void SetMidDisable();
    void SetEndDisable();


protected:
    void paintEvent(QPaintEvent *pEvent);

signals:

public slots:

public:
    ImgTxtLabel *m_pitlKill;
    ImgTxtLabel *m_pitlNetDefence;
    ImgTxtLabel *m_pitlBasicLog;

    ImgTxtLabel *m_pitlVirusDetail;
    ImgTxtLabel *m_pitlScanEvent;
//    ImgTxtLabel *m_pitlAppReinforce;
    ImgTxtLabel *m_pitlIsolation;

    ImgTxtLabel *m_pitlUrlAccess;
    ImgTxtLabel *m_pitlIPRule;

    ImgTxtLabel *m_pitlInstallation;
    ImgTxtLabel *m_pitlPlatformLog;


    QVector<ImgTxtLabel*>  m_TopLabels;
    QVector<ImgTxtLabel*>  m_MidLabels;
    QVector<ImgTxtLabel*>  m_EndLabels;

    GlobalConf  *m_pConf;
    int m_nFunctions;

};

#endif // LEFTWIDGET_H
