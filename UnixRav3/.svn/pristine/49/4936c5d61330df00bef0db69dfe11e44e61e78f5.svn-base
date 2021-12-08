#ifndef DIRSELITEM_H
#define DIRSELITEM_H

#include <QString>
#include <map>

using namespace std;


class DirSelItem
{
public:
    DirSelItem(int nID, const QString &sAbsPath, const QString &sPath, bool bSelected=false);
    ~DirSelItem();

    void AddChild(int nID, const QString &sAbsPath, const QString &psPath, bool bSelected);
    void SetSelected();
    void SetUnSelect();
    bool HasItemID(int nID);
    bool HasChild();

public:
    int m_nID;
    QString m_sAbsPath;
    QString  m_sPath;
    bool m_bSelected;

    map<int, DirSelItem*>  m_mapChild;

};

#endif // DIRSELITEM_H
