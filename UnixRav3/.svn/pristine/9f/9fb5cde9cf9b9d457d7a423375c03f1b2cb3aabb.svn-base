#ifndef MODEMENU_H
#define MODEMENU_H

#include <QMenu>
#include <QObject>
#include <QWidget>
#include <QAction>

class ModeMenu : public QMenu
{
     Q_OBJECT
public:
    explicit ModeMenu();

    void LoadMenu();
    int loadActionPixmap(QAction *pAction, QString pic_name, int nIndex);
public slots:
    void hoverMenu(QAction *pAction);

protected:
    void leaveEvent(QEvent *pEvent);
public:
    QAction * autoAction;
    QAction * officeAction;
    QAction * highspeedAction;

    QPixmap *m_ppImage;

};

#endif // MODEMENU_H
