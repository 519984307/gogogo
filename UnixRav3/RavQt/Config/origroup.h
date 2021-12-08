#ifndef ORIGROUP_H
#define ORIGROUP_H

#include <QGroupBox>
#include <QCheckBox>

#include "common/pubdef.h"
#include "common/checkboxlabel.h"
#include "common/pushbutton.h"
#include "common/privatecloudtable.h"


class CheckboxLabel;

class MainWindow;
class OriGroup : public QGroupBox
{
    Q_OBJECT
public:
    OriGroup(QWidget *parent, MainWindow *pMainWindow);

    void SetFocus(bool bFocus);

protected slots:
    void OnClickedAdd();

protected:
    //void paintEvent(QPaintEvent *);

public:
    MainWindow  *m_pMainWindow;

    bool m_bFocus;

    CheckboxLabel   *m_pRunEnv;
    CheckboxLabel   *m_pVirusTrace;
    CheckboxLabel   *m_pVirusLog;
    CheckboxLabel   *m_pBuffer;

    CheckboxLabel   *m_pCloud;
    PushButton      *m_ppbAdd;
    PrivateCloudTable * m_pPrivateTable;

    QLineEdit *m_CenterAddr;
};

#endif // ORIGROUP_H
