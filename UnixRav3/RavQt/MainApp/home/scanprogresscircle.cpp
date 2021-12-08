#include <QPainter>
#include <QMatrix>

#include "scanprogresscircle.h"

ScanProgressCircle::ScanProgressCircle(QWidget *parent, CircleType type)
    : QWidget(parent),m_nProgress(0),m_nDegree(0),m_ctType(type),m_wlLevel(NORMAL)
{
    pthread_mutex_init(&m_tLock, NULL);
   // m_pLabel = new QLabel(this);
    //m_pLabel->setFixedSize(84, 84);
    setFixedSize(88, 88);
    setAttribute(Qt::WA_TranslucentBackground, true);
    //setAutoFillBackground(false);

    QString images[] = {":/resource/all_kill_big", ":/resource/fast_kill_big", ":/resource/custom_kill_big", ":/resource/admin_kill_big"};
    m_ppmCenter = new QPixmap();
    m_ppmCenter->load(images[type]);
    m_ppmMotion = new QPixmap();
    m_ppmMotion->load(":/resource/in_progress");
    //m_ppmMotion->load(":/resource/logo");

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(timerEvent()));

}

ScanProgressCircle::~ScanProgressCircle()
{
    delete m_ppmCenter;
    delete m_ppmMotion;
    //delete m_pLabel;

    delete m_pTimer;
}

void ScanProgressCircle::timerEvent()
{
    if(m_nDegree >= 360) {
        m_nDegree = 0;
    }
    m_nDegree += 10;

    update();
}

void ScanProgressCircle::StartMotion()
{
    m_pTimer->start(100);
}

void ScanProgressCircle::StopMotion()
{
    m_pTimer->stop();
}

void ScanProgressCircle::Reset()
{
    m_pTimer->stop();
    m_nProgress = 0;
    m_nDegree = 0;
    m_wlLevel = NORMAL;
    update();
}

void ScanProgressCircle::SetCircleType(CircleType type)
{
    QString images[] = {":/resource/all_kill_big", ":/resource/fast_kill_big", ":/resource/custom_kill_big", ":/resource/admin_kill_big"};
    //Lock();
    m_ppmCenter->load(images[type]);
    m_ctType = type;
    //UnLock();
    update();
}

void ScanProgressCircle::SetWarnLevel(WarnLevel level)
{
    m_wlLevel = level;
    update();
}

void ScanProgressCircle::SetProgress(int progress)
{
    if(progress < 0 || progress > 100) {
        return;
    }
    m_nProgress = progress;
}
// warn:246,198,143  criticle:246,147,96
void ScanProgressCircle::paintEvent(QPaintEvent *)
{
    QColor color;
    int index;
    if(m_wlLevel == NORMAL) {
        color = QColor(186,171,208);
        index = 0;
    }else if(m_wlLevel == WARNING) {
        color = QColor(246,198,143);
        index = 1;
    }else if(m_wlLevel == CRITICLE) {
        color = QColor(246,147,96);
        index = 2;
    }
    Lock();
    QPainter painter(this);
    //painter.begin(this);
    painter.save();
    painter.translate(42, 42);
    painter.rotate(m_nDegree);
    painter.translate(-42, -42);
    painter.drawPixmap(0, 0, 84, 84, *m_ppmMotion);
    painter.restore();
    //painter.end();
    painter.drawPixmap(1, 1, m_ppmCenter->width()/3, m_ppmCenter->height(), m_ppmCenter->copy(m_ppmCenter->width()/3*index,0,m_ppmCenter->width()/3,m_ppmCenter->height()));
    painter.setPen(QPen(color,4,Qt::SolidLine));
    QRectF rect(2, 2, 84, 84);
    painter.drawArc(rect, 90*16, -57.6*m_nProgress);
    UnLock();
}

void ScanProgressCircle::Lock()
{
    pthread_mutex_lock(&m_tLock);
}

void ScanProgressCircle::UnLock()
{
    pthread_mutex_unlock(&m_tLock);
}
