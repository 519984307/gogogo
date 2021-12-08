#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "screen.h"
#include "baseconfig.h"
#include "commonapi.h"

#ifdef COMM_WITH_DBUS
    #include <QCoreApplication>
#endif

int main(int argc, char *argv[]) {
#ifdef COMM_WITH_DBUS
    QCoreApplication app(argc, argv);
    Q_UNUSED(app);
#endif
    signal(SIGUSR2, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    sigset_t signal_mask;
    sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGPIPE);
    int rc = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);
    if (rc != 0) {
        printf("block sigpipe error\n");
    }


    Screen *screen = Screen::NEW();
    if (!screen) {
        printf("Something wrong, please contact RISING\n");
    }
    else {
        screen->Show();
        delete screen;
    }
    system("clear");
    return 0;
}
