#include "config.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <glib/gi18n-lib.h>
#include <gio/gio.h>
#include <libcaja-extension/caja-extension-types.h>
#include <libcaja-extension/caja-file-info.h>
#include <libcaja-extension/caja-menu-provider.h>

#include "caja-rav.h"
#include "baseconfig.h"

#define COMM_WITH_PROCESS
#ifdef COMM_WITH_DBUS
//    #include <QtDBus/QDBusMessage>
//    #include <QtDBus/QDBusConnection>
    #include "dbus/dbus.h"
#endif


#define INSTALL_PATH "/opt/BDFZ/RAV"


static GObjectClass *parent_class;

//#define DEBUG

#ifdef DEBUG
void lograv(const char *fmt, ...) {
    char acTitle[1024] = { 0 };
    int iLen = 0;
    FILE *file = fopen("/tmp/caja_rav.log", "a+");
    if (file) 
{
        va_list args;
        va_start(args, fmt);
        vsnprintf(acTitle, sizeof(acTitle), fmt, args);
        va_end(args);
        iLen = strlen(acTitle);
        fwrite(acTitle, iLen, 1, file);
        fwrite("\n", 1, 1, file);
        fflush(file);
        fclose(file);
    }
}
#endif

int trimblank(char *szStr,int nMode)
{
    int nLen,i,j,nFlag;
    char szTempBuf[512];

    if(szStr==NULL)return -1;
    nLen=strlen(szStr);
    if(nLen==0 || nLen >511)return -1;
    strcpy(szTempBuf,szStr);
    j=0;

    //trim right blank
    if(nMode==1 || nMode==2) {
        for(i=nLen-1;i>=0;i--) {
            if(szTempBuf[i]==' ' || szTempBuf[i]=='\t' || szTempBuf[i]==0x0a || szTempBuf[i]==0x0d) {
                szTempBuf[i]='\0';
            }
            else {
                break;
            }
        }
    }

    nLen=strlen(szTempBuf);
    if (nLen==0) {
        szStr[0]='\0';
        return 0;
    }

    //trim left
    nFlag=0;
    j=0;
    for(i=0;i<=nLen;i++) {
        if((szTempBuf[i]==' ' ||szTempBuf[i]=='\t' || szTempBuf[i]=='\n') && nFlag==0) {
            continue;
        }
        else {
            nFlag=1;
            szStr[j++]=szTempBuf[i];
        }
    }
    return 0;
}

int check_process_state(const char *process) {
#if 0
    FILE *fpPipe=NULL;
    char szProcess[512];
    pid_t pnPid[10];

    sprintf(szProcess,"ps -ef|grep %s", process);

    if((fpPipe=popen(szProcess,"r"))==NULL)
        return -1;

    int nTot=0;

    char szBuf[512];
    char szGrepFlag[512];
    sprintf(szGrepFlag,"grep %s", process);

    while(fgets(szBuf,sizeof(szBuf)-1,fpPipe)!=NULL)
    {
        trimblank(szBuf,2);
        if( !strstr(szBuf,process) || strstr(szBuf,szGrepFlag) )
            continue;

        char *szSavePtr;
        char *token;
        char *szStr=szBuf;
        char szPid[256],szTmpCmd[256];
        memset(szPid,0,sizeof(szPid));
        memset(szTmpCmd,0,sizeof(szTmpCmd));

        int nItemsPid=1;
        int nCmdItems=7;
        int i;
        for(i=0;i<=nCmdItems;i++,szStr=NULL)
        {
            token=strtok_r(szStr," \t",&szSavePtr);
            if(!token)
                break;
            if(i==nItemsPid)
            {
                strncpy(szPid,token,sizeof(szPid)-1);
            }
            else if(i==nCmdItems)
                strncpy(szTmpCmd,token,sizeof(szTmpCmd)-1);
        }
        if(szPid[0]=='\0' || szTmpCmd[0]=='\0')
            continue;

        char *endptr;
        long int pid=strtol(szPid,&endptr,10);
        if( (endptr && *endptr!=0)|| pid<=0)
            continue;

        pnPid[nTot++]=pid;
        if(nTot >= 10)
            break;
    }
    pclose(fpPipe);
    return nTot;
#endif
    return 0;
}

