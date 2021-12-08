#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class ShowButton;
class PushButton;
class MoreStateTextButton;
class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = 0);
    ~TitleWidget();
    void UpdateTitle(const char *majorTitle, const char *minorTitle);


signals:
    //void showMin();
    void closeWidget();
    void changeCenter(int);
    void showConnectMenu();
    void logoClicked();

private:
    QHBoxLayout* initToolArea();

protected:
    void resizeEvent(QResizeEvent *event);


public:
    ShowButton  *mLogo;
    //ShowButton  *mTitle;
    QLabel *mTitleMajor;
    QLabel *mTitleMinor;
    //MoreStateTextButton *mOrganization;

    PushButton  *mSettingBtn;
    PushButton  *mMenuBtn;
    //PushButton  *mMiniBtn;
    PushButton  *mCloseBtn;
};

#endif // TITLEWIDGET_H
