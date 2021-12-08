#ifndef SETUP_GLOBAL_H
#define SETUP_GLOBAL_H

#include <stdint.h>
#include <sys/types.h>
#include <string>

#ifndef u8
#define u8 
#endif

struct SETUPTOC
{
    char strFile[256];	//文件名
    uint32_t  nSize;			// 大小
    uint16_t  mode;		// 打包文件的属性
    char szOrigMd5[40]; //打包文件的原始签名
    char szZipMd5[40];  // 打包文件压缩之后的签名
};

// 为了与老版本兼容，该结构只能在首部增加成员
struct SETUPTAIL
{
    char strVersion[64];        // 当前版本
    char strPackedtime[20];     // 打包时间

    char strNetRegFile[256];	//网络版注册文件名
    char strSingleRegFile[256];	//单机版注册文件名
    char strSignature[64];		//rising安装包标志字符串
    uint32_t  nSize;					// 包的 大小
    uint32_t  nFileNum;				// 包中的文件数目
    uint32_t  nTOCOffSet;			// 有效文件开始位置（除掉第一个用于解压的文件）
};


//NewStart 新支点  iSoft 普华 Emind 一铭 rockyf6 凝思磐石  EulerOS 华为  EulerOS  统一 uos
// /etc/os-release
const std::string RavGuoChanOsFlag[] = {"longd", "NeoKylin", "YHKylin", "Kylin", "Deepin", "Redflag", "Linx", "NewStart", "iSoft", "Emind", "EulerOS", "uos", "UOS", "Uniontech OS"};
const int RavGuoChanOSFlagCount = 14;
// /etc/issue
const std::string RavGuoChanIssueFlag[] = {"NeoKylin", "Kylin", "Deepin", "NewStart", "Linx", "iSoft", "Emind"};
const int RavGuoChanIssueFlagCount = 7;

// uname -n
const std::string RavGuoChanNodeName[] = {"rockyf6"};
const int RavGuoChanNodeNameCount = 1;



#endif // SETUP_GLOBAL_H