void join_path(char *out, const char* path, ...) {
    strcpy(out, path);
    if (out[strlen(out)-1] == '/') {
        out[strlen(out)-1] = '\0';
    }

    va_list args;
    va_start(args, path);
    while(1) {
        const char* one = va_arg(args, const char*);
        if (one == NULL) {
            break;
        }

        if (strlen(one) == 0) {
            continue;
        }
        if (one[0] != '/') {
            strcat(out, "/");
        }
        strcat(out, one);
    }
}

int send_scan_command(const char *domain, const char *filepath) {
#if defined(COMM_WITH_SOCKET)
    int ret;
    int socket_fd;

    static struct sockaddr_un srv_addr;

    socket_fd = socket(AF_UNIX,SOCK_STREAM,0);
    if(socket_fd < 0) {
        return 1;
    }
    srv_addr.sun_family = AF_UNIX;
    strcpy(srv_addr.sun_path,domain);

    ret = connect(socket_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
    if(ret == -1) {
        close(socket_fd);
        return 2;
    }

    // the send data format
    // declear in CommonAPI, rserializer.h
    int temp = 1; //string
    write(socket_fd, &temp, sizeof(int));

    // write size
    temp = strlen(filepath) + sizeof(int);
    write(socket_fd, &temp, sizeof(int));

    // write command id
    temp = 103; //CommandBundle::CONTEXT_SCAN;
    write(socket_fd, &temp, sizeof(int));

    // write data
    write(socket_fd, filepath, strlen(filepath));

    // read the result
    read(socket_fd, &temp, sizeof(int));

    close(socket_fd);
#elif defined(COMM_WITH_DBUS)
#if 0
    // QtDBus
    QDBusMessage message = QDBusMessage::createMethodCall(
                NOTIFY_DBUS_NAME,
                "/rav/signal/Object",
                "com.rising.antivirus.interface",
                "RemoteCall");

    message << 103; //CommandBundle::SCAN_FILE
    message << filepath;

    QDBusMessage response = QDBusConnection::sessionBus().call(message);
    if (response.type() == QDBusMessage::ReplyMessage) {
        return response.arguments().takeFirst().toInt();
    }
    return -1;
#endif

    DBusError err;
    dbus_error_init(&err);

    // connect to the system bus and check for errors
    DBusConnection* conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        dbus_error_free(&err);
    }
    if (NULL == conn) {
        return -1;
    }

    // request our name on the bus
    int ret = dbus_bus_request_name(conn, CONTEXT_MENU_DBUS, DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
    if (dbus_error_is_set(&err)) {
        dbus_error_free(&err);
    }
    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
        return -1;
    }

    // create a new method call and check for errors
     DBusMessage* msg = dbus_message_new_method_call(
                            NOTIFY_DBUS_NAME,               // target for the method call
                            "/rav/signal/Object", // object to call on
                            "com.rising.antivirus.interface", // interface to call on
                            "RemoteCall"); // method name
    if (NULL == msg) {
        return -1;
    }

    // append arguments
    int method = 103;
    DBusMessageIter args;
    dbus_message_iter_init_append(msg, &args);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &method);

    dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &filepath);

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

    int back = -1;
    if (DBUS_TYPE_INT32 == dbus_message_iter_get_arg_type(&args)) {
        dbus_message_iter_get_basic(&args, &back);
    }

    // free reply
    dbus_message_unref(msg);
    dbus_bus_release_name(conn, CONTEXT_MENU_DBUS, &err);
#elif defined(COMM_WITH_PROCESS)
    char temp[256];
    join_path(temp,  INSTALL_PATH, DEPLOYED_BIN_FOLDER, CONTEXT_DAEMON_ENTRY, NULL);

    char command[256];
    snprintf(command, 255, "%s -p \"%s\"", temp, filepath);

#ifdef DEBUG
    lograv("%s", command);
#endif

    system(command);
#endif


    return 0;
}


