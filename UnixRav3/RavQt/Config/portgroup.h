#ifndef PORTGROUP_H
#define PORTGROUP_H

#include "common/pushbutton.h"
#include "common/ipwhitetable.h"
#include "common/ipblacktable.h"
#include "common/porttable.h"

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QMenu>
#include <QAction>

class PushButton;


class PortGroup : public QGroupBox
{
    Q_OBJECT
public:
    PortGroup(int flag = 0, QWidget *parent = Q_NULLPTR);

	void InitWidget(QString);
    void SetFocus(bool bFocus);

public slots:
    void AddClicked();

public:
    bool m_bFocus;
	bool firewallflag;

    PushButton  *m_pbExtName;
    PushButton  *m_ppbDirFile;

    IpBlackNameTable    *m_IpBlackName;
    IpWhiteNameTable    *m_IpWhiteName;
    PortNameTable    *m_PortName;

private:
	int Wflag = 0;

};

#endif // PORTGROUP_H
