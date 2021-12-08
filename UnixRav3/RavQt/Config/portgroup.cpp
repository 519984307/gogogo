#include "portgroup.h"
#include "common/pubdef.h"

#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QStringList>
#include <QFileDialog>
#include <QModelIndex>

#include <QDebug>

//#define WHITELIST_GROUP_WIDTH   620
#define PORT_GROUP_HEIGHT  280

PortGroup::PortGroup(int flag, QWidget *parent)
    :QGroupBox(parent),m_bFocus(false),Wflag(flag)
{
    QFont font;
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
    font.setPointSize(10);
    setFont(font);

    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
    setMaximumHeight(PORT_GROUP_HEIGHT);

    setFlat(true);

    setStyleSheet("QGroupBox {\
                  font-size:13px; \
                  border: 2px solid #7B599F;\
                  margin-top: 1ex; \
                  }\
                  QGroupBox::title {\
                  top: 1px;\
                  left: 20px;\
                  subcontrol-origin: margin;\
                  subcontrol-position: top left;}");

	switch(Wflag)
	{
		case 0:
			setTitle(u8"白名单");
			InitWidget("ip/ip段：");
			break;

		case 1:
			setTitle(u8"黑名单");
			InitWidget("ip/ip段：");
			break;

		case 2:
			setTitle(u8"端口");
			InitWidget("端口&协议&方向：");
			break;

		default :
			setTitle(u8"白名单");
			InitWidget("ip/ip段：");
			break;
	}

	firewallflag = false;
    SetFocus(m_bFocus);
}

void PortGroup::InitWidget(QString str)
{

#ifdef QT_NEW_HEADER_54
    QFont font("Arial", 10, QFont::Thin, false);
#else
    QFont font("Arial", 10, QFont::Light, false);
#endif

    QGridLayout *glayout = new QGridLayout;


    QHBoxLayout *fdlayout = new QHBoxLayout;
    QLabel *pDFLabel = new QLabel(str, this);
    pDFLabel->setFont(font);
    m_ppbDirFile = new PushButton(this);
    m_ppbDirFile->loadPixmap(":/resource/plus_btn");

    fdlayout->addWidget(pDFLabel, 0, Qt::AlignRight);
    fdlayout->addWidget(m_ppbDirFile, 0, Qt::AlignRight);
    glayout->addLayout(fdlayout, 0, 0, Qt::AlignRight);

    QVBoxLayout *all_layout = new QVBoxLayout;
	switch(Wflag)
	{
		case 0:
			m_IpWhiteName = new IpWhiteNameTable(this);
    		glayout->addWidget(m_IpWhiteName, 1, 0);
    		all_layout->addLayout(glayout);

			//QLabel *pinfo = new QLabel;
    		//QPixmap *pxinfo = new QPixmap(":/resource/whitelist_info");
    		//pinfo->setPixmap(*pxinfo);
    		//pinfo->setFixedSize(pxinfo->width(), pxinfo->height());
    		//QHBoxLayout * pinfolayout = new QHBoxLayout;
    		//pinfolayout->addWidget(pinfo);
    		//pinfolayout->setAlignment(Qt::AlignLeft);
    		//all_layout->addLayout(pinfolayout);
			break;

		case 1:
			m_IpBlackName = new IpBlackNameTable(this);
    		glayout->addWidget(m_IpBlackName, 1, 0);
    		all_layout->addLayout(glayout);
			break;

		case 2:
			m_PortName = new PortNameTable(this);
    		glayout->addWidget(m_PortName, 1, 0);
    		all_layout->addLayout(glayout);
			break;

		default :
			m_IpWhiteName = new IpWhiteNameTable(this);
    		glayout->addWidget(m_IpWhiteName, 1, 0);
    		all_layout->addLayout(glayout);
			break;
	}

    setLayout(all_layout);

    connect(m_ppbDirFile, SIGNAL(clicked()), this, SLOT(AddClicked()));

}

void PortGroup::AddClicked()
{
	if(firewallflag)
	{
		switch(Wflag)
		{
			case 0:
					if(!m_IpWhiteName->ischanged)
						m_IpWhiteName->AddRowEdit();
				break;

			case 1:
					if(!m_IpBlackName->ischanged)
						m_IpBlackName->AddRowEdit();
				break;

			case 2:
					if(!m_PortName->ischanged)
						m_PortName->AddRowEdit();
				break;

			default :
				break;
		}
	}
}

void PortGroup::SetFocus(bool bFocus)
{
    m_bFocus = bFocus;
    if(m_bFocus){
        setStyleSheet("QGroupBox {\
              font-size:13px; \
              border: 2px solid #7B599F;\
              margin-top: 1ex; \
              }\
              QGroupBox::title {\
              top: 1px;\
              left: 20px;\
              subcontrol-origin: margin;\
              subcontrol-position: top left;}");

    }else{
        setStyleSheet("QGroupBox {\
              font-size:13px; \
              border: 2px solid #EFEBE7;\
              margin-top: 1ex; \
              }\
              QGroupBox::title {\
              top: 1px;\
              left: 20px;\
              subcontrol-origin: margin;\
              subcontrol-position: top left;}");
    }
    update();
}
