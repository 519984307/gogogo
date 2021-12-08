#include "dynmenu.h"
#include "common.h"

DynMenu::DynMenu(bool isSecureMachine)
{
    m_bIsSecureMachine = isSecureMachine;
    m_ppImage = new QPixmap;
    setFixedWidth(160);
    LoadMenu(m_bIsSecureMachine);
}


void DynMenu::LoadMenu(bool isSecureMachine)
{
    m_bIsSecureMachine = isSecureMachine;
    logAction = addAction(u8"日志");
    loadActionPixmap(logAction, ":/resource/menu_log_btn", 0);

    isolateAction = addAction(u8"隔离区");
    loadActionPixmap(isolateAction, ":/resource/menu_isolation_btn", 0);

    updateAction = addAction(u8"检测更新");
    loadActionPixmap(updateAction, ":/resource/menu_update_btn", 0);

    localUpdateAction = addAction(u8"离线升级");
    loadActionPixmap(localUpdateAction, ":/resource/menu_offline_update_btn", 0);

    aboutAction = addAction(u8"关于");
    loadActionPixmap(aboutAction, ":/resource/menu_about_btn", 0);

    connect(this,SIGNAL(hovered(QAction*)),this,SLOT(hoverMenu(QAction*)));
    //connect(this,SIGNAL(leaveEvent(QEvent*)),this,SLOT(leaveMenu(QEvent*)));
}

int DynMenu::loadActionPixmap(QAction *pAction, QString pic_name, int nIndex)
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

void DynMenu::setUpdateTitle(const QString &title, bool showed) {
    updateAction->setVisible(showed);
    if (showed) {
        updateAction->setText(title);
    }
}

void DynMenu::HideNonAdminMenu()
{
    removeAction(logAction);
    removeAction(isolateAction);
    removeAction(localUpdateAction);
}

void DynMenu::hoverMenu(QAction *pAction)
{
    if(pAction == logAction){
        loadActionPixmap(logAction, ":/resource/menu_log_btn", 1);
        loadActionPixmap(isolateAction, ":/resource/menu_isolation_btn", 0);
        if (!m_bIsSecureMachine) {
            loadActionPixmap(updateAction, ":/resource/menu_update_btn", 0);
        }
        loadActionPixmap(localUpdateAction, ":/resource/menu_offline_update_btn", 0);
        loadActionPixmap(aboutAction, ":/resource/menu_about_btn", 0);
    }else if(pAction == isolateAction){
        loadActionPixmap(isolateAction, ":/resource/menu_isolation_btn", 1);
        loadActionPixmap(logAction, ":/resource/menu_log_btn", 0);
        if (!m_bIsSecureMachine) {
            loadActionPixmap(updateAction, ":/resource/menu_update_btn", 0);
        }
        loadActionPixmap(localUpdateAction, ":/resource/menu_offline_update_btn", 0);
        loadActionPixmap(aboutAction, ":/resource/menu_about_btn", 0);
    }else if(pAction == updateAction){
        if (!m_bIsSecureMachine) {
            loadActionPixmap(updateAction, ":/resource/menu_update_btn", 1);
        }
        loadActionPixmap(logAction, ":/resource/menu_log_btn", 0);
        loadActionPixmap(isolateAction, ":/resource/menu_isolation_btn", 0);
        loadActionPixmap(localUpdateAction, ":/resource/menu_offline_update_btn", 0);
        loadActionPixmap(aboutAction, ":/resource/menu_about_btn", 0);

    }else if(pAction == localUpdateAction){
        loadActionPixmap(logAction, ":/resource/menu_log_btn", 0);
        loadActionPixmap(isolateAction, ":/resource/menu_isolation_btn", 0);
        if (!m_bIsSecureMachine) {
            loadActionPixmap(updateAction, ":/resource/menu_update_btn", 0);
        }
        loadActionPixmap(localUpdateAction, ":/resource/menu_offline_update_btn", 1);
        loadActionPixmap(aboutAction, ":/resource/menu_about_btn", 0);

    }else if(pAction == aboutAction){
        loadActionPixmap(aboutAction, ":/resource/menu_about_btn", 0);
        loadActionPixmap(logAction, ":/resource/menu_log_btn", 0);
        loadActionPixmap(isolateAction, ":/resource/menu_isolation_btn", 0);
        if (!m_bIsSecureMachine) {
            loadActionPixmap(updateAction, ":/resource/menu_update_btn", 0);
        }
        loadActionPixmap(localUpdateAction, ":/resource/menu_offline_update_btn", 0);
    }
}

void DynMenu::leaveEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);
    loadActionPixmap(logAction, ":/resource/menu_log_btn", 0);
    loadActionPixmap(isolateAction, ":/resource/menu_isolation_btn", 0);
    if (!m_bIsSecureMachine) {
        loadActionPixmap(updateAction, ":/resource/menu_update_btn", 0);
    }
    loadActionPixmap(localUpdateAction, ":/resource/menu_offline_update_btn", 0);
    loadActionPixmap(aboutAction, ":/resource/menu_about_btn", 0);
    //qDebug("leaveMenu event");
}