static struct {
    char     *mime_type;
    gboolean  is_compressed;
} archive_mime_types[] = {
        { "application/x-7z-compressed", TRUE },
        { "application/x-7z-compressed-tar", TRUE },
        { "application/x-ace", TRUE },
        { "application/x-alz", TRUE },
        { "application/x-ar", TRUE },
        { "application/x-arj", TRUE },
        { "application/x-bzip", TRUE },
        { "application/x-bzip-compressed-tar", TRUE },
        { "application/x-bzip1", TRUE },
        { "application/x-bzip1-compressed-tar", TRUE },
        { "application/vnd.ms-cab-compressed", TRUE },
        { "application/x-cbr", TRUE },
        { "application/x-cbz", TRUE },
        { "application/x-cd-image", FALSE },
        { "application/x-compress", TRUE },
        { "application/x-compressed-tar", TRUE },
        { "application/x-cpio", TRUE },
        { "application/x-deb", TRUE },
        { "application/x-ear", TRUE },
        { "application/x-ms-dos-executable", FALSE },
        { "application/x-gtar", FALSE },
        { "application/x-gzip", TRUE },
        { "application/x-gzpostscript", TRUE },
        { "application/x-java-archive", TRUE },
        { "application/x-lha", TRUE },
        { "application/x-lhz", TRUE },
        { "application/x-lzip", TRUE },
        { "application/x-lzip-compressed-tar", TRUE },
        { "application/x-lzma", TRUE },
        { "application/x-lzma-compressed-tar", TRUE },
        { "application/x-lzop", TRUE },
        { "application/x-lzop-compressed-tar", TRUE },
        { "application/x-ms-wim", TRUE },
        { "application/x-rar", TRUE },
        { "application/x-rar-compressed", TRUE },
        { "application/x-rpm", TRUE },
        { "application/x-rzip", TRUE },
        { "application/x-tar", FALSE },
        { "application/x-tarz", TRUE },
        { "application/x-stuffit", TRUE },
        { "application/x-war", TRUE },
        { "application/x-xz", TRUE },
        { "application/x-xz-compressed-tar", TRUE },
        { "application/x-zip", TRUE },
        { "application/x-zip-compressed", TRUE },
        { "application/x-zoo", TRUE },
        { "application/zip", TRUE },
        { "multipart/x-zip", TRUE },
        { NULL, FALSE }
};


typedef struct {
      gboolean is_archive;
      gboolean is_derived_archive;
      gboolean is_compressed_archive;
} FileMimeInfo;


static FileMimeInfo
get_file_mime_info (CajaFileInfo *file)
{
    FileMimeInfo file_mime_info;
    int          i;

    file_mime_info.is_archive = FALSE;
    file_mime_info.is_derived_archive = FALSE;
    file_mime_info.is_compressed_archive = FALSE;

    for (i = 0; archive_mime_types[i].mime_type != NULL; i++)
        if (caja_file_info_is_mime_type (file, archive_mime_types[i].mime_type)) {
            char *mime_type;
            char *content_type_mime_file;
            char *content_type_mime_compare;

            mime_type = caja_file_info_get_mime_type (file);

#ifdef DEBUG
            lograv("mime type is %s", mime_type);
#endif

            content_type_mime_file = g_content_type_from_mime_type (mime_type);
            content_type_mime_compare = g_content_type_from_mime_type (archive_mime_types[i].mime_type);

#ifdef DEBUG
            lograv("content type is %s", content_type_mime_file);
            lograv("content mime compare %s", content_type_mime_compare);
#endif

            file_mime_info.is_archive = TRUE;
            file_mime_info.is_compressed_archive = archive_mime_types[i].is_compressed;
            if ((content_type_mime_file != NULL) && (content_type_mime_compare != NULL))
                file_mime_info.is_derived_archive = ! g_content_type_equals (content_type_mime_file, content_type_mime_compare);

            g_free (mime_type);
            g_free (content_type_mime_file);
            g_free (content_type_mime_compare);

            return file_mime_info;
        }

    return file_mime_info;
}


static gboolean
unsupported_scheme (CajaFileInfo *file)
{
    gboolean  result = TRUE;
    GFile    *location;
    char     *scheme;

    location = caja_file_info_get_location (file);
    scheme = g_file_get_uri_scheme (location);

    if (scheme != NULL) {
        const char *unsupported[] = { "trash", "computer", NULL };
        const char *supported[] = { "file", NULL };
        int         i;

        for (i = 0; supported[i] != NULL; i++)
            if (strcmp (scheme, supported[i]) == 0)
                result = FALSE;
    }

    g_free (scheme);
    g_object_unref (location);

    return result;
}


