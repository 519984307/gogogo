#include <QApplication>
#include <QTextCodec>
#include <QDesktopWidget>
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
#include <unistd.h>
#include <sys/types.h>

#include "mainwindow.h"

#include "../BaseController/application.h"
#include "../BaseController/baseconfig.h"

#include "../CommonAPI/circlelink.h"
#include "../CommonAPI/handler.h"
#include "../CommonAPI/commonapi.h"
#include "../CommonAPI/communicatebundle.h"
#include "config.ver"


int g_running_state = 0;
int g_running_mode = 0;


enum _running_mode {
    RUNNING_MODE_POWER_ON = 1,
    RUNNING_MODE_DAEMON,
    RUNNING_MODE_UNINSTALL,
    RUNNING_MODE_KILL,
    RUNNING_MODE_DEBUG
};


//void exit_signal(int ) {
//    g_running_state = 0;
//#ifdef DEBUG
//    printf("catch signal %d\n", signal);
//#endif
//}


int MainFunc(QApplication &a)
{
//    signal(SIGUSR2, exit_signal);
//    char csLockFile[1024];
//    string sLockFile = Utils::Path::JoinPath(Utils::Path::GetInstallPath().c_str(), FOLDER_RJJH_ETC, LOCK_RJJH_SETTING, NULL);
//    sprintf(csLockFile, sLockFile.c_str(), getuid());
//    FILE* fd = fopen(csLockFile, "wb+");
//    if(fd == NULL){
//        cout << "Error, Open process lock file fail,then quit!" << endl;
//        return -1;
//    }
//    if(flock(fd->_fileno, LOCK_EX|LOCK_NB) != 0){
//        cout << "Error, Lock process file fail, then quit!" << endl;
//        return -1;
//    }

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

    MainWindow *w = new MainWindow;
    w->UpdateFromConfig();

    w->move((a.desktop()->width()-w->width())/2, (a.desktop()->height()- w->height())/2);

    w->show();
    int nret = a.exec();

    delete w;

//    flock(fd->_fileno, LOCK_UN);
//    fclose(fd);
    return nret;
}


//void _backtrace(int eid) {
//    void *pTrace[256];
//    char **ppszMsg = NULL;
//    size_t uTraceSize = 0;
//    static const char szSigMsg[][256] = {
//        "Received SIGSEGV",
//        "Received SIGPIPE",
//        "Received SIGFPE",
//        "Received SIGABRT"
//    };

//    do {
//        if (0 == (uTraceSize = backtrace(pTrace, sizeof(pTrace) / sizeof(void *)))) {
//            break;

//        }

//        if (NULL == (ppszMsg = backtrace_symbols(pTrace, uTraceSize))) {
//            break;

//        }

//        Utils::LOG::PrintLog("%s. call stack:\n", szSigMsg[eid]);
//        for (size_t i = 0; i < uTraceSize; ++i) {
//            Utils::LOG::PrintLog("%s. call stack:\n", ppszMsg[i]);
//        }
//    } while (0);

//    if (NULL != ppszMsg) {
//        free(ppszMsg);
//        ppszMsg = NULL;
//    }
//}

//void _signal_handler(int signo) {
//    _backtrace(signo);
//    exit(0);
//}

int parse_args(char argc, char *argv[]) {
    int opt = 0;
    while ((opt = getopt(argc, argv, "hg")) != -1) {
        switch (opt) {
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
            break;
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

//    signal(SIGPIPE, _signal_handler);    // SIGPIPE，管道破裂。
//    signal(SIGSEGV, _signal_handler);    // SIGSEGV，非法内存访问
//    signal(SIGFPE, _signal_handler);       // SIGFPE，数学相关的异常，如被0除，浮点溢出，等等
//    signal(SIGABRT, _signal_handler);     // SIGABRT，由调用abort函数产生，进程非正常退出

    switch (g_running_mode) {
    case RUNNING_MODE_POWER_ON: {
        pid_t pid;
        if ((pid=fork()) == 0) {
            QApplication a(argc, argv);
            ret = MainFunc(a);
        }
        break;
    }
    case RUNNING_MODE_DEBUG: {
        QApplication a(argc, argv);
        ret = MainFunc(a);
    }
    default:
        break;
    }
    return ret;

}
