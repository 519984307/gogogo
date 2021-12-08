#include "dirselpathproc.h"
#include "mainwindow.h"

#include <QDir>
#include <string>
#include <map>
#include <vector>

using namespace std;


DirSelPathProc::DirSelPathProc()
{

}

DirSelPathProc::~DirSelPathProc()
{

}

bool DirSelPathProc::ContainedChildPath(map<int, DirSelItem*> &mapItems, QString &sFile)
{
    if(mapItems.size() == 0)
        return false;
    map<int, DirSelItem*>::iterator it = mapItems.begin();
    while(it != mapItems.end()){
        DirSelItem *pdsi = it->second;
        if(pdsi->m_sPath == sFile)
            return true;
        it++;
    }
    return false;
}


DirSelItem * DirSelPathProc::GetChildDirSelItem(map<int, DirSelItem*> &mapItems, QString &sFile)
{
    if(mapItems.size() == 0)
        return NULL;
    map<int, DirSelItem*>::iterator it = mapItems.begin();
    while(it != mapItems.end()){
        DirSelItem *pdsi = it->second;
        if(pdsi->m_sPath == sFile)
            return pdsi;
        it++;
    }
    return NULL;
}

int DirSelPathProc::InitTreeItems(int &nID, bool bSelected, map<int, DirSelItem*> &mapItems, QString sPath, int nDepth)
{
    if(nDepth == 0)
        return 0;
    int nLevel = nDepth;
    map<string, vector<string> > infos;
    QStringList list;
    if (m_pMainWindow->IsSecretMachine()) {

        if (m_pMainWindow->QueryPath(sPath.toStdString(), infos) != 0) {
            return 0;
        }
        if (infos.size() == 0) {
            return 0;
        }

        map<string, vector<string> >::iterator itv = infos.begin();
        while(itv != infos.end()){
            string tpath = itv->first;
            list.append(QString::fromStdString(tpath));
            itv++;
        }
    }else{
        QDir dir(sPath);
        if(!dir.exists())
        {
            return 0;
        }
        dir.setFilter(QDir::Dirs | QDir::NoSymLinks);
        list = dir.entryList();
    }
    int file_count = list.count();
    if(file_count <= 0)
    {
        return 0;
    }
    int added = 0;
    nLevel--;
    for(int i=0; i<list.size(); i++)
    {
        QString sfile = list.at(i);
        if(sfile == "." || sfile == "..")
            continue;
        DirSelItem * pdsi = NULL;
        if((pdsi = GetChildDirSelItem(mapItems, sfile))){
            QString sFullPath = "";
            if(sPath != "/"){
                sFullPath = sPath + "/" + sfile;
            }else{
                sFullPath = sPath + sfile;
            }
            added += InitTreeItems(nID, bSelected, pdsi->m_mapChild, sFullPath, nLevel);
            continue;
        }
        DirSelItem *pitem = new DirSelItem(++nID, sPath, sfile, bSelected);
        added++;
        mapItems[nID-1] = pitem;
        if(nLevel > 0){
            QString sFullPath = "";
            if(sPath != "/"){
                sFullPath = sPath + "/" + sfile;
            }else{
                sFullPath = sPath + sfile;
            }
            added += InitTreeItems(nID, bSelected, pitem->m_mapChild, sFullPath, nLevel);
        }
    }
    return added;
}
