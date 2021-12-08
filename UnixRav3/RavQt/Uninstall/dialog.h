#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class QPushButton;
class QLineEdit;
class GuiUninstaller;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    void OnClickedOK();


protected:
    void changeEvent(QEvent *e);

public:
    bool m_bVerifyPasswdOK;

private:
    Ui::Dialog *ui;
    QPushButton * m_pbtnOK;
    QLineEdit * m_plePasswd;
    GuiUninstaller * m_pUninstaller;
};

#endif // DIALOG_H
