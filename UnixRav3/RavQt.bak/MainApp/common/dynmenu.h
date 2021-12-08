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

	void setUpdateTitle(const QString & title, bool showed=true);

signals:
	void signal_setUpdateTitle(const QString & title, bool showed);

public slots:
    void hoverMenu(QAction *pAction);
    void slot_setUpdateTitle(const QString& title, bool showed);

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
