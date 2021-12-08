#ifndef WHITELISTGROUP_H
#define WHITELISTGROUP_H

#include "common/pushbutton.h"
#include "common/dirfiletable.h"
#include "common/extnametable.h"
#include "common/dirselectdialog.h"

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QMenu>
#include <QAction>

class PushButton;
class DirFileTable;
class ExtNameTable;
class DirSelectDialog;


class WhiteListGroup : public QGroupBox
{
    Q_OBJECT
public:
    WhiteListGroup(QWidget *parent = Q_NULLPTR);

    void InitAllWidget();

    void MenuDirFChildClicked();
    void MenuDirClicked();
    void MenuChildClicked();
    void MenuFileClicked();
    void SetFocus(bool bFocus);

public slots:
    void PopDirFileMenu();
    void TrigerDirFileMenu(QAction *pAction);
    void AddExtNameClicked();

public:
    bool m_bFocus;

    PushButton  *m_ppbDirFile;
    PushButton  *m_pbExtName;

    DirFileTable    *m_pdfDirFile;
    ExtNameTable    *m_penExtName;

    QMenu   *m_pMenu;
    QAction *m_paDirAndChild;
    QAction *m_paDir;
    QAction *m_paChild;
    QAction *m_paFile;
};

#endif // WHITELISTGROUP_H
