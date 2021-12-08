#include "modemenu.h"
#include "common.h"

ModeMenu::ModeMenu()
{
    LoadMenu();
}

//padding:8px 32px;                   border-bottom:1px solid #DBDBDB; #2dabf9
void ModeMenu::LoadMenu()
{
    autoAction = addAction(u8"自动");
    officeAction = addAction(u8"办公");
    highspeedAction = addAction(u8"高速");

    connect(this,SIGNAL(hovered(QAction*)),this,SLOT(hoverMenu(QAction*)));
    setFixedSize(80, 90);
    setStyleSheet("QMenu {\
                  background-color: white;\
                  border: 1px solid white;\
              }\
              QMenu::item {\
                  font-size: 10pt; \
                  background-color: transparent;\
                  padding:4px 4px 4px 4px; \
                  margin:0px 4px;\
              }\
              QMenu::item:selected { \
                  background-color:rgb(228, 221, 235);\
                  color:black;\
              }");
}

int ModeMenu::loadActionPixmap(QAction *pAction, QString pic_name, int nIndex)
{
    bool ret = m_ppImage->load(pic_name);
    if(ret) {
        int nWidth = m_ppImage->width()/3;
        int nHeight = m_ppImage->height();
        int nXPos = nIndex * nWidth;
        int nYPos = 0;
        pAction->setIcon(QIcon(m_ppImage->copy(nXPos, nYPos, nWidth, nHeight)));
        return 0;
    }
    return -1;
}


void ModeMenu::hoverMenu(QAction *)
{

}

void ModeMenu::leaveEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

}
