#ifndef CHECKBOXLABEL_H
#define CHECKBOXLABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPixmap>

class CheckboxLabel : public QLabel
{
    Q_OBJECT
public:
    CheckboxLabel(QWidget *parent, QString sRightText);

    void LoadImage();
    bool IsChecked(){return m_bChecked;}
    void SetValue(bool bChecked)
    {
        m_bChecked = bChecked;
        update();
    }

signals:
    void clicked();
protected:
    void enterEvent(QEvent *pEvent);
    void leaveEvent(QEvent *pEvent);
//    void mousePressEvent(QMouseEvent *pEvent);
    void mouseReleaseEvent(QMouseEvent *pEvent);
    void paintEvent(QPaintEvent *pEvent);

public:
    bool    m_bChecked;
    bool    m_bHoveredCheckbox;

    int m_nWidth;
    int m_nHeight;

    QPixmap *m_ppCheckBox;
    QString m_sRightText;

};

#endif // CHECKBOXIMAGELABEL_H
