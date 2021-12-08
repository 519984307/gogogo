#ifndef CHECKBOXIMAGELABEL_H
#define CHECKBOXIMAGELABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPixmap>

class CheckboxImageLabel : public QLabel
{
    Q_OBJECT
public:
    CheckboxImageLabel(QWidget *parent, QString sRightText);

    void LoadImage(QString sRightText);

    bool IsChecked(){
        return m_bChecked;
    }

    void SetChecked(bool bChecked){
        m_bChecked = bChecked;
        update();
    }

protected:
    void enterEvent(QEvent *pEvent);
    void leaveEvent(QEvent *pEvent);
//    void mousePressEvent(QMouseEvent *pEvent);
    void mouseReleaseEvent(QMouseEvent *pEvent);
    void paintEvent(QPaintEvent *pEvent);

signals:
    void clicked();

public:
    bool    m_bChecked;
    bool    m_bHoveredCheckbox;

    int m_nWidth;
    int m_nHeight;

    QPixmap *m_ppCheckBox;
    QPixmap *m_ppRightText;

};

#endif // CHECKBOXIMAGELABEL_H
