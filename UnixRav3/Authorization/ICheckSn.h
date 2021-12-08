#ifndef __ICHECKSN_H__
#define __ICHECKSN_H__

#include "RSCFGDEF.H"

/*******************************************************************************
<序列号验证调用接口>
*******************************************************************************/
class ICheckSn
{
public:
	/**********************************************************
	<序列号检查函数CheckSn>
	返回值说明：
		验证正确：ERR_SN_VAL_SUCCESS
		验证出错：ERR_SN_VAL_INVALID，
				  取具体出错信息调用GetFirstError和GetLastError
	**********************************************************/
	virtual RS_LONG		CheckSn(					//验证序列号
						RS_CHAR *pstSn, RS_INT nSnSize, RS_DWORD dwSnKey) = 0;	
	/**********************************************************
	<以下函数为取具体序列号信息函数>：需在CheckSn调用后调用
	返回值说明：
		正确：返回具体值
		出错：ERR_SN_VAL_INVALID，
			  取具体出错信息调用GetFirstError和GetLastError
	**********************************************************/
	virtual	RS_LONG		GetProductBigType() = 0;	//取产品大类
	virtual RS_LONG		GetProductType() = 0;		//取产品类型
	virtual RS_LONG		GetProductSubtype() = 0;	//取产品子类型 
	virtual RS_LONG		GetTrialType() = 0;			//取限制类型
	virtual RS_LONG		GetBatchNo() = 0;			//取批号
	virtual RS_LONG		GetFlowNo() = 0;			//取流水号
	virtual RS_LONG		GetRegionNo() = 0;			//取地区号
	virtual RS_LONG		GetOEMID() = 0;				//取OEM厂家ID
	virtual RS_LONG		GetHardwareType() = 0;		//取硬件平台
	virtual RS_LONG		GetOSType() = 0;			//取OS系统类型
	virtual RS_LONG		GetOSVersion() = 0;			//取OS系统版本
	virtual RS_LONG		GetNumCenters() = 0;		//取系统中心数
	virtual RS_LONG		GetNumServers() = 0;		//取服务器数量
	virtual RS_LONG		GetNumClients() = 0;		//取客户端数量
	virtual RS_LONG		GetDecodeVer() = 0;			//取解密版本
	/**********************************************************
	<以下函数为错误处理函数>
	返回值说明：
		返回错误号
	**********************************************************/
	virtual RS_LONG		GetFirstError() = 0;		//取第一个错误号
	virtual RS_LONG		GetLastError() = 0;			//取最后一个错误号
};

#endif
