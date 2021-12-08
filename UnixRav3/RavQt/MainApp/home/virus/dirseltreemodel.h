#ifndef DIRSELTREEMODEL_H
#define DIRSELTREEMODEL_H

#include <QObject>
#include <QWidget>
#include <QStandardItemModel>

class DirSelectMidBodyWidget;



class DirSelTreeModel : public QStandardItemModel
{
friend class DirSelectMidBodyWidget;
public:
    DirSelTreeModel(QObject *parent = Q_NULLPTR);


    bool hasChildren(const QModelIndex &parent = QModelIndex()) const;


public slots:


};

#endif // DIRSELTREEMODEL_H
