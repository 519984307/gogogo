#ifndef DIRSELECTDIALOG_H
#define DIRSELECTDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>

#include "common.h"

#include "dirselectheaderwidget.h"
#include "dirselectmidbodywidget.h"
#include "dirselectbotbodywidget.h"


class DirSelectHeaderWidget;
class DirSelectMidBodyWidget;
class DirSelectBotBodyWidget;
class MainWindow;


class DirSelectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DirSelectDialog(QWidget *parent = nullptr);


protected:
    void paintEvent(QPaintEvent *pPEvent);

signals:

public slots:
    void CancelDialog();
    void CloseDialog();

public:
    //QLabel  *m_pLabel;

    DirSelectHeaderWidget   *m_pHeader;
    DirSelectMidBodyWidget  *m_pMidBody;
    DirSelectBotBodyWidget  *m_pBotBody;

    QStringList m_lDirs;

    MainWindow *m_pMainWindow;

};

#endif // DIRSELECTDIALOG_H
