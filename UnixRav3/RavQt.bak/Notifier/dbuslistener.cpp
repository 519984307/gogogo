#include "dbuslistener.h"
#include "json/json.h"
#include "commonapi.h"
#include "communicatebundle.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include  <QtDBus/QtDBus>

#define MAX_PACKET 1024

#define CLIENT_SEND_SOURCE  "client.signal.source"
#define CLIENT_SERV_SOURCE  "client.method.source"
#define CLIENT_OBJECT_NAME  "/rav/signal/Object"
#define CLIENT_SIGNAL_TYPE  "rav.signal.Type"
#define CLIENT_METHOD_TYPE  "rav.method.Type"
#define SERVICE_INTERFACE   "com.rising.antivirus.interface"
#define REMOTE_SIGNAL_NAME  "SendSignal"
#define REMOTE_METHOD_NAME  "RemoteCall"

//
#ifdef COMM_WITH_DBUS
DBusListener::DBusListener(DBusObserver *observer, int type, bool systemType) {
    mObserver = observer;

    mStatus = 0;
    mType = type;
    mSystemType = systemType;

    mRavObject = NULL;
}

DBusListener::~DBusListener() {
    Stop();

    if (mRavObject) {
        delete mRavObject;
        mRavObject = NULL;
    }
}

DBusListener* DBusListener::NEW(DBusObserver *observer, int type, const char *sock, bool systemType) {
    DBusListener *self = new DBusListener(observer, type, systemType);
    if (self) {
        if (self->Construct(sock)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

void DBusListener::Start() {
    mStatus = 1;
    start();
}

void DBusListener::Stop() {
    if (mStatus > 0) {
        mStatus = 0;

        QDBusConnection connection = mSystemType?QDBusConnection::systemBus():QDBusConnection::sessionBus();

        connection.unregisterObject(CLIENT_OBJECT_NAME);
        connection.unregisterService(mDBusName);
        connection.disconnectFromBus(connection.name());
        QThread::quit();
    }
}

int DBusListener::Construct(const char *sock) {
    mDBusName = QString::fromUtf8(sock);
    return 0;
}


int DBusListener::InitDBus(const char *conn_name) {
#ifdef LOW_DBUS_API
    DBusError err;
    dbus_error_init(&err);

    mDBusConnection = dbus_bus_get(mSystemType?DBUS_BUS_SYSTEM:DBUS_BUS_SESSION , &err);
    if (dbus_error_is_set(&err)) {
        dbus_error_free(&err);
    }

    if (NULL == mDBusConnection) {
        return -1;
    }

    dbus_bus_register(mDBusConnection, &err);
    dbus_bus_set_unique_name(mDBusConnection, "rav.comm.dbus");

    // request our name on the bus and check for errors
    int ret = dbus_bus_request_name(mDBusConnection, conn_name, DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
    if (dbus_error_is_set(&err)) {
        dbus_error_free(&err);
    }
    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
        return -1;
    }
#endif

    return 0;
}


void DBusListener::run(void) {
    QDBusConnection connection = mSystemType?QDBusConnection::systemBus():QDBusConnection::sessionBus();
    if(!connection.registerService(mDBusName)) {
        return;
    }

    Utils::LOG::PrintLog(1, "type is %d and system name %s\n", mSystemType, mDBusName.toUtf8().data());

    if (mRavObject) {
        delete mRavObject;
    }
    mRavObject = new RavDBusObject(mObserver);
    connection.registerObject(CLIENT_OBJECT_NAME, mRavObject, QDBusConnection::ExportAllSlots);

    exec();
}

int DBusListener::SendDBusSignal(int method, const char *data, bool toSystem) {
#ifdef LOW_DBUS_API
//    this block use libdbus low level function
//    error is listen is root , can't receive signal
//
    // initialise the error value
    DBusError err;
    dbus_error_init(&err);

    // connect to the DBUS system bus, and check for errors
    DBusConnection *conn = dbus_bus_get(toSystem?DBUS_BUS_STARTER:DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        dbus_error_free(&err);
    }
    if (NULL == conn) {
        return -1;
    }

    // register our name on the bus, and check for errors
    char tmpName[128] = {0};
    snprintf(tmpName, sizeof(tmpName), "%s%ld", CLIENT_SEND_SOURCE, Utils::Date::GetTimeStamp());
    int ret = dbus_bus_request_name(conn, tmpName, DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
    if (dbus_error_is_set(&err)) {
        dbus_error_free(&err);
    }

    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
        return -1;
    }

    // create a signal & check for errors
    DBusMessage *msg = dbus_message_new_signal(CLIENT_OBJECT_NAME,   // object name of the signal
                                                CLIENT_SIGNAL_TYPE,  // interface name of the signal
                                                CLIENT_SIGNAL_NAME); // name of the signal
    if (NULL == msg) {
        return -1;
    }

    // append arguments onto signal
    DBusMessageIter args;
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &method)) {
        dbus_message_unref(msg);
        dbus_bus_release_name(conn, CLIENT_SEND_SOURCE, &err);

        return -1;
    }

    if (dbus_validate_utf8(data, NULL)) {
        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &data)) {
            dbus_message_unref(msg);
            dbus_bus_release_name(conn, CLIENT_SEND_SOURCE, &err);
            return -1;
        }
    }

    // send the message and flush the connection
    dbus_uint32_t serial = 0;
    if (!dbus_connection_send(conn, msg, &serial)) {
        dbus_message_unref(msg);
        dbus_bus_release_name(conn, CLIENT_SEND_SOURCE, &err);

        return -1;
    }
    dbus_connection_flush(conn);

    dbus_message_unref(msg);
    dbus_bus_release_name(conn, CLIENT_SEND_SOURCE, &err);
