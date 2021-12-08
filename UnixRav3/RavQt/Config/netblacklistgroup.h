#ifndef NETBLACKLISTGROUP_H
#define NETBLACKLISTGROUP_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QButtonGroup>
#include <QLineEdit>

#include "common/pubdef.h"

class CheckboxLabel;
class RadioButton;
class PushButton;
class BlacklistIPTable;
class BlacklistUrlTable;


class NetBlacklistGroup : public QGroupBox
{
    Q_OBJECT
public:
    NetBlacklistGroup(QWidget *parent = Q_NULLPTR);

    void SetFocus(bool bFocus);

public slots:
    void SetSizeEditValid();

    void OnClickedIpAdd();
    void OnClickedUrlAdd();

public:
    bool m_bFocus;

    CheckboxLabel   *m_pclStartBlacklist;

    PushButton      *m_ppbIpAdd;
    PushButton      *m_ppbUrlAdd;

    BlacklistIPTable    *m_pIPTable;
    BlacklistUrlTable   *m_pUrlTable;


};


#endif // NETBLACKLISTGROUP_H
