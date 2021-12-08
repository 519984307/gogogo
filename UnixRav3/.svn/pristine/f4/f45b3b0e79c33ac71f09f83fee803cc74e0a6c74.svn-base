#include <QGridLayout>
#include <QHBoxLayout>
#include <QIntValidator>

#include "common/pubdef.h"

#include "kvgroup.h"
#include <sys/types.h>
#include <sys/stat.h>


//#define BACKUP_LEFT_LABEL_WIDTH    150
//#define BACKUP_RIGHT_CHKBOX_WIDTH  180

KVGroup::KVGroup(QWidget *parent):QGroupBox(parent),m_bFocus(false)
{
    setTitle(u8"病毒扫描");
    QFont font;
#ifdef QT_NEW_HEADER_54
    font.setWeight(QFont::Thin);
#else
    font.setWeight(QFont::Light);
#endif
    font.setPointSize(10);
    setFont(font);

	bool  nShowCloudEngine = false;
	struct stat s;
	nShowCloudEngine = stat("/opt/BDFZ/RAV/plugins/cloud.engine.1", &s)==0;
    setFixedWidth(APP_SCREEN_WIDTH-APP_SCREEN_LEFT_WIDTH-30);
    setFixedHeight(nShowCloudEngine ?285:235);
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

    lFileType = new QLabel(u8"文件类型：", this);
    lFileType->setFont(font);
    m_pbgFileType = new QButtonGroup(this);
    m_prbAllFiles = new RadioButton(u8"所有文件", this);
    m_prbProgDoc = new RadioButton(u8"程序和文档", this);
    m_prbUserDefine = new RadioButton(u8"自定义", this);
    m_pleFileTypes = new QLineEdit(this);
    m_pbgFileType->addButton(m_prbAllFiles, APP_FILE_ALL);
    m_pbgFileType->addButton(m_prbProgDoc, APP_FILE_PROGDOC);
    m_pbgFileType->addButton(m_prbUserDefine, APP_FILE_USER_DEFINE);
    m_prbAllFiles->setFont(font);
    m_prbProgDoc->setFont(font);
    m_pleFileTypes->setFont(font);

    m_prbUserDefine->setFont(font);
    lFileType->setFixedHeight(m_prbAllFiles->height());
    lFileType->setGeometry(25, 30, lFileType->width(), lFileType->height());
    m_prbAllFiles->setGeometry(110, lFileType->y()+3, m_prbAllFiles->width()+5, m_prbAllFiles->height());
    m_prbProgDoc->setGeometry(270, lFileType->y()+3, m_prbProgDoc->width()+5, m_prbProgDoc->height());
    m_prbUserDefine->setGeometry(430, lFileType->y()+3, m_prbUserDefine->width()+5, m_prbUserDefine->height());
    m_pleFileTypes->setGeometry(110, lFileType->y()+30, 300, 20);

    lKVEngine = new QLabel(u8"查杀引擎：", this);
    lKVEngine->setFixedHeight(m_prbAllFiles->height());
    lKVEngine->setFont(font);
    m_pclOnWildlist = new CheckboxLabel(this, u8"仅查杀流行病毒");
    m_pclHeurist = new CheckboxLabel(this, u8"启发式查杀");
    m_pclProcCompressed = new CheckboxLabel(this, u8"启动压缩包查杀");
    lcomment1 = new QLabel(u8"查杀不大于", this);
    lcomment2 = new QLabel(u8"M的压缩包", this);
    lcomment3 = new QLabel(u8"（重点查杀活跃病毒）", this);
    lcomment4 = new QLabel(u8"（可有效发现可疑病毒）", this);
    lcomment5 = new QLabel(u8"（查杀压缩包内的文件）", this);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::gray);
    disableLablePalette = pa;
    lcomment3->setPalette(pa);
    lcomment4->setPalette(pa);
    lcomment5->setPalette(pa);
    m_pleSize = new QLineEdit("100", this);
    m_pclOnWildlist->setFont(font);
    m_pclHeurist->setFont(font);
    m_pclProcCompressed->setFont(font);
    lcomment1->setFont(font);
    normalLabelPalette = lcomment1->palette();
    lcomment2->setFont(font);
    lcomment3->setFont(font);
    lcomment4->setFont(font);
    lcomment5->setFont(font);
    m_pleSize->setFont(font);
    lcomment1->adjustSize();
    lcomment2->adjustSize();
    lcomment3->adjustSize();
    lcomment4->adjustSize();
    lcomment5->adjustSize();
    lKVEngine->setGeometry(25, m_pleFileTypes->y()+25, lKVEngine->width(), lKVEngine->height());
    m_pclOnWildlist->setGeometry(110, m_pleFileTypes->y()+28, m_pclOnWildlist->width(), lcomment1->height());
    lcomment3->setGeometry(110+m_pclOnWildlist->width(), m_pleFileTypes->y()+33, lcomment3->width(), lcomment3->height());
    m_pclHeurist->setGeometry(110, m_pclOnWildlist->y()+28, m_pclHeurist->width(), lcomment1->height());
    lcomment4->setGeometry(110+m_pclHeurist->width(), m_pclOnWildlist->y()+31, lcomment4->width(), lcomment4->height());
    m_pclProcCompressed->setGeometry(110, m_pclHeurist->y()+28, m_pclProcCompressed->width(), lcomment1->height());
    lcomment5->setGeometry(110+m_pclProcCompressed->width(), m_pclHeurist->y()+31, lcomment5->width(), lcomment5->height());
    lcomment1->setGeometry(140, m_pclProcCompressed->y()+25, lcomment1->width()+5, lcomment1->height());
    m_pleSize->setAlignment(Qt::AlignRight);
    m_pleSize->setValidator(new QIntValidator(0, 1000, this));
    m_pleSize->setGeometry(140+lcomment1->width(), m_pclProcCompressed->y()+25, 50, lcomment1->height()+2);
    lcomment2->setGeometry(140+lcomment1->width()+55, m_pclProcCompressed->y()+25, lcomment2->width(), lcomment2->height());

    lFoundVirus = new QLabel(u8"发现病毒：", this);
    lFoundVirus->setFont(font);
    lFoundVirus->adjustSize();
    m_pbgFoundVirus = new QButtonGroup;
    m_prbAuto = new RadioButton(u8"自动处理", this);
    m_prbManual = new RadioButton(u8"手动处理", this);
    m_prbIgnore = new RadioButton(u8"不处理", this);
    m_pbgFoundVirus->addButton(m_prbAuto, APP_FOUNDV_AUTO);
    m_pbgFoundVirus->addButton(m_prbManual, APP_FOUNDV_MANUAL);
    m_pbgFoundVirus->addButton(m_prbIgnore, APP_FOUNDV_IGNORE);
    m_prbAuto->setFont(font);
    m_prbManual->setFont(font);
    lFoundVirus->setGeometry(25, 202, lFoundVirus->width(), lFoundVirus->height());
    m_prbAuto->setGeometry(110, 195, m_prbAuto->width()+5, m_prbAuto->height());
    m_prbManual->setGeometry(270, 195, m_prbManual->width()+5, m_prbAuto->height());
    m_prbIgnore->setGeometry(430, 195, m_prbIgnore->width()+5, m_prbAuto->height());

	// 20210618zcg+
	m_pCloudEngineLabel = 0; m_pCloudEnable = 0; m_pCloudUrlLabel = 0; m_pCloudUrl = 0;
	if(nShowCloudEngine)
	{
		m_pCloudEngineLabel = new QLabel(u8"云引擎：", this);
		m_pCloudEngineLabel->setFont(font);
		m_pCloudEngineLabel->adjustSize();
		m_pCloudEnable = new CheckboxLabel(this, u8"是否启用");
		m_pCloudEnable->setFont(font);
		m_pCloudUrlLabel = new QLabel(u8"URL", this);
		m_pCloudUrlLabel->setFont(font);
		m_pCloudUrlLabel->adjustSize();
		m_pCloudUrl = new QLineEdit(this);
		m_pCloudUrl->setFont(font);

		m_pCloudEngineLabel->setGeometry(25, lFoundVirus->y()+25, m_pCloudEngineLabel->width(), m_pCloudEngineLabel->height());
		m_pCloudEnable->setGeometry(110, lFoundVirus->y()+21, m_pCloudEnable->width(), m_pCloudEnable->height());
		m_pCloudUrlLabel->setGeometry(110, m_pCloudEnable->y()+30, m_pCloudUrlLabel->width(), m_pCloudUrlLabel->height());
		m_pCloudUrl->setGeometry(m_pCloudUrlLabel->x()+30, m_pCloudEnable->y()+25, 300, 20);
	}

    connect(m_pclProcCompressed, SIGNAL(clicked()), this, SLOT(SetSizeEditValid()));
    connect(m_pbgFileType, SIGNAL(buttonClicked(int)), this, SLOT(UpdateEditStatus(int)));

    SetFocus(m_bFocus);
}

