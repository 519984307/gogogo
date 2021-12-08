///////////////////////////////////////////////////////////////////////////
//  项目名	：瑞星2005配置树测试用例
//	文件名	：testregapi.c
//	用  途	：测试瑞星2005配置树操作和regapi的结构
//	作  者	：许自龙
//	完成日期  : 2004-11-10
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <regapi.h>

int ProcRav(int nFunc,char *szConfigFile,char *szImportFile);
int ProcSerial(int nFunc,char *szConfigFile,char *szSerial);
int ProcExport(int nFunc,char *szConfigFile);

void ListSerial(UNIXSNPARAM stSerino);

void Usage()
{
	fprintf(stderr,"\n\n");
	fprintf(stderr,"Usage :testcfg [-c config]  -d : display ravservice parameters \n");
	fprintf(stderr,"Usage :testcfg [-c config]  -r  file: import ravservice parameters \n\n");
	fprintf(stderr,"Usage :testcfg [-c config]  -R  file: import  serial  \n\n");

	fprintf(stderr,"Usage :testcfg [-c config]  -s serno   : set product serial \n");
	fprintf(stderr,"Usage :testcfg [-c config]  -t serno: check serno");
	fprintf(stderr,"Usage :testcfg [-c config]  -g : Get serno\n");
	fprintf(stderr,"Usage :testcfg [-c config]  -e 1 : export 0xffffffff\n");
	fprintf(stderr,"Usage :testcfg [-c config]  -e 2 : export 0x6000\n");
	
	fprintf(stderr,"Usage :testcfg -h");
	fprintf(stderr,"\n\n");
	exit(0);
}

/////////////////////////////////////////////////////////////////
//  函数名： int main(int argc,char *argv[])
//  用途： 根据命令行参数的不同，完成不同的管理
//  对全局变量的影响：
//  参数说明 ： 
// 		./testregapi [-c config] -R 显示ravservice的参数
//		./testregapi [-c config] -r filename 导入ravservice设置
//		./testregapi [-c config] -s serno  设置序列号
//		./testregapi [-c config] -t serno 测试序列号
//		./testregapi [-c config] -g  :获取已经设置的序列号
//		help
//			-h
//		总的格式是 ：testcfg [-c config] [-r] [-d |-i file]//  返回结果 ：
/////////////////////////////////////////////////////////////

