#ifndef PUBDEF_H
#define PUBDEF_H

#include <QString>

#define APP_SCREEN_WIDTH    820
#define APP_SCREEN_HEIGHT   520

#define APP_SCREEN_LEFT_WIDTH   180

#define APP_SCREEN_RIGHT_BOTTOM_HEIGHT  80


enum{
    APP_MODE_FAST = 0,
    APP_MODE_STANDARD,
    APP_MODE_PROFESSIONAL,
    APP_MODE_PLUS
};

enum{
    APP_FILE_ALL = 0,
    APP_FILE_PROGDOC,
    APP_FILE_USER_DEFINE
};

enum{
    APP_FOUNDV_AUTO = 0,
    APP_FOUNDV_MANUAL,
    APP_FOUNDV_IGNORE
};
enum{
    APP_ENABLE_RUN = 0,
    APP_DISABLE_RUN
};

enum{
    APP_ASK_ME = 0,
    APP_DELETE_FILE,
    APP_DONT_CARE,
    APP_PROC_IMMEDIATELY,
    APP_PROC_AUTO
};

enum{
    APP_AUTO_REPLACE = 0,
    APP_AUTO_INCREASE
};

enum{
    APP_LOW = 0,
    APP_MIDDLE,
    APP_HIGH
};

enum{
    APP_UPGRADE_ALL = 0,
    APP_UPGRADE_VIRUSDB
};

enum{APP_MENU_DIRSEL_TYPE_DIR_CHILD = 0,
     APP_MENU_DIRSEL_TYPE_DIR,
     APP_MENU_DIRSEL_TYPE_CHILD,
     APP_MENU_DIRSEL_TYPE_FILE
};

#ifndef u8
#define u8
#endif

#ifndef nullptr
#define nullptr 0
#endif


#define SECURITY_FREE(x) if(x) {delete x; x=NULL;}
#define SECURITY_CLOSE(x) if(x) {fclose(x); x=NULL;}
#define SECURITY_CHECK(x, y, z) if(x != y) return z;
#define ASSERT_ZERO(x, y) if(!x) return y;

const QString APP_MENU_WHITELIST_TYPE[] = {
    u8"本目录和子目录",
    u8"本目录",
    u8"子目录",
    u8"文件"
};

#endif // PUBDEF_H
