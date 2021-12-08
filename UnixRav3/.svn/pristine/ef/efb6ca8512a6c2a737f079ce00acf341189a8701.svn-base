#include <QApplication>
#include <QTextCodec>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/file.h>
#include <errno.h>
#include <sys/time.h>
#include <algorithm>
#include <signal.h>
#include <execinfo.h>
#include <QFontDatabase>

#include "../BaseController/application.h"
#include "../BaseController/baseconfig.h"
#include "../CommonAPI/circlelink.h"
#include "../CommonAPI/handler.h"
#include "../CommonAPI/commonapi.h"
#include "../CommonAPI/communicatebundle.h"
#include "mainwindow.h"

class Controller;
class MainWindow;

enum _running_mode {
    RUNNING_MODE_POWER_ON = 1,
    RUNNING_MODE_DAEMON,
    RUNNING_MODE_UNINSTALL,
    RUNNING_MODE_KILL,
    RUNNING_MODE_DEBUG
};

int g_running_state = 0;
int g_running_mode = 0;
int g_enter_function = ENTER_FUNCTION_STAGE_VIRUS_DETAIL;

int MainFunc(QApplication &a, int function_stage)
{
    cout << "MainFunc function_stage=" << function_stage << endl;
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
//    int fontId = QFontDatabase::addApplicationFont(":/resource/simsun.ttf");
//    QString msyh = QFontDatabase::applicationFontFamilies(fontId ).at(0);
//   qDebug("xxxxxx font fontID=%d", fontId);

    QString family;
    QFontDatabase database;
    foreach(family, database.families(QFontDatabase::SimplifiedChinese))
    {
        qDebug("found:%s ", family.toStdString().c_str());
        if(family.contains("song", Qt::CaseInsensitive))
            break;
    }
    QFont font(family, 10);
    a.setFont(font);
#else //QT_VERSION
#ifndef LOW_LINUX_VERSION
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QTextCodec::setCodecForLocale(codec);
    int fontId = QFontDatabase::addApplicationFont(":/resource/simsun.ttf");
    QString appFont = QFontDatabase::applicationFontFamilies(fontId).at(0);
    qDebug("xxxxxx font name=%s fontID=%d", appFont.toStdString().c_str(), fontId);
    QFont font(appFont, 9);
    a.setFont(font);
#endif
#endif //QT_VERSION
    cout << "g_enter_function=" << g_enter_function << endl;
    MainWindow *w = new MainWindow(function_stage);
    w->InitAllScreenData();
    w->show();

    int nret = a.exec();
    delete w;
    return nret;
}
extern char *optarg;
int parse_args(char argc, char *argv[]) {
    int opt = 0;
    while ((opt = getopt(argc, argv, "t:rhg")) != -1) {
        switch (opt) {
        case 't':
            g_enter_function = atoi(optarg);
            cout << "optarg=" << optarg << endl;
            if (g_enter_function < ENTER_FUNCTION_STAGE_VIRUS_DETAIL || g_enter_function > ENTER_FUNCTION_STAGE_PLATFORM){
                g_enter_function = ENTER_FUNCTION_STAGE_VIRUS_DETAIL;
                cout << "Error, parse_args() fail" << endl;
                return -1;
            }
            break;
        case 'r':
            g_running_mode = RUNNING_MODE_POWER_ON;
            break;
        case 'g':
            g_running_mode = RUNNING_MODE_DEBUG;
            break;
        case 'h': {
            cout << "Format: LogViewer -[d|r|h|g] -v version" << endl;
            cout << "g: debug it, no fork" << endl;
            cout << "k: kill all proc" << endl;
            cout << "h: help info" << endl;
            return -1;
        }
        default:
            g_running_mode = RUNNING_MODE_POWER_ON;
            break;
        }
    }
    return 0;
}


int main(int argc, char *argv[]) {
    int ret = -1;

    g_running_mode = RUNNING_MODE_POWER_ON;
    int err = parse_args(argc, argv);
    if (err) {
        return err;
    }

    switch (g_running_mode) {
    case RUNNING_MODE_POWER_ON: {
        pid_t pid;
        if ((pid=fork()) == 0) {
            QApplication a(argc, argv);
            ret = MainFunc(a, g_enter_function);
        }
        break;
    }
    case RUNNING_MODE_DEBUG: {
        QApplication a(argc, argv);
        ret = MainFunc(a, g_enter_function);
    }
    default:
        break;
    }
    return ret;
}
