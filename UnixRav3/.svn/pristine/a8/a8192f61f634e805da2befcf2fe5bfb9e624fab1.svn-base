#ifndef DIRSELECTDIALOG_H
#define DIRSELECTDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QTreeView>
#include <QPushButton>
#include <QDirModel>
#include <QStringList>
#include <QMouseEvent>
#include <QLabel>
#include <QToolButton>

#include "common/pubdef.h"


class DirSelectDialog : public QDialog
{
    Q_OBJECT
public:
    DirSelectDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    void InitWidget();


protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

public slots:
    void ClickedTree(QModelIndex);

private:
    bool m_bIsMove;
    QPoint  m_PressPoint;

public:
    int m_nType;
    QString m_sName;

    QLabel *m_pTitle;

    QTreeView *m_pTreeView;
    QDirModel    *m_pDirModel;
    QStringList *m_pDirList;

    QLineEdit   *m_pEdit;

    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QToolButton *closeBtn;

};

#endif // DIRSELECTDIALOG_H