static void
scanfile_callback (CajaMenuItem *item,
          gpointer          user_data)
{
    GList            *files, *scan;
    CajaFileInfo    *file;
    char             *uri, *dir;

    files = (GList*)g_object_get_data (G_OBJECT (item), "files");
    file = (CajaFileInfo*)files->data;

    uri = caja_file_info_get_uri (file);
    dir = g_path_get_dirname (uri);

    char temp[128] = {0};
    join_path(temp, INSTALL_PATH, FOLDER_RJJH_GJCZ, GJCZ_RJJH_SOCKET, NULL);

#ifdef DEBUG
    lograv("send %s to %s", uri, temp);
#endif

    int ret = send_scan_command(temp, uri);

#ifdef DEBUG
    lograv("send value is %d", ret);
#endif

    g_free (uri);
    g_free (dir);
}


static GList *
caja_fr_get_file_items (CajaMenuProvider *provider,
			    GtkWidget            *window,
			    GList                *files)
{
	GList    *items = NULL;
	GList    *scan;
    gboolean  can_write = TRUE;
    gboolean  one_item;
    gboolean  one_archive = FALSE;
    gboolean  one_derived_archive = FALSE;
    gboolean  one_compressed_archive = FALSE;
    gboolean  all_archives = TRUE;
    gboolean  all_archives_derived = TRUE;
    gboolean  all_archives_compressed = TRUE;


	if (files == NULL) {
        return NULL;
	}

    if (unsupported_scheme ((CajaFileInfo *) files->data)) {
        return NULL;
    }

    for (scan = files; scan; scan = scan->next) {
        CajaFileInfo *file = (CajaFileInfo*)scan->data;
        FileMimeInfo      file_mime_info;

        file_mime_info = get_file_mime_info (file);

        if (all_archives && ! file_mime_info.is_archive)
            all_archives = FALSE;

        if (all_archives_compressed && file_mime_info.is_archive && ! file_mime_info.is_compressed_archive)
            all_archives_compressed = FALSE;

        if (all_archives_derived && file_mime_info.is_archive && ! file_mime_info.is_derived_archive)
            all_archives_derived = FALSE;

        if (can_write) {
            CajaFileInfo *parent;

            parent = caja_file_info_get_parent_info (file);
            can_write = caja_file_info_can_write (parent);
            g_object_unref (parent);
        }
    }

    one_item = (files != NULL) && (files->next == NULL);
    one_archive = one_item && all_archives;
    one_derived_archive = one_archive && all_archives_derived;
    one_compressed_archive = one_archive && all_archives_compressed;

    if (one_item || one_archive)
    {
        CajaMenuItem *item;

        item = caja_menu_item_new ("CajaFr::scan_file",
                           "瑞星杀毒扫描",
                           "使用瑞星杀毒软件进行病毒扫描",
                           "drive-harddisk");

        g_signal_connect (item,
                  "activate",
                  G_CALLBACK (scanfile_callback),
                  provider);

        g_object_set_data_full (G_OBJECT (item),
                    "files",
                    caja_file_info_list_copy (files),
                    (GDestroyNotify) caja_file_info_list_free);

        items = g_list_append (items, item);
    }

	return items;
}


static void
caja_fr_menu_provider_iface_init (CajaMenuProviderIface *iface)
{
	iface->get_file_items = caja_fr_get_file_items;
}


static void
caja_fr_instance_init (CajaFr *fr)
{
}


static void
caja_fr_class_init (CajaFrClass *cls)
{
    parent_class = (GObjectClass*)g_type_class_peek_parent (cls);
}


static GType fr_type = 0;


GType
caja_fr_get_type (void)
{
	return fr_type;
}


void
caja_fr_register_type (GTypeModule *module)
{
	static const GTypeInfo info = {
		sizeof (CajaFrClass),
		(GBaseInitFunc) NULL,
		(GBaseFinalizeFunc) NULL,
		(GClassInitFunc) caja_fr_class_init,
		NULL,
		NULL,
		sizeof (CajaFr),
		0,
		(GInstanceInitFunc) caja_fr_instance_init,
	};

	static const GInterfaceInfo menu_provider_iface_info = {
		(GInterfaceInitFunc) caja_fr_menu_provider_iface_init,
		NULL,
		NULL
	};

	fr_type = g_type_module_register_type (module,
					       G_TYPE_OBJECT,
                           "CajaRav",
                           &info, (GTypeFlags)0);

	g_type_module_add_interface (module,
				     fr_type,
				     CAJA_TYPE_MENU_PROVIDER,
				     &menu_provider_iface_info);

}