int main(int argc,char *argv[])
{
	char *szValid_options;
	char *szConfigFile,*szImportFile,*szSerno; 
	int nRet;
	
	int nFunc;
		

	if(argc==1)
	{
		Usage();
	}

	szValid_options="?dc:r:R:gGs:t:e:";
	szConfigFile=NULL;
	
	nFunc=-1;
	while( (nRet=getopt(argc,argv,szValid_options))!=-1)
	{
		switch(nRet)
		{
			case 'h':
				Usage();
				break;
				
			case 'c':
				szConfigFile=strdup(optarg);
				break;
				
			case 'r':
				if( !(szImportFile=strdup(optarg)))
					Usage();
				nFunc=1;
				break;
				
			case 'R':
				if( !(szImportFile=strdup(optarg)))
					Usage();
				nFunc=0x100;
				break;

			case 'd':
					nFunc=0;
				break;
			case 's':
				if( !(szSerno=strdup(optarg)))
					Usage();
				else
					nFunc=3;
				break;

			case 't':
				if( !(szSerno=strdup(optarg)))
					Usage();
				else
					nFunc=2;
				break;
			case 'g':
					nFunc=4;
					break;
			case 'G':
					nFunc=5;
					break;
			case 'e':
				nFunc=atoi(optarg);
				if(nFunc<=0 || nFunc>2)
					Usage();
				nFunc+=5;
				break;
					
			default:
				Usage();
				break;
		}			
				
	}
	
	if(nFunc==-1)
		Usage();

	if(!szConfigFile)
		szConfigFile=strdup("RsConfig.cfg");
	
	switch(nFunc)
	{
		case 0:
		case 1:
		case 0x100:
			ProcRav(nFunc,szConfigFile,szImportFile);
			break;
		case 2:
		case 3:
		case 4:
		case 5:
			ProcSerial(nFunc-2,szConfigFile,szSerno);
			break;
		case 6:
		case 7:
			ProcExport(nFunc-5,szConfigFile);
			break;
	}
			
	return 0;	

}
int ProcRav(int nFunc,char *szConfigFile,char *szImportFile)
{
	UNIXRSRAVPARAM stParam;
	UNIX_RS_ERROR_TYPE nRet;
	char szBuf[256];

	void *pHandle;
	
	nRet=InitCfgConfig(0,szConfigFile,&pHandle);
	if(nRet!=UNIX_RS_ERROR_OK )
	{
		fprintf(stderr,"%s\n",unixrscfgerr(nRet,szBuf,256));
		return -1;
	}
	if(!pHandle)
		return -1;
	

	if(nFunc==1 || nFunc==0x100)
	{
		//nRet=stCfg.ImportRsCfg(szImportFile);
		if(nFunc==1)
			nRet=WriteCfgScanParam(pHandle,EXPORTRAVSERVICE,szImportFile);   
		else
			nRet=WriteCfgScanParam(pHandle,EXPORTCONFIG,szImportFile);   
			
		if(nRet==UNIX_RS_ERROR_OK)
		{
			fprintf(stderr,"Import %s to %s Ok\n",szImportFile,szConfigFile);
			
		}
		else
		{
			fprintf(stderr,"Import %s to %s Failed\n",szImportFile,szConfigFile);
			fprintf(stderr,"%s\n",unixrscfgerr(nRet,szBuf,256));
			ExitCfgConfig(0,&pHandle);
			return -1;
		}
		ExitCfgConfig(0,&pHandle);

		return 0;

		
	}
	//nRet=stCfg.ReadConfig(stParam);
	nRet=ReadCfgScanParam(pHandle,0,&stParam);
	
	if(nRet==UNIX_RS_ERROR_OK)
	{
		;//fprintf(stderr,"Import %s to %s Ok\n",szImportFile,szConfigFile);
		fprintf(stderr,"=========list ravservice param=========================\n");
		fprintf(stderr,"nKillMode=%d\n",stParam.nKillMode);
		fprintf(stderr,"nKFailedMode=%d\n",stParam.nKFailedMode) ;
		fprintf(stderr,"finished proc:=%d\n",stParam.nFinishMode);
		fprintf(stderr," Force killed =%s\n",stParam.szForceType); 
	
		fprintf(stderr,"filter mode=%d\n",stParam.nFilterType);
		fprintf(stderr," all file filter %s\n",stParam.szAllFilter);
		fprintf(stderr,"exe file filter %s\n",stParam.szExeFilter);
		fprintf(stderr,"ext file filter %s\n",stParam.szExtFilter);
		fprintf(stderr,"Backup or not:%d\n",stParam.BackupType); // 是否备份
	
		fprintf(stderr,"scan compress %d\n",stParam.nKillCompress); // 是否查压缩文件
		fprintf(stderr,"scan mbox %d\n",stParam.nKillMbox); // 是否查邮箱文件
		fprintf(stderr,"scan mail %d\n",stParam.nKillMail); //是否 查邮件
		fprintf(stderr,"scan self exec %d\n",stParam.nKillSelfExtract); // 是否查自解压可执行文件

		fprintf(stderr,"scan dos exe %d\n",stParam.nKillDos); // 是否查杀Dos执行文件
		fprintf(stderr,"scan win exe %d\n",stParam.nKillWin);  // 是否查杀Win执行文件
		fprintf(stderr,"scan script %d\n",stParam.nKillScript);  // 是否查杀脚本文件
		fprintf(stderr,"scan macro %d\n",stParam.nKillMacro);  // 是否查杀宏
		fprintf(stderr,"scan general %d\n",stParam.nKillGeneral);  // 是否查杀一般文件

		fprintf(stderr,"scan unknown dos %d\n",stParam.nKillUnknownDos); // 是否查杀未知Dos执行文件
		fprintf(stderr,"scan unknown  win%d\n",stParam.nKillUnknownWin); // 是否查杀未知Win执行文件
		fprintf(stderr,"scan unknown  script %d\n",stParam.nKillUnknownScript); // 是否查杀未知脚本文件
		//fprintf(stderr,"scan unknown  mail %d\n",stParam.nKillUnknownMail); // 是否查杀未知邮件
		fprintf(stderr,"scan unknown  macro %d\n",stParam.nKillUnknownMacro); // 是否查杀未知宏
	
			
		
		
	}
	else
	{
		fprintf(stderr,"Display ravservice\n");
		fprintf(stderr,"%s\n",unixrscfgerr(nRet,szBuf,256));
		//return -1;
	}  
	ExitCfgConfig(0,&pHandle);

	return 0;
	
}

///////////////////////////////////////////////////////////////////////////////////////////////
//  函数名：  int ProcSerial(int nFunc,char *szConfigFile,char *szSerial)
//  用途： 测试对序列号的操作
//  对全局变量的影响：
//  参数说明 ：
//		nCode ：功能代码
//			0 ：检查序列号
//			1 : 设置序列号
//			2 : 获取设置的序列号			
//		szSerial : 要检查的序列号
//		szConfigfile :配置文件名，为NULL是根据nCode决定
//		szInstallPath ：指定安装路径
//  返回结果 ：
///////////////////////////////////////////////////////////////////////////////////////////////

