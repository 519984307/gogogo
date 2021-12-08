#ifndef __FEATURE_H__
#define __FEATURE_H__

/********************************************************************************/
//启用授权认证功能
//#define	ENABLE_CERTIFICATION
#ifdef ENABLE_CERTIFICATION
	
	//发布版的证书都是加密的
	//内部功能测试时，都使用明文证书，防止相关工具外泄
	#define	USE_ENCRYPT_CERTIFICATION
#endif


//新功能测试开关，后台自动构建rush引擎时会定义此宏
#define RUSH_TESTING_FALG	0


#define MASK_VIRUS_NAME		0


#define	ONLY_SCAN_VIRUS		0

#define COMPRESS_PACKAGE_BOMB_CHECK	1

// use memory or not when loadling viruslib on linux
#define USE_MEMORY_RATHER_THAN_FILE 0


//加载大型未压缩的病毒库
//1> 病毒库必须是未压缩的  2> 不支持热升级
//#define	LOAD_HUGE_UNCOMPRESS_MALWARE_MODE


//定义则使用rxsecns中的云引擎，否则使用cloundcom
//#define ENABLE_CAV_RXSECNS
//#define ENABLE_CAV_ZDR17

/************************************ Algorithm Setting ***********************************/

//限制一下lzmaprop.dc的大小，防止申请更大的内存
#define LZMA_MAX_DC_SIZE			(256 * 1024 * 1024)	

/************************************* Archive Setting ************************************/

/************************************** 250 platform **************************************/

//启用格式过滤功能
#define	ENABLE_PROCESS_FILETER

//定义则使用可回收文件系统，否则使用原文件系统
#define USE_REVOKE_VFILE_SYSTEM


#endif //__FEATURE_H__