#endif
    return 0;
}

int DBusListener::SendDBusMethod(const char *dbus, int method, const char *params, bool toSystem) {
    // QtDBus
    QDBusMessage message = QDBusMessage::createMethodCall(
                dbus,
                CLIENT_OBJECT_NAME,
                SERVICE_INTERFACE,
                REMOTE_METHOD_NAME);

    message << method;
    if (NULL != params) {
        message << params;
    }

    QDBusMessage response = (toSystem?QDBusConnection::systemBus():QDBusConnection::sessionBus()).call(message);
    if (response.type() == QDBusMessage::ReplyMessage) {
        return response.arguments().takeFirst().toInt();
    }

#ifdef LOW_DBUS_API
    // this block use libdbus low level function
    // error is listen is root , can't receive signal
    // initialiset the errors
    DBusError err;
    dbus_error_init(&err);

    // connect to the system bus and check for errors
    DBusConnection* conn = dbus_bus_get(toSystem?DBUS_BUS_STARTER:DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        dbus_error_free(&err);
    }
    if (NULL == conn) {
        return -1;
    }

    // request our name on the bus
    char tmpName[128] = {0};
    snprintf(tmpName, sizeof(tmpName), "%s%ld", CLIENT_SERV_SOURCE, Utils::Date::GetTimeStamp());
    int ret = dbus_bus_request_name(conn, tmpName, DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
    if (dbus_error_is_set(&err)) {
        dbus_error_free(&err);
    }
    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
        return -1;
    }

    // create a new method call and check for errors
     DBusMessage* msg = dbus_message_new_method_call(
                            dbus,               // target for the method call
                            CLIENT_OBJECT_NAME, // object to call on
                            CLIENT_METHOD_TYPE, // interface to call on
                            CLIENT_METHOD_NAME); // method name
    if (NULL == msg) {
        return -1;
    }

    // append arguments
    DBusMessageIter args;
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &method)) {
        return -1;
    }
    if (NULL != params) {
        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &params)) {
            return -1;
        }
    }

    // send message and get a handle for a reply
    DBusPendingCall* pending;
    if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) { // -1 is default timeout
        return -1;
    }
    if (NULL == pending) {
        return -1;
    }
    dbus_connection_flush(conn);
    dbus_message_unref(msg);

    // block until we recieve a reply
    dbus_pending_call_block(pending);
    msg = dbus_pending_call_steal_reply(pending);
    if (NULL == msg) {
        return -1;
    }
    // free the pending message handle
    dbus_pending_call_unref(pending);

    // read the parameters
    if (!dbus_message_iter_init(msg, &args)) {
        return -1;
    }

    do  {
        if (DBUS_TYPE_UINT32 == dbus_message_iter_get_arg_type(&args)) {
            dbus_message_iter_get_basic(&args, &back);
        }
        else if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&args)) {
            char *p = NULL;
            dbus_message_iter_get_basic(&args, &p);
            if (NULL != p && NULL != data) {
                strcpy(data, p);
            }
        }
    }while(dbus_message_iter_next(&args));

    // free reply
    dbus_message_unref(msg);
    dbus_bus_release_name(conn, CLIENT_SERV_SOURCE, &err);
#endif
    return -1;
}

#ifdef LOW_DBUS_API
int DBusListener::GetMethodParams(DBusMessage *msg, int &method, char *data, int max_len) {
    DBusMessageIter arg;
    if(!dbus_message_iter_init(msg, &arg)) {
        return -1;
    }

    do {
        if(dbus_message_iter_get_arg_type(&arg) == DBUS_TYPE_STRING) {
            char *temp = NULL;
            dbus_message_iter_get_basic(&arg, &temp);
            if (NULL != temp) {
                strncpy(data, temp, max_len-1);
            }
        }
        else if(dbus_message_iter_get_arg_type(&arg) == DBUS_TYPE_UINT32) {
            dbus_message_iter_get_basic(&arg, &method);
        }
    } while(dbus_message_iter_next(&arg));
    return 0;
}

int DBusListener::ReplyMethodCall(DBusMessage * msg, DBusConnection * conn, int state, const char *data) {
    //创建返回消息reply
    DBusMessageIter arg;
    if(!dbus_message_iter_init(msg, &arg)) {
        return -1;
    }

    DBusMessage *reply = dbus_message_new_method_return(msg);
    dbus_message_iter_init_append(reply, &arg);

    if(!dbus_message_iter_append_basic(&arg, DBUS_TYPE_UINT32, &state)){
        return -1;
    }
    if (NULL != data) {
        if (!dbus_message_iter_append_basic(&arg, DBUS_TYPE_STRING, &data)) {
            return -1;
        }
    }

    //发送返回消息
    uint32_t serial = 0;
    if( !dbus_connection_send(conn, reply, &serial)){
        return -1;
    }
    dbus_connection_flush (conn);
    dbus_message_unref (reply);
    return 0;
}
#endif
#endif
