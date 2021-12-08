#include <QLabel>
#include <QPalette>
#include <QPen>
#include <QPainter>
#include <QPainterPath>
#include <QMutexLocker>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include "scanchart.h"

CurvePainter::CurvePainter(int ymax, QWidget *parent):QWidget(parent), yDataMax(ymax)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setFixedSize(203, 46);
    nTopSpace = 5;
    nBottomSpace = 5;
    xFactor = 20;
    yFactor = yDataMax/(height()-nTopSpace-nBottomSpace);
    QPalette pe;
    pe.setColor(QPalette::Background, QColor(216,187,229));   //216,187,229
    pe.setColor(QPalette::Foreground, Qt::black);
    setPalette(pe);
    setAutoFillBackground(true);
}
CurvePainter::~CurvePainter()
{

}

void CurvePainter::UpdateData(int data)
{
    bool maxChanged = false;
//    qDebug("max data is %d and current is %d", yDataMax, data);

    if(data > yDataMax){
        yDataMax = data;
        yPrevFactor = yFactor;
        yFactor = yDataMax/(height()-nTopSpace-nBottomSpace);
        maxChanged = true;
    }
    int i;
    QMutexLocker locker(&mutex);
    for(i=0; i<lstPointF.size(); i++){
        lstPointF[i].setX(lstPointF[i].rx()-xFactor);
        if(lstPointF[i].rx() < -20){
//            qDebug("i=%d remove point(%f, %f)", i, lstPointF[i].rx(), lstPointF[i].ry());
            lstPointF.removeAt(i);
            i -= 1;
            continue;
        }
        if (maxChanged) {
           lstPointF[i].setY(lstPointF[i].ry()*yPrevFactor/yFactor+nBottomSpace);
        }
    }
    QPointF newPF(width(), data/yFactor);
    lstPointF.push_back(newPF);
    update();
}

void CurvePainter::Reset()
{
    lstPointF.clear();
    update();
}

void CurvePainter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int w = width();
    int h = height();
    QVector<qreal> dashes;
    qreal space = 2;
    dashes << 2 << space << 2 <<space;
    QPen pen(Qt::lightGray,1);
    pen.setDashPattern(dashes);
    pen.setWidth(1);
    painter.setPen(pen);
    for(int i=1; i<21; i++){
        painter.drawLine(10*i, 0, 10*i, h);
    }
    for(int i=1; i<6; i++){
        painter.drawLine(0, 10*i, w, 10*i);
    }
    painter.save();
    QPen pen1;
    pen1.setWidth(2);
    pen1.setColor(Qt::white);
    painter.setPen(pen1);
    //painter.drawLine(0, 0, 205, 0);
    //painter.drawLine(10, 20, 30, 40);
    QMutexLocker locker(&mutex);
    if (lstPointF.length() <= 0) {
        return;
    }
    painter.setWindow(0, h, w, -h);
    painter.setViewport(0, nTopSpace, w, h-nBottomSpace);
    QPainterPath path(lstPointF[0]);
    for (int i = 0; i < lstPointF.size() - 1; ++i) {
        QPointF sp = lstPointF[i];
        QPointF ep = lstPointF[i+1];
        QPointF c1 = QPointF((sp.x() + ep.x()) / 2, sp.y());
        QPointF c2 = QPointF((sp.x() + ep.x()) / 2, ep.y());
        path.cubicTo(c1, c2, ep);
    }
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawPath(path);
    painter.restore();
}


ScanChart::ScanChart(int id, QWidget *parent) : QWidget(parent),m_bRunning(false)
{
    //whole widget size is 205, 68
    setFixedSize(205, 68); //curvepainter 205*47
    setAttribute(Qt::WA_StyledBackground, true);

    //title lable height is 21
    m_nID = id;
    m_nDataMax = 100;
    m_nFileCount = 0;

    QPalette pe(palette());
    pe.setColor(QPalette::Background, QColor(255, 255, 255));
    //pe.setColor(QPalette::Background, Qt::green);
    setPalette(pe);
    setAutoFillBackground(true);

    InitChart();
}

