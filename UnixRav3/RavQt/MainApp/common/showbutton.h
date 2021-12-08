#ifndef SHOWBUTTON_H
#define SHOWBUTTON_H

#include <QPushButton>

class ShowButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ShowButton(QWidget *parent = 0);
    explicit ShowButton(int page, int index, QWidget *parent = 0);
    ~ShowButton();
    
    void setButtonState();

    void loadPixmap(QString path);
    void loadPixmap(QString pic_name, QSize size);

protected:
    void paintEvent(QPaintEvent *);

signals:
    void itemclicked(int, int);

public slots:
    void onClicked();

public:
    QPixmap mImage;
    int     mBtnWidth;
    int     mBtnHeight;

    int mPage;
    int mIndex;
};

#endif // SHOWBUTTON_H
