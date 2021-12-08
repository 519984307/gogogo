#ifndef DYNMENU_H
#define DYNMENU_H

#include <QMenu>
#include <QObject>
#include <QWidget>
#include <QAction>

class DynMenu : public QMenu
{
     Q_OBJECT
public:
    explicit DynMenu(bool isSecureMachine);

    void LoadMenu(bool isSecureMachine=false);
    int loadActionPixmap(QAction *pAction, QString pic_name, int nIndex);
    void HideNonAdminMenu();

    void setUpdateTitle(const QString& title, bool showed=true);

public slots:
    void hoverMenu(QAction *pAction);

protected:
    void leaveEvent(QEvent *pEvent);
public:
    QAction * logAction;
    QAction * isolateAction;
    QAction * updateAction;
    QAction * localUpdateAction;
    QAction * aboutAction;

    QPixmap *m_ppImage;

    bool m_bIsSecureMachine;
};

#endif // DYNMENU_H
