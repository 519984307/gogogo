#ifndef _H_OFFLINEUPDATEINFODIALOG_H
#define _H_OFFLINEUPDATEINFODIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>


#include "common.h"

class QPushButton;
class QLabel;


class OfflineUpdateInfoDialog : public QDialog
{
    Q_OBJECT
public:
    OfflineUpdateInfoDialog(QWidget *parent = Q_NULLPTR);

public slots:
    void onClickedOK();
    void onUpdateOK();
    void onUpdateFail();
    void onUpdateWait();

protected:
    QPushButton* m_pbtnOK;
    QLabel * m_plInfo;
};

#endif // _H_OFFLINEUPDATEINFODIALOG_H
