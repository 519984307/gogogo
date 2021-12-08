#include "screenwidget.h"
#include "baseconfig.h"
#include "screenwidget.h"
#include "appconfig.h"

#include <QApplication>
#include <QEventLoop>

#ifndef WITH_QT
extern "C"  {
#endif

    static pthread_t g_qt_thread;
    static void *g_screen_widget = NULL;
    static bool g_tray_show_state = true;

#ifndef WITH_QT
    QApplication *g_qt_core;
#endif

    void* start_qt_env(void *params) {
        LOG_RJJH("start the QT thread");
#ifndef WITH_QT
        int argc = 1;
        char *argv[] = {"temp", NULL};
        g_qt_core = new QApplication(argc, argv);
#endif
        LOG_RJJH("qt application created");

        g_screen_widget = new ScreenWidget((AppConfig*)params);
        ((ScreenWidget*)g_screen_widget)->Start(g_tray_show_state);

        LOG_RJJH("screen widget creaged");

#ifndef WITH_QT
        g_qt_core->exec();
#endif
        return NULL;
    }

    void initialize_widget(bool tray, AppConfig *config) {
        g_tray_show_state = tray;
        pthread_create(&g_qt_thread, NULL, start_qt_env, config);
    }

    void unload_widget() {
        if (g_screen_widget) {
            delete (ScreenWidget*)g_screen_widget;
            g_screen_widget = NULL;
        }
        // 没啥作用, 直接崩溃, 待解决BUG
        QApplication::exit(0);
    }

    void show_notify(CommunicateBundle *bundle) {
        if (g_screen_widget) {
            ((ScreenWidget*)g_screen_widget)->ShowTips(*bundle);
            bundle->mSelected = ((ScreenWidget*)g_screen_widget)->GetSelected();
        }
    }

    void set_tray_icon(bool state) {
        if (g_screen_widget) {
            ((ScreenWidget*)g_screen_widget)->SetTrayIcon(state);
        }
    }
#ifndef WITH_QT
}
#endif


