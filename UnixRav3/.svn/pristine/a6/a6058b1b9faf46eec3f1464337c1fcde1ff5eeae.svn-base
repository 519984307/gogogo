#ifndef __CFGDEF2_H__
#define __CFGDEF2_H__

#include "RSTYPE.H"

//调试信息的输出方式
//#define LOGTODBGVIEW		//记录日志到DBGView中
//#define LOGTOFILE			//记录日志到文件中
//#define LOGERRORMSG		//是否输出错误信息
//#define FOR_TEST			//此参数用来增加2个接口，供测试程序使用，正式出品不使用此宏
//#define FOR_CFG_EDITOR	//为编辑器增加的兼容老版本
//#define FOR_INNER_MODIFY	//内部使用的修改某项的值
//#define FOR_INNER_INSTALL	//内部使用的已安装标志
//#define FOR_INNER_TRACE   //内部调试函数调用
#ifdef LOGTODBGVIEW
        #define MyTrace InnerTrace
#else
        #ifdef LOGTOFILE
                #define MyTrace InnerTrace
        #else
                #define MyTrace
        #endif
#endif

#ifdef LOGTODBGVIEW
        #ifndef LOGTODBGVIEW_REAL
                #define LOGTODBGVIEW_REAL
        #endif
#endif

#ifdef LOGERRORMSG
        #ifndef LOGTODBGVIEW_REAL
                #define LOGTODBGVIEW_REAL
        #endif
#endif

#ifdef	RS_BIG_ENDIAN
#define	RS_SWAPDWORDEX(x)			x=RS_SWAPDWORD(x)
#define	RS_SWAPWORDEX(x)			x=RS_SWAPWORD(x)
#else
#define	RS_SWAPDWORDEX
#define	RS_SWAPWORDEX
#endif

//Unix需要重定义的类型和函数
#ifdef UNIXPLATFORM
typedef unsigned int DWORD;
typedef unsigned char UCHAR;
typedef  char CHAR;
typedef  const char*  LPCSTR;
typedef  char*  LPCTSTR;
typedef  unsigned int HKEY;
typedef long LONG;
typedef int BOOL;
typedef int INT ;

#define Sleep usleep
#define _MAX_PATH			512
#define HKEY_CLASSES_ROOT	0
#define HKEY_LOCAL_MACHINE	0
#define ERROR_SUCCESS		0
#define lstrcmpA			strcmp
#endif

#ifdef WIN32
#ifndef __FUNCTION__		//The GNU compiler and VC++ 7.x supports this macro
#define __FUNCTION__ ""		//If compiler does not support it, then use empty string
#endif //!__FUNCTION__
#endif //WIN32

// 定义用户数量和用户名称
//#define USER_NUMBER				0x00000004		// 系统中使用的用户数量
//#define ACCESS_NUMBER				0x0000000b		// 权限的种类

#define RS_MAX_PATH					0x00000400		//最大路径长度
#define RS_USERID_LEN				0x00000004		//标识用户ID的长度
#define RS_MAX_CFGPATH_LEN			0x00000041		//最大路径长度，即树的层数
#define INVALID_ITEMID				0x00000000		//无效的ID值
#define CONFIG_ACCESS_SUPER			0x80000000		//超级权限
#define PRODUCT_TOTAL_NUM			0x00000020		//产品数量


//配置的访问模式定义
#define CONFIG_ACCESS_MODE_NAME		0x00000001		// 名字模式
#define CONFIG_ACCESS_MODE_ID		0x00000002		// ID 模式

//树的装载模式
#define LOAD_MODE_INIT				0x00000000		//初始化模式
#define LOAD_MODE_ADD				0x00000001		//追加模式

//Mutex类型
#define MUTEX_WRITE_FILE			0x00000001		//写文件
#define MUTEX_WRITE_MEM				0x00000002		//写内存
#define MUTEX_IMPORT_FILE			0x00000003		//导入文件

#define MYNEW(x,y)		(x*)CSharedMem::alloc(sizeof(x)*y)
#define MYDELETE(x)		CSharedMem::free(x)

//树的排序方式
enum ENUMTREEORDERTYPE
{
        ORDER_NO = 0,
        ORDER_ASCENDING,
        ORDER_DESCENDING
};

enum ENUMDEMOSTATUS
{
        DEMOSTATUS_ERR	=	0,		//读注册表出现错误
        DEMOSTATUS_NOT	=	1,		//不是试用版。
        DEMOSTATUS_USE	=	2,		//在试用期内，而且不在警告期内，可以正常使用。
        DEMOSTATUS_TIP	=	3,		//在最后警告期内，可以使用，但应提示购买。
        DEMOSTATUS_DIE	=	4,		//超过了最后期限，停止使用。
};

typedef RS_INT (*fnCompressDef)(RS_BYTE*, RS_DWORD*, RS_BYTE*, RS_DWORD);
typedef RS_INT (*fnUncompressDef)(RS_BYTE*, RS_DWORD*, RS_BYTE*, RS_DWORD);
enum ENUMZIPERRORS
{
        Z_VERSION_ERROR = -6,
        Z_BUF_ERROR		= -5,
        Z_MEM_ERROR		= -4,
        Z_DATA_ERROR	= -3,
        Z_STREAM_ERROR	= -2,
        Z_ERRNO			= -1,
        Z_OK			= 0,
        Z_STREAM_END	= 1,
        Z_NEED_DICT		= 2,
};
#endif	// __CFGDEF2_H__
