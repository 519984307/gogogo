/**
 * 这个类是点击按钮的类，用于设计按钮的状态
 *
 * by pangxiaoliang1981@gmail.com
 */

#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>

class PushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(QWidget *parent = 0);
    ~PushButton();

signals:

public slots:

public:
    /** 加载图片 */
    void loadPixmap(const QString& path);
    void setText(const QString &text, const QColor& normal, const QColor& enter,const QColor& press,const QColor& nostatus) {
        mText = text;
        mTextColor[0] = normal;
        mTextColor[1] = enter;
        mTextColor[2] = press;
        mTextColor[3] = nostatus;
    }

protected:
    /**
    * 功能说明 鼠标移进事件
    */
    void enterEvent(QEvent *);

    /**
    * 功能说明 鼠标移出事件
    */
    void leaveEvent(QEvent *);

    /**
    * 功能说明 鼠标按下事件
    */
    void mousePressEvent(QMouseEvent *event);

    /**
    * 功能说明 鼠标释放事件
    */
    void mouseReleaseEvent(QMouseEvent *event);

    /**
    * 功能说明 绘图事件
    */
    void paintEvent(QPaintEvent *);

protected:
    //枚举按钮的几种状态
    enum ButtonStatus{NORMAL=0, ENTER, PRESS, NOSTATUS};

    ButtonStatus mStatus;
    QPixmap      mPixmap;

    int mBtnWidth;      //按钮宽度
    int mBtnHeight;     //按钮高度
    bool mMousePress;   //按钮左键是否按下

    QString mText;
    QColor  mTextColor[4];
};

class IconTextButton : public PushButton
{
public:
    explicit IconTextButton(QString icon, QString txt, QWidget *parent = 0);
    ~IconTextButton();

signals:

public slots:

protected:
    /**
    * 功能说明 绘图事件
    */
    void paintEvent(QPaintEvent *);
};


class MoreStateTextButton :public PushButton
{
public:
    void setTitle(QString title) {
        mTitle = title;
    }

protected:
    void paintEvent(QPaintEvent *);

private:
    QString mTitle;
};

#endif // PUSHBUTTON_H
