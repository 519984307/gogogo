#ifndef POPSCREEN_H
#define POPSCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QString>
#include <QLabel>
#include <QBoxLayout>
#include <QCheckBox>

const static char *complete_str = "扫描完成!;未发现病毒";
const static char *pecent_beg_str = "正在扫描;进度:";
const static char *pecent_end_str="%;路径";

class ColorWidget : public QWidget {
    Q_OBJECT
public:
    ColorWidget(QWidget *parent=NULL) : QWidget(parent) {

    }

protected:
    void paintEvent(QPaintEvent *event);

public:
    void SetBackgroundColor(const QColor& color) {
        mBackgroundColor = color;
    }

private:
    QColor mBackgroundColor;
};

class ImageLabel : public QLabel {
    Q_OBJECT
public:
    ImageLabel(const QString& image, QWidget *parent=NULL);
    void setImage(const QString& image);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap mPixmap;
};


class PushButton;
class PopScreen : public QWidget
{
    Q_OBJECT
public:
    PopScreen(QWidget *parent = 0);
    ~PopScreen();

    void showMessage(int type, const char* title, int icon, const char *message, const char *left, const char *right);
    int getMsgNum(const std::string);

private slots:
    void beClose(const char *msg = NULL);

    void onMove();
    void onStay();
    void onClose();
    void onExit();
    void onLeftClicked();
    void onRightClicked();

signals:
    void end(int selected);

private:
    QWidget* initHead(QWidget *parent=NULL);
    QWidget* initTitle(QWidget *parent=NULL);
    QWidget* initPrompt(QWidget *parent=NULL);

    QWidget* initBody(QWidget *parent=NULL);
    QWidget* initContent(QWidget *parent=NULL);
    QWidget* initFoot(QWidget *parent=NULL);

    QLabel* initLabel(int fontSize, const QColor& fontColor);
    void UpdateScreen(int type, const char* title, int icon, const char *message, const char *left, const char *right);

    void showMessageType(bool immediatly);

private:
    PushButton *closeButton;

    PushButton *confirmButton;
    PushButton *ignoreButton;

    QLabel  *titleLabel;
    QLabel  *contentLabel;
    ImageLabel  *iconLabel;
    QLabel  *tipLabel;

    QLabel *mContentLabel[3];

    QCheckBox *mDefaultSet;

    QTimer *showTimer;
    QTimer *stayTimer;
    QTimer *closeTimer;
    QPoint point;

    double transparentPercent;
    int desktopHeight;

private:
    QString mLeftButtonTitle;
    QString mRightButtonTitle;

    bool mPopState;

    int  mPopFlag;
    int  mPopType;

    int mSelected;

    bool bClose;
    int iPreNum;

};

#endif // POPSCREEN_H
