#ifndef FILEDIRBUTTON_H
#define FILEDIRBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>


class FileDirButton : public QPushButton
{
    Q_OBJECT
public:
    FileDirButton(QWidget *parent = 0);

signals:

public slots:

public:
    void loadPixmap(QString sImgName);


protected:
    void enterEvent(QEvent *pEvent);
    void leaveEvent(QEvent *pEvent);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *pEvent);

protected:
    //枚举按钮的几种状态
    enum ButtonStatus{NORMAL=0, ENTER, PRESS, NOSTATUS};

    ButtonStatus mStatus;
    QPixmap      mPixmap;

    int m_nBtnWidth;
    int m_nBtnHeight;
    bool m_bMousePress;
};

#endif // FILEDIRBUTTON_H