ScanChart::~ScanChart(){


}

void ScanChart::Start()
{
    m_bRunning = true;
}

void ScanChart::Stop()
{
    m_bRunning = false;
}

void ScanChart::Reset()
{
    m_bRunning = false;
    m_nFileCount = 0;
    m_pcpPainter->Reset();
    m_plTitleCount->setText("0");
    m_plTitleFileName->setText("");
}

int ScanChart::SimpleFileName(const string &filename, string &outStr, int maxLen)
{
    outStr = "";
    if (((int)filename.length()) > maxLen){
        size_t pos = filename.rfind("/");
        if(pos == string::npos) {
            outStr = filename.substr(0, maxLen);
            return 0;
        }
        string file = filename.substr(pos+1, filename.length()-pos-2);
        size_t len = file.length();
        if (((int)(len+4)) >= maxLen) {
            outStr = ".../" + file;
        }else {
            size_t cutLen = maxLen - len -4;
            outStr = filename.substr(0, cutLen) + ".../" + file;
        }

    }else{
        outStr = filename;
    }
    return 0;
}

void ScanChart::UpdateData(int count, const string &filename)
{
    if(m_bRunning == false) {
        return;
    }
    char tmp[20];
    sprintf(tmp, "%d", count);
    m_plTitleCount->setText(tmp);
    if (m_nFileCount > count) {
        m_nFileCount = count;
    }
    string file;
    if(filename.length() == 0 || SimpleFileName(filename, file) != 0) {
        file = "";
    }
    //qDebug("%s -> %s", filename.c_str(), file.c_str());
    m_plTitleFileName->setText(QString::fromStdString(file));

    if (m_nFileCount > 0) {
        m_pcpPainter->UpdateData(count - m_nFileCount);
    }
    m_nFileCount = count;
}

//size: 205*68
void ScanChart::InitChart()
{
    QPalette pe;
    pe.setColor(QPalette::Background, QColor(255, 255, 255));
    //setPalette(pe);
    QFont ft;
    ft.setPixelSize(10);
    ft.setWeight(0);

    QLabel *pcounttext = new QLabel(this);
    pe.setColor(QPalette::WindowText,Qt::black);
    pcounttext->setPalette(pe);
    pcounttext->setFont(ft);
    pcounttext->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    pcounttext->setText(QString("线程%1:").arg(m_nID, 0, 10));
    pcounttext->setFixedSize(45, 20);
    pcounttext->setGeometry(1, 0, pcounttext->width(), pcounttext->height());
    m_plTitleCount = new QLabel(this);
    m_plTitleCount->setFixedSize(40, 20);
    m_plTitleCount->setGeometry(pcounttext->width(), 0, m_plTitleCount->width() ,m_plTitleCount->height());
    m_plTitleCount->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    pe.setColor(QPalette::WindowText,Qt::red);
    m_plTitleCount->setPalette(pe);
    m_plTitleCount->setFont(ft);
    m_plTitleCount->setText("0");

    m_plTitleFileName = new QLabel(this);
    pe.setColor(QPalette::WindowText,Qt::black);
    m_plTitleFileName->setPalette(pe);
    m_plTitleFileName->setFixedSize(116, 20);
    m_plTitleFileName->setGeometry(pcounttext->width()+m_plTitleCount->width(),0, m_plTitleFileName->width(), m_plTitleFileName->height());
    m_plTitleFileName->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    m_plTitleFileName->setFont(ft);
    m_plTitleFileName->setText("");

    m_pcpPainter = new CurvePainter(m_nDataMax, this);
    m_pcpPainter->setGeometry(1, 21, 203, 46);
}

void ScanChart::paintEvent(QPaintEvent *)
{
    QPen pen;
    QPainter painter(this);

    pen.setWidth(1);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawLine(0, 20, 204, 20);
    painter.drawRect(0, 0, 204, 67);
}
