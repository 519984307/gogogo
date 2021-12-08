#include <QPainter>
#include <QToolTip>
#include <QModelIndex>
#include <QEvent>
#include <QHelpEvent>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
//#include <QGuiApplication>

#include <QDebug>

#include "midbodywidget.h"
#include "common.h"

MidBodyWidget::MidBodyWidget(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(APP_MID_BODY_HTIGHT);

    LoadMidTextArcImage();
    LoadImgTipsLabel();
    LoadButton();
    SetBtnValue(8, 5);
    m_bTipsEventOccur = false;
    setMouseTracking(true);
    //setAutoFillBackground(true);
    //connect(this, SIGNAL(entered(QModelIndex)), m_pBasicEngine, SLOT(showToolTip(QModelIndex)));
}


void MidBodyWidget::LoadMidTextArcImage()
{
    m_ppTextImage = new QPixmap;
    bool bret = m_ppTextImage->load(":/resource/smart_security_engine_txt");
    if(!bret){
//        qDebug("MidBodyWidget::LoadMidTextArcImage smart_security_engine_txt fail");
    }
    m_ppArcImage = new QPixmap;
    bret = m_ppArcImage->load(":/resource/arc");
    if(!bret){
//        qDebug("MidBodyWidget::LoadMidTextArcImage arc fail");
    }
    //update();
}

void MidBodyWidget::LoadButton()
{
    m_pDefendButton = new DefendButton(":/resource/defend_btn", u8"防护中心");
    m_pDefendButton->setParent(this);
    //QRect pos = geometry();
    //qDebug("MidBodyWidget1 x,y,w,h %d  %d  %d  %d", pos.x(), pos.y(), pos.width(), pos.height());
    m_pDefendButton->setGeometry(APP_SCREEN_WIDTH-m_pDefendButton->m_nBackgroundImageWidth-30, APP_MID_BODY_HTIGHT-m_pDefendButton->m_nBackgroundImageHeight/3-20,
                                 m_pDefendButton->m_nBackgroundImageWidth, m_pDefendButton->m_nBackgroundImageHeight/3);

    m_pDefendButton->SetBtnText(u8"防护中心", 2, 0);
    m_pDefendButton->activateWindow();
    //pos = m_pDefendButton->geometry();
    //qDebug("MidBodyWidget2 x,y,w,h %d  %d  %d  %d", pos.x(), pos.y(), pos.width(), pos.height());
//#RAVQT_FUNC_FLAG  0x01:original 0x02:whitelist 0x04:kv-backup 0x08:v-scan 0x10:schedul-scan 0x20:file-monitor 0x40:udisk-monitor
//DEFINES += RAVQT_FUNC_FLAG=127 (127 or 31)
//    qDebug("RAVQT_FUNC_FLAG=%d  %d %d", RAVQT_FUNC_FLAG, (RAVQT_FUNC_FLAG & 0x20), (RAVQT_FUNC_FLAG & 0x40));
//    if((RAVQT_FUNC_FLAG & 0x20)==0 || (RAVQT_FUNC_FLAG & 0x40)==0)
//        m_pDefendButton->hide();
}

