#ifndef LISTDATADELEGATE_H
#define LISTDATADELEGATE_H

template<class T>
class ListViewDataDelegate {
public:
    virtual int Count() = 0;
    virtual void Content(int pos, T& data) = 0;
    virtual void SelectData(int pos) = 0;
    virtual void DeleteData(int pos) = 0;
};

#endif // LISTDATADELEGATE_H