int ProcSerial(int nFunc,char *szConfigFile,char *szSerial)
{
	UNIXSNPARAM stSerino;
	UNIX_RS_ERROR_TYPE nRet;
	char szBuf[256];

	void *pHandle;

	if(nFunc==3) 
	{
		fprintf(stderr,"test ReadSNID\n");
		nRet=ReadSNID(&stSerino);

		if(nRet!=UNIX_RS_ERROR_OK )
		{
			fprintf(stderr,"%s\n",unixrscfgerr(nRet,szBuf,256));
			return -1;
		}
		
		fprintf(stderr,"====== List Serial  Information===\n");
		ListSerial(stSerino);
		return 0;
	
	}
	
	nRet=InitCfgConfig(0,szConfigFile,&pHandle);
	if(nRet!=UNIX_RS_ERROR_OK )
	{
		fprintf(stderr,"%s\n",unixrscfgerr(nRet,szBuf,256));
		return -1;
	}
	if(!pHandle)
		return -1;
	
	switch(nFunc)
	{
		case 0:
		case 2:
			if(nFunc==0)
				nRet=CheckCfgSnValid(pHandle,0,szSerial,&stSerino);
			else
				nRet=ReadCfgSnInfo(pHandle,0,&stSerino);
				
			if(nRet==UNIX_RS_ERROR_OK)
			{
				fprintf(stderr,"Check Serial  Ok\n");
				
			}
			else
			{
				fprintf(stderr,"Check Serial %s Failed\n",szSerial);
				fprintf(stderr,"%s\n",unixrscfgerr(nRet,szBuf,256));
				ExitCfgConfig(0,&pHandle);
				return -1;
			}
        	
			fprintf(stderr,"====== List Serial  Information===\n");
			ListSerial(stSerino);
			ExitCfgConfig(0,&pHandle);	
			return 0;
			
		case 1:
			nRet=WriteCfgSnInfo(pHandle,0,szSerial,&stSerino);
			if(nRet==UNIX_RS_ERROR_OK)
			{
				nRet=ReadCfgSnInfo(pHandle,0,&stSerino);
				if(nRet==UNIX_RS_ERROR_OK)
					ListSerial(stSerino);
				else
					fprintf(stderr,"%s\n",unixrscfgerr(nRet,szBuf,256));
			}
			else
				fprintf(stderr,"%s\n",unixrscfgerr(nRet,szBuf,256));	
			ExitCfgConfig(0,&pHandle);
			break;
			
		default:
			break;
		
	}
	

	return 0;
}

void ListSerial(UNIXSNPARAM stSerino)
{
	fprintf(stderr,"Serial No. : %s\n",stSerino.szSn);
	fprintf(stderr,"Big type of production : %d\n",stSerino.nBigProdId);
	fprintf(stderr,"Type of Production : %d\n",stSerino.nProdId);
	fprintf(stderr,"Child Type of Production : %d\n",stSerino.nChildProdId);
	fprintf(stderr,"Expire Data Type : %d\n",stSerino.nExpireType);
	fprintf(stderr,"Batch No.: %d\n",stSerino.nPNo);
	fprintf(stderr,"Flux No. : %d\n",stSerino.nStreamNo);
	fprintf(stderr,"Area No. : %d\n",stSerino.nAreaNo);
	fprintf(stderr,"OEM ID No. : %d\n",stSerino.nOEMId);
	fprintf(stderr,"HardWare Type. : %d\n",stSerino.nHardWare);
	fprintf(stderr,"Os Type. : %d\n",stSerino.nOsType);
	fprintf(stderr,"Os Ver. : %d\n",stSerino.nOsVer);
	fprintf(stderr,"Produce Id. : %d %X\n",stSerino.nPrId,stSerino.nPrId);
	fprintf(stderr, "Expire Flag :%d\n",stSerino.nExpireFlag);
	
	return ;
	
}


///////////////////////////////////////////////////////////////////////////////////////////////
//  函数名：  int ProcExport(int nFunc,char *szConfigFile)
//  用途： 导出配置文件的节点
//  对全局变量的影响：
//  参数说明 ：
//		nFunc ：1 导出 0xffffffff 2 导出ravservice 0x6000
//		szConfigfile :配置文件名，为NULL是根据nCode决定
//  返回结果 ：
///////////////////////////////////////////////////////////////////////////////////////////////

int ProcExport(int nFunc,char *szConfigFile)
{

	char szBuf[256],szExportFile[256];
	int nRet;

	void *pHandle;
	
	nRet=InitCfgConfig(0,szConfigFile,&pHandle);
	if(nRet!=UNIX_RS_ERROR_OK )
	{
		fprintf(stderr,"%s\n",unixrscfgerr(nRet,szBuf,256));
		return -1;
	}
	if(!pHandle)
		return -1;
	
	if(nFunc==1)
		sprintf(szExportFile,"0xffffffff.cfg");
	else
		sprintf(szExportFile,"0x6000.cfg");
		
	nRet=ExportNode(pHandle,nFunc,szExportFile);
	ExitCfgConfig(0,&pHandle);
	
	if(nRet!=UNIX_RS_ERROR_OK)
	{
		fprintf(stderr,"%s\n",unixrscfgerr(nRet,szBuf,256));
		return -1;
	}
	else
		fprintf(stderr,"Export ok :%s\n",szExportFile);

	return 0;

}
