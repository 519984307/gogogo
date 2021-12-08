#ifndef DIRSELPATHPROC_H
#define DIRSELPATHPROC_H


#include "dirselitem.h"


class DirSelItem;
class MainWindow;

class DirSelPathProc
{
public:
    explicit DirSelPathProc();
    ~DirSelPathProc();

    bool ContainedChildPath(map<int, DirSelItem*> &mapItems, QString &sFile);
    DirSelItem * GetChildDirSelItem(map<int, DirSelItem*> &mapItems, QString &sFile);

    int InitTreeItems(int &nID, bool bSelected, map<int, DirSelItem*> &mapItems, QString sPath, int nDepth);


public:
    MainWindow * m_pMainWindow;
};

#endif // DIRSELPATHPROC_H
