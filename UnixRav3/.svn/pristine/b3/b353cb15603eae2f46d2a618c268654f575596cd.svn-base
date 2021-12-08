#ifndef VIRUSMESSAGEDIALOG_H
#define VIRUSMESSAGEDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>


#include "pushbutton.h"
#include "imagebutton.h"
#include "checkboxlabel.h"


class ImageButton;
class PushButton;

class VMessageTitleWidget: public QWidget
{
    Q_OBJECT
public:
    VMessageTitleWidget(QWidget *parent = Q_NULLPTR);
    ~VMessageTitleWidget();

    void SetTitle(QString sTitle);

public:
    QLabel  *m_plTitle;


};


class VirusMessageDialog : public QDialog
{
    Q_OBJECT
public:
    VirusMessageDialog(QWidget *parent = Q_NULLPTR);
    ~VirusMessageDialog();

    void SetMessage(QString sTitle, QString sVName, QString sFile);
    int Value();
    bool SameAction();

public slots:
    void OnClickedKill();
    void OnClickedIgnore();

private:
    void InitDialog();



protected:
    void paintEvent(QPaintEvent *pPEvent);

public:

    VMessageTitleWidget * m_pHeader;

    QString m_sTitle;
    QString m_sVirus;
    QString m_sFile;
    QLabel *m_plWarning;
    QLabel *m_plIcon;
    QLabel * m_plVirus;
    QLabel * m_plFile;

    QPushButton *m_ppbKill;
    QPushButton *m_ppbIgnore;

    CheckboxLabel   *m_pclSameAction;

    int m_nRetValue;

    QVBoxLayout * vlayout;
    QHBoxLayout * h0layout;
    QHBoxLayout * h1layout;
    QHBoxLayout * h2layout;
    QHBoxLayout * h3layout;


};

#endif // VIRUSMESSAGEDIALOG_H
