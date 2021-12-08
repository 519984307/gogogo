#ifndef SCANBODYWIDGET_H
#define SCANBODYWIDGET_H

#include <QWidget>
#include <QFrame>


class ScanTopBodyWidget;
class ScanMidBodyWidget;
class ScanBotBodyWidget;

class ScanBodyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScanBodyWidget(QWidget *parent = 0);
    ~ScanBodyWidget();

private:
    void setBackgroud(QString image);

protected:
    void resizeEvent(QResizeEvent *event);

signals:

public slots:

public:
    ScanTopBodyWidget   *m_pTopBody;
    ScanMidBodyWidget   *m_pMidBody;
    QFrame  *m_pLine;
    ScanBotBodyWidget   *m_pBotBody;


};

#endif // SCANBODYWIDGET_H
