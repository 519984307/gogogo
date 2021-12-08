#ifndef ConfirmRestoreWidget_H
#define ConfirmRestoreWidget_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>


#include "pushbutton.h"
#include "common/imagebutton.h"
#include "checkboxlabel.h"


class ImageButton;
class PushButton;

class ConfirmTitleWidget: public QWidget
{
    Q_OBJECT
public:
    ConfirmTitleWidget(QWidget *parent = Q_NULLPTR);
    ~ConfirmTitleWidget();

    void SetTitle(QString sTitle);

public:
    QLabel  *m_plTitle;


};


class ConfirmRestoreWidget : public QDialog
{
    Q_OBJECT
public:
    ConfirmRestoreWidget(QWidget *parent = Q_NULLPTR);
    ~ConfirmRestoreWidget();

    void SetMessage(QString sTitle, QString sVName, QString sFile);
    int Value();
    bool IfAddWhiteList();

public slots:
    void OnClickedOK();
    void OnClickedIgCancle();

private:
    void InitDialog();



protected:
    void paintEvent(QPaintEvent *pPEvent);

public:

    ConfirmTitleWidget * m_pHeader;

    QString m_sTitle;
    QString m_sQueryInfo;
    QLabel *m_plWarning;

    QPushButton *m_ppbOK;
    QPushButton *m_ppbCancle;

    CheckboxLabel   *m_pclSameAction;

    int m_nRetValue;

    QVBoxLayout * vlayout;
    QHBoxLayout * h0layout;
    QHBoxLayout * h1layout;
    QHBoxLayout * h2layout;
    QHBoxLayout * h3layout;


};

#endif // ConfirmRestoreWidget_H