void KVGroup::SetFocus(bool bFocus)
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

void KVGroup::SetSizeEditValid()
{
    if(m_pclProcCompressed->m_bChecked == false){
        m_pleSize->setDisabled(true);
        lcomment1->setDisabled(true);
        lcomment1->setPalette(disableLablePalette);
        lcomment2->setDisabled(true);
        lcomment2->setPalette(disableLablePalette);
    }else{
        m_pleSize->setDisabled(false);
        lcomment1->setDisabled(false);
        lcomment1->setPalette(normalLabelPalette);
        lcomment2->setDisabled(false);
        lcomment2->setPalette(normalLabelPalette);
    }

}

void KVGroup::UpdateEditStatus(int buttonID)
{
    switch (buttonID) {
      case APP_FILE_ALL:
        m_pleFileTypes->setDisabled(true);
        break;
      case APP_FILE_PROGDOC:
        m_pleFileTypes->setDisabled(true);
        break;
      case APP_FILE_USER_DEFINE:
        m_pleFileTypes->setDisabled(false);
        break;
      default:
        break;
    }
}

void KVGroup::SetCloudEngine(bool bEnable, const char * szUrl)
{
	if(m_pCloudEnable) m_pCloudEnable->SetValue(bEnable);
	if(m_pCloudUrl)    m_pCloudUrl->setText(szUrl);
}
