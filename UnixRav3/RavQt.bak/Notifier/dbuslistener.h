#ifndef DBUSLISTENER_H
#define DBUSLISTENER_H

#include <linux/socket.h>
#include <linux/netlink.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <list>
#include <string>

#include "rserializer.h"
#include "communicatebundle.h"


class DBusObserver {
public:
    enum {
        PUBLIC_API_SCAN,
        CLIENT_COMMAND,
        CLIENT_SIGNAL
    };
    virtual int OnDBusObserver(int event, void *args, void **result) = 0;
};

#ifdef COMM_WITH_DBUS
#include  <QtCore/QObject>
#include  <QThread>

class RavDBusObject : public QObject {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.rising.antivirus.interface")

public:
    RavDBusObject(DBusObserver *observer) {
        mObserver = observer;
    }

public slots:
    int RemoteCall(int argi, const QString& args) {
        Utils::LOG::PrintLog(1, "get in dbus event");

        CommandBundle bundle;
        bundle.mCommandID = argi;
        bundle.mStringValue = args.toStdString();

        Utils::LOG::PrintLog(1, "get info is %d -- data is %s\n", argi, args.toUtf8().data());

        if (mObserver) {
            return mObserver->OnDBusObserver(DBusObserver::CLIENT_COMMAND, &bundle, NULL);
        }
        return -1;
    }
    int SendSignal(int argi, const QString& args) {
        CommandBundle bundle;
        bundle.mCommandID = argi;
        bundle.mStringValue = args.toStdString();

        if (mObserver) {
            return mObserver->OnDBusObserver(DBusObserver::CLIENT_SIGNAL, &bundle, NULL);
        }
        return -1;
    }

private:
    DBusObserver *mObserver;
};


class DBusListener : public QThread
{
    Q_OBJECT
public:
    static DBusListener* NEW(DBusObserver *observer,
                               int/*DBusObserver*/ type,
                               const char *sock,
                               bool systemType=false);
    ~DBusListener();

    void Start();
    void Stop();

    static int SendDBusSignal(int method, const char *data, bool toSystem=false);
    static int SendDBusMethod(const char *dbus, int method, const char *params, bool toSystem=false);

private:
    DBusListener(DBusObserver *observer,  int type, bool systemType);
    int Construct(const char *sock);

    int InitDBus(const char *conn_name);

#ifdef LOW_DBUS_API
    int GetMethodParams(DBusMessage *msg, int &method, char *data, int max_len);
    int ReplyMethodCall(DBusMessage * msg, DBusConnection * conn, int state, const char *data);
#endif

private:
    RavDBusObject    *mRavObject;


private:
    void run(void);

private:
    int mStatus;

private:
    DBusObserver   *mObserver;
    int     mType;
    bool    mSystemType;
    bool    mQtApp;

    // dbus name
    QString mDBusName;

    // for file monitor respond path
    QString mTmpPath;
};
#endif
#endif // DBUSLISTENER_H