void MidBodyWidget::LoadImgTipsLabel()
{
    m_pBasicEngine = new ImgTipsLabel(":/resource/basic_engine", ":/resource/4_engine_tips", IMG_LABEL_TYPE_BASIC_ENGINE);
    m_pBasicEngine->setParent(this);
    m_pBasicEngineText = new QLabel(this);

    m_pAdminEngine = new ImgTipsLabel(":/resource/admin_engine",  ":/resource/4_engine_tips", IMG_LABEL_TYPE_ADMIN_ENGINE);
    m_pAdminEngine->setParent(this);
    m_pAdminEngineText = new QLabel(this);

    m_pCloudEngine = new ImgTipsLabel(":/resource/cloud_engine", ":/resource/4_engine_tips", IMG_LABEL_TYPE_CLOUD_ENGINE);
    m_pCloudEngine->setParent(this);
    m_pCloudEngineText = new QLabel(this);

    m_pGeneEngine = new ImgTipsLabel(":/resource/gene_engine", ":/resource/4_engine_tips", IMG_LABLE_TYPE_GENE_ENGINE);
    m_pGeneEngine->setParent(this);
    m_pGeneEngineText = new QLabel(this);

    m_ppEngineNameText = new QPixmap;
    bool bret = m_ppEngineNameText->load(":/resource/4_engine_name_txt");
    if(!bret){
//        qDebug("ImgTipsLabel::LoadImage 4_engine_name_txt fail");
    }
    m_pBasicEngineText->setPixmap(m_ppEngineNameText->copy(0,0,m_ppEngineNameText->width(),m_ppEngineNameText->height()/4));
    m_pAdminEngineText->setPixmap(m_ppEngineNameText->copy(0,m_ppEngineNameText->height()/4,m_ppEngineNameText->width(),m_ppEngineNameText->height()/4));
    m_pCloudEngineText->setPixmap(m_ppEngineNameText->copy(0,m_ppEngineNameText->height()/2,m_ppEngineNameText->width(),m_ppEngineNameText->height()/4));
    m_pGeneEngineText->setPixmap(m_ppEngineNameText->copy(0,m_ppEngineNameText->height()/4*3,m_ppEngineNameText->width(),m_ppEngineNameText->height()/4));

    QRect pos = geometry();
    m_pBasicEngine->setGeometry(APP_SCREEN_WIDTH/2-180, pos.y()+30, m_pBasicEngine->m_ppIcoImage->width()/2, m_pBasicEngine->m_ppIcoImage->height());
    m_pAdminEngine->setGeometry(APP_SCREEN_WIDTH/2-60, pos.y()+50, m_pBasicEngine->m_ppIcoImage->width()/2, m_pBasicEngine->m_ppIcoImage->height());
    m_pCloudEngine->setGeometry(APP_SCREEN_WIDTH/2+60, pos.y()+50, m_pBasicEngine->m_ppIcoImage->width()/2, m_pBasicEngine->m_ppIcoImage->height());
    m_pGeneEngine->setGeometry(APP_SCREEN_WIDTH/2+180, pos.y()+30, m_pBasicEngine->m_ppIcoImage->width()/2, m_pBasicEngine->m_ppIcoImage->height());

    m_pBasicEngineText->setGeometry(APP_SCREEN_WIDTH/2-179, pos.y()+80, m_ppEngineNameText->width(),m_ppEngineNameText->height()/4);
    m_pAdminEngineText->setGeometry(APP_SCREEN_WIDTH/2-70, pos.y()+100, m_ppEngineNameText->width(),m_ppEngineNameText->height()/4);
    m_pCloudEngineText->setGeometry(APP_SCREEN_WIDTH/2+60, pos.y()+100, m_ppEngineNameText->width(),m_ppEngineNameText->height()/4);
    m_pGeneEngineText->setGeometry(APP_SCREEN_WIDTH/2+183, pos.y()+80, m_ppEngineNameText->width(), m_ppEngineNameText->height()/4);

//    m_pBasicEngine->activateWindow();
//    m_pAdminEngine->activateWindow();
//    m_pCloudEngine->activateWindow();
//    m_pGeneEngine->activateWindow();

//    m_pBasicEngineText->activateWindow();
//    m_pAdminEngineText->activateWindow();
//    m_pCloudEngineText->activateWindow();
//    m_pGeneEngineText->activateWindow();

    m_pTipsLabel = new QLabel(this);
    //m_pTipsLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
}

void MidBodyWidget::SetBtnValue(int nTotal, int nCurrent)
{
    m_pDefendButton->SetBtnText(u8"防护中心", nTotal, nCurrent);
    m_pDefendButton->update();
}

void MidBodyWidget::SetFunctions(int nFunctions)
{
    if ((nFunctions & AppConfig::FILE_DEFEN) || (nFunctions & AppConfig::PLUGIN_CHECKER) || (nFunctions & AppConfig::NET_FILTER)) {
        m_pDefendButton->show();
    }else{
        m_pDefendButton->hide();
    }
};

bool MidBodyWidget::event(QEvent *pEvent)
{
    if (pEvent->type() == QEvent::ToolTip) {
        QPixmap *pmp = m_pBasicEngine->m_ppTipsImage;
        QPoint cpos = QCursor::pos();

        if(QRect(m_pBasicEngine->x(),m_pBasicEngine->y(),m_pBasicEngine->width(),m_pBasicEngine->height()).contains(mapFromGlobal(cpos))){
            m_pTipsLabel->setPixmap(pmp->copy(0,0,pmp->width(),pmp->height()/4));
            m_rLastTipsRect.setRect(m_pBasicEngine->x(),m_pBasicEngine->y()-20,m_pBasicEngine->m_ppTipsImage->width(),m_pBasicEngine->m_ppTipsImage->height()/4);
            m_bTipsEventOccur = true;
            //qDebug() << "MidBodyWidget::event: show tips";
        }else if(QRect(m_pAdminEngine->x(),m_pAdminEngine->y(),m_pAdminEngine->width(),m_pAdminEngine->height()).contains(mapFromGlobal(cpos))){
            m_pTipsLabel->setPixmap(pmp->copy(0,pmp->height()/4,pmp->width(),pmp->height()/4));
            m_rLastTipsRect.setRect(m_pAdminEngine->x(),m_pAdminEngine->y()-20,m_pAdminEngine->m_ppTipsImage->width(),m_pAdminEngine->m_ppTipsImage->height()/4);
            m_bTipsEventOccur = true;
        }else if(QRect(m_pCloudEngine->x(),m_pCloudEngine->y(),m_pCloudEngine->width(),m_pCloudEngine->height()).contains(mapFromGlobal(cpos))){
            m_pTipsLabel->setPixmap(pmp->copy(0,pmp->height()/2,pmp->width(),pmp->height()/4));
            m_rLastTipsRect.setRect(m_pCloudEngine->x(),m_pCloudEngine->y()-20,m_pCloudEngine->m_ppTipsImage->width(),m_pCloudEngine->m_ppTipsImage->height()/4);
            m_bTipsEventOccur = true;
        }else if(QRect(m_pGeneEngine->x(),m_pGeneEngine->y(),m_pGeneEngine->width(),m_pGeneEngine->height()).contains(mapFromGlobal(cpos))){
            m_pTipsLabel->setPixmap(pmp->copy(0,pmp->height()/4*3,pmp->width(),pmp->height()/4));
            m_rLastTipsRect.setRect(m_pGeneEngine->x(),m_pGeneEngine->y()-20,m_pGeneEngine->m_ppTipsImage->width(),m_pGeneEngine->m_ppTipsImage->height()/4);
            m_bTipsEventOccur = true;
        }else{
            m_bTipsEventOccur = false;
        }
        if(m_bTipsEventOccur){
            m_pTipsLabel->setGeometry(m_rLastTipsRect.x(), m_rLastTipsRect.y(), m_rLastTipsRect.width(), m_rLastTipsRect.height());
            m_pTipsLabel->activateWindow();
            m_pTipsLabel->show();
        }else{
            m_pTipsLabel->hide();
            pEvent->ignore();
        }
        update();
        return true;
    }else if (pEvent->type() == QEvent::MouseMove) {
        QPoint cpos = QCursor::pos();
        if(QRect(m_pBasicEngine->x(),m_pBasicEngine->y(),m_pBasicEngine->width(),m_pBasicEngine->height()).contains(mapFromGlobal(cpos))){
            m_bTipsEventOccur = true;
        }else if(QRect(m_pAdminEngine->x(),m_pAdminEngine->y(),m_pAdminEngine->width(),m_pAdminEngine->height()).contains(mapFromGlobal(cpos))){
            m_bTipsEventOccur = true;
        }else if(QRect(m_pCloudEngine->x(),m_pCloudEngine->y(),m_pCloudEngine->width(),m_pCloudEngine->height()).contains(mapFromGlobal(cpos))){
            m_bTipsEventOccur = true;
        }else if(QRect(m_pGeneEngine->x(),m_pGeneEngine->y(),m_pGeneEngine->width(),m_pGeneEngine->height()).contains(mapFromGlobal(cpos))){
            m_bTipsEventOccur = true;
        }else{
            m_bTipsEventOccur = false;
        }
        if (!m_bTipsEventOccur) {
            m_pTipsLabel->hide();
            update();
            //qDebug("MidBodyWidget::event  pEvent->type() == QEvent::ToolTip m_bTipsEventOccur=%d", m_bTipsEventOccur?1:0);
        }
    }
    return QWidget::event(pEvent);
}

void MidBodyWidget::paintEvent(QPaintEvent *pPEvent)
{
    Q_UNUSED(pPEvent);

    QRect pos = rect();
    QPainter painter(this);
    painter.drawPixmap((pos.width()-m_ppTextImage->width())/2, 20, m_ppTextImage->width(), m_ppTextImage->height(), *m_ppTextImage);
    painter.drawPixmap((pos.width()-m_ppArcImage->width()-20)/2, 35, m_ppArcImage->width()+20, m_ppArcImage->height(), *m_ppArcImage);

//    if(m_bTipsEventOccur){
//        m_pTipsLabel->setGeometry(m_rLastTipsRect.x(), m_rLastTipsRect.y(), m_rLastTipsRect.width(), m_rLastTipsRect.height());

//        m_pTipsLabel->activateWindow();
//        m_pTipsLabel->show();
//    }
    if(!m_bTipsEventOccur){
        m_pTipsLabel->hide();
    }
}

//void MidBodyWidget::showToolTip(const QModelIndex &index)
//{
//    if (index.isValid()) {
//        qDebug() << "Invalid index";
//        return;
//    }
//    qDebug() << "index:" << index.data().toString();
//    QToolTip::showText(QCursor::pos(), index.data().toString() + "\ngoto hell");
//}




