#ifdef UNIXPLATFORM
#include <stdio.h>
    #include <string.h>
#else
    #include "stdafx.h"
#endif
#include "checksn.h"
#include "cfgdef2.h"
#include "extern.h"
#include "regapi.h"

CCheckSn::CCheckSn()
{
        InitParam();
}

CCheckSn::~CCheckSn()
{
        InitParam();
}

RS_LONG	CCheckSn::GetProductID()
{
        LONG lBigType = GetProductBigType();
        if(ERR_SN_VAL_INVALID == lBigType)
                return 0;

        LONG lType = GetProductType();
        if(ERR_SN_VAL_INVALID == lType)
                return 0;

        LONG lSubType = GetProductSubtype();
        if(ERR_SN_VAL_INVALID == lSubType)
                lSubType = 0;

        LONG lRegion = GetRegionNo();
        if(ERR_SN_VAL_INVALID == lRegion)
                return 0;

        LONG lHWType = GetHardwareType();
        if(ERR_SN_VAL_INVALID == lHWType)
                lHWType = 0;

        LONG lOEMID = GetOEMID();
        if(ERR_SN_VAL_INVALID == lOEMID)
                lOEMID = 0;

        LONG lProductID = 0;

        //lProductID = lBigType<<28 | lType<<20 | lSubType<<16;
        //此ID值用来升级，供网站进行产品目录重定向
        lProductID = lBigType<<28 | lType<<20 | lSubType<<16 | lRegion<< 12 | lHWType << 8 | lOEMID;

        return lProductID;
}

RS_LONG	CCheckSn::GetTranslatedProductID()
{
        LONG lBigType = GetProductBigType();
        if(ERR_SN_VAL_INVALID == lBigType)
                return 0;

        LONG lType = GetProductType();
        if(ERR_SN_VAL_INVALID == lType)
                return 0;

        LONG lSubType = GetProductSubtype();
        if(ERR_SN_VAL_INVALID == lSubType)
                lSubType = 0;

        LONG lRegion = GetRegionNo();
        if(ERR_SN_VAL_INVALID == lRegion)
                return 0;

        LONG lHWType = GetHardwareType();
        if(ERR_SN_VAL_INVALID == lHWType)
                lHWType = 0;

        LONG lOEMID = GetOEMID();
        if(ERR_SN_VAL_INVALID == lOEMID)
                lOEMID = 0;

        if(	(lType >= CONFIG_SNTYPE_PSN_2000 && lType <= CONFIG_SNTYPE_PSN_2003) ||
                (lType == CONFIG_SNTYPE_PSN_2004)	)
        {
                if(	((lOEMID >= 0) && (lOEMID <= CONFIG_SNOEMID_ACER)) ||	//所有OEM和在线版统一为在线版
                        (lOEMID == CONFIG_SNOEMID_OEM) ||
                        ((lOEMID == CONFIG_SNOEMID_OEM_LEGEND) && (lType < CONFIG_SNTYPE_PSN_2004))||
                        (lOEMID == CONFIG_SNOEMID_ONLINE) ||
                        (lOEMID == CONFIG_SNOEMID_RAV) ||
                        (lOEMID == CONFIG_SNOEMID_RAVRFW) ||
                        (lOEMID == CONFIG_SNOEMID_TAX) )
                {
                        lOEMID = CONFIG_SNOEMID_ONLINE;
                }
                else if(lOEMID == CONFIG_SNOEMID_SHARE)
                {
                        lRegion = CONFIG_SNREGION_CHINAML;		//国际共享版的地区统一用中国大陆
                }
                if(lRegion == CONFIG_SNREGION_CHINAHK
                        || lRegion == CONFIG_SNREGION_CHINATW
                        || lRegion == CONFIG_SNREGION_MIPS
                        || lRegion == CONFIG_SNREGION_MIPS64
                        || lRegion == CONFIG_SNREGION_GERMANY
                        || lRegion == CONFIG_SNREGION_HOLLAND)
                {
                        lRegion = CONFIG_SNREGION_CHINAML;		//国际共享版的地区统一用中国大陆
                        lOEMID = CONFIG_SNOEMID_SHARE;
                }
                //不是2005版的类型都转为2005版的
                lType = CONFIG_SNTYPE_PSN_2005;
        }

        //此ID值用来升级，供网站进行产品目录重定向
        LONG lProductID = 0;
        lProductID = lBigType<<28 | lType<<20 | lSubType<<16 | lRegion<< 12 | lHWType << 8 | lOEMID;
        return lProductID;
}

// 检测序列号
RS_LONG	CCheckSn::CheckSn(RS_CHAR *pstSn, RS_INT nSnSize, RS_DWORD dwSnKey)
{
        memcpy(m_aszSn, pstSn, nSnSize);
        m_nSnSize = nSnSize;
        m_dwSnKey = dwSnKey;

        RS_LONG		lChkResult = 0;			// 检测是否正确

        const RS_CHAR TranTableN[36] = "0123456789ABCDEFGHIJKLMNPQRSTUVWXYZ";

        RS_UINT i = 0;
        RS_UINT j = 0;
        RS_CHAR aszTempSCN[25] = { 0 };
        RS_LONG lBatchNumber = 0;
        RS_LONG lBatchNumber_1 = 0;
        RS_CHAR aszErrorDescription[128] = { 0	};
        RS_CHAR acScnInfo[16] = { 0 };

        // 去掉序列号中的'_' [4/1/2002]
        for ( i = 0; i < 27; i++ )
        {
                if ( m_aszSn[i] != '-' )
                        aszTempSCN[j++] = m_aszSn[i];
        }

        if ( j != 24 )
        {
                SetErrCode(ERR_SN_LENGTH_ERR);
                return ERR_SN_VAL_INVALID;
        }

        if ( strlen(aszTempSCN) != 24 )
        {
                SetErrCode(ERR_SN_LENGTH_ERR);
                return ERR_SN_VAL_INVALID;
        }

        // 第十六步 去出批号信息 [4/1/2002]
        RS_BYTE bTemp_16_0 = 0xFF;
        RS_BYTE bTemp_16_1 = 0xFF;
        RS_BYTE bTemp_16_2 = 0xFF;
        RS_BYTE bTemp_16_3 = 0xFF;

        for ( i = 0; i < 35; i++ )
        {
                if ( TranTableN[i] == aszTempSCN[20] )
                        bTemp_16_0 = i;
                if ( TranTableN[i] == aszTempSCN[21] )
                        bTemp_16_1 = i;
                if ( TranTableN[i] == aszTempSCN[22] )
                        bTemp_16_2 = i;
                if ( TranTableN[i] == aszTempSCN[23] )
                        bTemp_16_3 = i;
        }

        // 对字符的合法性进行检测 [7/7/2002]
        if (  bTemp_16_0 == 0xFF ||
                  bTemp_16_1 == 0xFF ||
                  bTemp_16_2 == 0xFF ||
                  bTemp_16_3 == 0xFF )
        {
                SetErrCode(ERR_SN_STRING_ERR);
                return ERR_SN_VAL_INVALID;
        }

        lBatchNumber = bTemp_16_3;
        lBatchNumber <<= 4;
        lBatchNumber += bTemp_16_2;
        lBatchNumber <<= 4;
        lBatchNumber += bTemp_16_1;
        lBatchNumber <<= 4;
        lBatchNumber += bTemp_16_0;

        m_stSnInfo.Btch = lBatchNumber;
        lBatchNumber_1 = lBatchNumber;
        lBatchNumber &= 0x7FFF;

// Added by LauranceZhang 2003-9-20 18:34:53 <
        if ( lBatchNumber >= CONFIG_SNBATCH_DIVIDELINE4 )
        {
                if ( DecSNO_5( m_aszSn, 27, acScnInfo, 15, aszErrorDescription, 128 ) )
                {
                        memcpy( m_szSnInfo  , acScnInfo, 16);
                        if ( SetInfo( (RS_UCHAR *)acScnInfo ) )
                        {
                                lChkResult = ERR_SN_VAL_SUCCESS;
                                m_stSnInfo.lSCNVerID = CONFIG_SNENCRYPT_VERSION5;
                                m_stSnInfo.Btch = lBatchNumber;
                        }
                        else
                        {
                                SetErrCode(ERR_SN_TYPE_ERR);
                                lChkResult = ERR_SN_VAL_INVALID;
                        }
                }
                else
                {
                        SetErrCode(ERR_SN_DECODE5_ERR);
                        lChkResult = ERR_SN_VAL_INVALID;
                }
        }
        else
// Added by LauranceZhang 2003-9-20 18:34:53 >
        if ( lBatchNumber >= CONFIG_SNBATCH_DIVIDELINE3 )
        {
                if ( DecSNO_4( m_aszSn, 27, acScnInfo, 15, aszErrorDescription, 128 ) )
                {
                        memcpy( m_szSnInfo  , acScnInfo, 16);
                        if ( SetInfo( (RS_UCHAR *)acScnInfo ) )
                        {
                                lChkResult = ERR_SN_VAL_SUCCESS;
                                m_stSnInfo.lSCNVerID = CONFIG_SNENCRYPT_VERSION4;
                                m_stSnInfo.Btch = lBatchNumber;
                        }
                        else
                        {
                                SetErrCode(ERR_SN_TYPE_ERR);
                                lChkResult = ERR_SN_VAL_INVALID;
                        }
                }
                else
                {
                        SetErrCode(ERR_SN_DECODE4_ERR);
                        lChkResult = ERR_SN_VAL_INVALID;
                }
        }
        else if ( lBatchNumber >= CONFIG_SNBATCH_DIVIDELINE2 )
        {
                if ( DecSNO_3( m_aszSn, 27, acScnInfo, 15, aszErrorDescription, 128 ) )
                {
                        memcpy( m_szSnInfo  , acScnInfo, 16);
                        if ( SetInfo( (RS_UCHAR *)acScnInfo ) )
                        {
                                lChkResult = ERR_SN_VAL_SUCCESS;
                                m_stSnInfo.lSCNVerID = CONFIG_SNENCRYPT_VERSION3;
                                m_stSnInfo.Btch = lBatchNumber;
                        }
                        else
                        {
                                SetErrCode(ERR_SN_TYPE_ERR);
                                lChkResult = ERR_SN_VAL_INVALID;
                        }
                }
                else
                {
                        SetErrCode(ERR_SN_DECODE3_ERR);
                        lChkResult = ERR_SN_VAL_INVALID;
                }
        }
        else if ( ( lBatchNumber < CONFIG_SNBATCH_DIVIDELINE2 ) && ( lBatchNumber >= CONFIG_SNBATCH_DIVIDELINE1 ) && ( lBatchNumber_1 > 0x7FFF ) )
        {
                if ( DecSNO_2( m_aszSn, &m_stSnInfo, m_dwSnKey ) )
                {
                        if ( lBatchNumber_1 > 0x7FFF )
                                m_stSnInfo.lNetOrPc = 2;
                        else
                                m_stSnInfo.lNetOrPc = 1;

                        lChkResult = ERR_SN_VAL_SUCCESS;
                        m_stSnInfo.lSCNVerID = CONFIG_SNENCRYPT_VERSION2;
                }
                else
                {
                        SetErrCode(ERR_SN_DECODE2_ERR);
                        lChkResult = ERR_SN_VAL_INVALID;
                }

        }
        else if ( lBatchNumber_1 > 0x7FFF )
        {
                if ( DecSNO_1( m_aszSn, &m_stSnInfo) )
                {
                        if ( m_stSnInfo.Btch > 0x7FFF )
                                m_stSnInfo.lNetOrPc = 2;
                        else
                                m_stSnInfo.lNetOrPc = 1;

                        lChkResult = ERR_SN_VAL_SUCCESS;
                        m_stSnInfo.lSCNVerID = CONFIG_SNENCRYPT_VERSION1;
                }
                else
                {
                        SetErrCode(ERR_SN_DECODE1_ERR);
                        lChkResult = ERR_SN_VAL_INVALID;
                }

        }
        else
        {
                if ( DecSNO_0( m_aszSn, &m_stSnInfo, m_stSnInfo.Btch ) )
                {
                        if ( m_stSnInfo.Btch > 0x7FFF )
                                m_stSnInfo.lNetOrPc = 2;
                        else
                                m_stSnInfo.lNetOrPc = 1;
                        if ( m_stSnInfo.Btch > 2 )
                                m_stSnInfo.VerType = 0;
                        else
                                m_stSnInfo.VerType = 8;

                        lChkResult = ERR_SN_VAL_SUCCESS;
                        m_stSnInfo.lSCNVerID = CONFIG_SNENCRYPT_VERSION0;
                }
                else
                {
                        SetErrCode(ERR_SN_DECODE0_ERR);
                        lChkResult = ERR_SN_VAL_INVALID;
                }
        }

        if(lChkResult==ERR_SN_VAL_SUCCESS)
                m_bIsChecked =RS_TRUE;
        else
                InitParam();
        return lChkResult;
}


RS_LONG	CCheckSn::GetNumCenters()		//系统中心数量
{
        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;
        return m_stSnInfo.SysCnt;
}

RS_LONG	CCheckSn::GetNumServers()		//服务器数量
{
        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;
        return m_stSnInfo.Svr;
}

RS_LONG	CCheckSn::GetNumClients()		//客户端数量
{
        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;
        return m_stSnInfo.Clnt;
}

RS_LONG	CCheckSn::GetBatchNo()		//批号
{
        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;
        return m_stSnInfo.Btch;
}

RS_LONG	CCheckSn::GetFlowNo()		//流水号
{
        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;
        return m_stSnInfo.SNo;
}

RS_LONG    CCheckSn::GetDecodeVer()
{
        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;

        return m_stSnInfo.lSCNVerID;
}

// 地区号
RS_LONG	CCheckSn::GetRegionNo()
{
        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;
        //return m_stSnInfo.lAreaID;
        RS_LONG lRegion = ERR_SN_VAL_INVALID;
        switch(m_stSnInfo.lAreaID)
        {
        case 0:
                lRegion = CONFIG_SNREGION_CHINAML;
                break;
        case 1:
                lRegion = CONFIG_SNREGION_CHINAHK;
                break;
        case 2:
                lRegion = CONFIG_SNREGION_JAPAN;
                break;
        case 3:
                lRegion = CONFIG_SNREGION_CHINATW;
                break;
        case 9:
                lRegion = CONFIG_SNREGION_MIPS;
                break;
        case 10:
                lRegion = CONFIG_SNREGION_MIPS64;
                break;
        case 4:
                lRegion = CONFIG_SNREGION_GERMANY;
                break;
        case 7:
                lRegion = CONFIG_SNREGION_HOLLAND;
                break;
        default:
                break;
        }
        return lRegion;
}
// OEM厂家ID
RS_LONG	CCheckSn::GetOEMID()
{
        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;
        RS_LONG lProductTypeBig = GetProductBigType();
        if(lProductTypeBig != CONFIG_SNEDITION_PERSONAL)
                return ERR_SN_VAL_INVALID;

        RS_LONG lProductType = GetProductType();
        RS_LONG lOEMID = ERR_SN_VAL_INVALID;
        switch(m_stSnInfo.lOEMID)
        {
        case -1:
                if(	lProductType==CONFIG_SNTYPE_PSN_2000 ||
                        lProductType==CONFIG_SNTYPE_PSN_2001)
                        lOEMID = CONFIG_SNOEMID_STANDARD;
                break;
        case 0:
// Modified by LauranceZhang 2004-11-15 9:55:00 <
/*
                if(m_stSnInfo.VerType==CONFIG_SNTYPE_PSN_2001)
                        return ERR_SN_VAL_INVALID;
                else
                        lOEMID = CONFIG_SNOEMID_STANDARD;
*/
                lOEMID = CONFIG_SNOEMID_STANDARD;
// Modified by LauranceZhang 2004-11-15 9:55:00 >
                break;
        case 1:
                if(lProductType==CONFIG_SNTYPE_PSN_2001)
                        lOEMID = CONFIG_SNOEMID_LEGEND;
                else
                        lOEMID = CONFIG_SNOEMID_UPGRADE;
                break;
        case 2:
                if(lProductType==CONFIG_SNTYPE_PSN_2001)
                        lOEMID = CONFIG_SNOEMID_FOUNDER;
                else
                        lOEMID = CONFIG_SNOEMID_CLASSA;
                break;
        case 3:
                if(lProductType==CONFIG_SNTYPE_PSN_2001)
                        lOEMID = CONFIG_SNOEMID_IBM;
                else
                        lOEMID = CONFIG_SNOEMID_CLASSB;
                break;
        case 4:
                if(lProductType==CONFIG_SNTYPE_PSN_2001)
                        lOEMID = CONFIG_SNOEMID_LANGCHAO;
                else
                        lOEMID = CONFIG_SNOEMID_CTN_FWL;
                break;
        case 5:
                if(lProductType==CONFIG_SNTYPE_PSN_2001)
                        lOEMID = CONFIG_SNOEMID_ACER;
                else
                        lOEMID = CONFIG_SNOEMID_CTN_NFWL;
                break;
        case 6:
                lOEMID = CONFIG_SNOEMID_NCTN_FWL;
                break;
        case 7:
                lOEMID = CONFIG_SNOEMID_ONLINE;
                break;
        case 8:
                if(lProductType==CONFIG_SNTYPE_PSN_FWALL)
                        lOEMID = CONFIG_SNOEMID_NETGAME;
                else if(lProductType==CONFIG_SNTYPE_PSN_2003)	//03年出过税控版
                        lOEMID = CONFIG_SNOEMID_TAX;
                else
                        lOEMID = CONFIG_SNOEMID_RAV;
                break;
        case 9:
                lOEMID = CONFIG_SNOEMID_RAVRFW;
                break;
        case 10:
                lOEMID = CONFIG_SNOEMID_SHARE;
                break;
        case 100:
                lOEMID = CONFIG_SNOEMID_OEM;
                break;
        case 101:
                lOEMID = CONFIG_SNOEMID_OEM_LEGEND;
                break;
        case 102:
                lOEMID = CONFIG_SNOEMID_LCOMM;
                break;
        case 103:
                lOEMID = CONFIG_SNOEMID_HELLOKITY;
                break;

        default:
                break;
        }
        return lOEMID;
}

RS_LONG	CCheckSn::GetTrialDays()
{
        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;
        RS_LONG lTrial = ERR_SN_VAL_INVALID;
        RS_LONG lBigType = GetProductBigType();
        if(lBigType == CONFIG_SNEDITION_NETWORK)
        {
                switch(m_stSnInfo.lLimitTime)
                {
                case 0:
                        lTrial = 365*10;
                        break;
                case 1:		//一个月
                        lTrial = 31;
                        break;
                case 2:		//半年
                        lTrial = (RS_LONG)(30.5 * 6);
                        break;
                case 3:		//一年
                        lTrial = (RS_LONG)(30.5 * 12);
                        break;
                }
        }
        else
        {
                switch(m_stSnInfo.lLimitTime)
                {
                case 0:
                        lTrial = 365*20 ;	//20年
                        break;
                case 1:
                        lTrial = 31;
                        break;
                case 2:
                        lTrial = 31 * 2 + 30;
                        break;
                case 3:
                        lTrial = (RS_LONG)(30.5 * 6);
                        break;
                case 4:
                        lTrial = (RS_LONG)(30.5 * 8) + 31;
                        break;
                case 5:
                        lTrial = (RS_LONG)(30.5 * 12);
                        break;
                case 6:
                        lTrial = (RS_LONG)(30.5 * 18);
                        break;
                case 7:
                        lTrial = (RS_LONG)(30.5 * 24);
                        break;
                case 8:
                        lTrial = (RS_LONG)(30.5 * 36);
                        break;
                case 9:
                        lTrial = (RS_LONG)(30.5 * 48);
                        break;
                case 10:
                        lTrial = (RS_LONG)(30.5 * 60);
                        break;
                default:
                        break;
                }
        }

    if (isTrialSnVersion(m_aszSn)) {
        lTrial = 30.5*6;
    }
        return lTrial;
}

// 限时时间
RS_LONG	CCheckSn::GetTrialType()
{
        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;
        RS_LONG lTrial = ERR_SN_VAL_INVALID;
        RS_LONG lBigType = GetProductBigType();
        if(lBigType == CONFIG_SNEDITION_NETWORK)
        {
                switch(m_stSnInfo.lLimitTime)
                {
                case 0:
                        lTrial = CONFIG_SNTRIAL_NOLIMIT_N;
                        break;
                case 1:
                        lTrial = CONFIG_SNTRIAL_ONEMON_N;
                        break;
                case 2:
                        lTrial = CONFIG_SNTRIAL_SIXMON_N;
                        break;
                case 3:
                        lTrial = CONFIG_SNTRIAL_ONEYEAR_N;
                        break;
                }
        }
        else
        {
                switch(m_stSnInfo.lLimitTime)
                {
                case 0:
                        lTrial = CONFIG_SNTRIAL_NOLIMIT;
                        break;
                case 1:
                        lTrial = CONFIG_SNTRIAL_ONEMON;
                        break;
                case 2:
                        lTrial = CONFIG_SNTRIAL_THREEMON;
                        break;
                case 3:
                        lTrial = CONFIG_SNTRIAL_SIXMON;
                        break;
                case 4:
                        lTrial = CONFIG_SNTRIAL_NINEMON;
                        break;
                case 5:
                        lTrial = CONFIG_SNTRIAL_ONEYEAR;
                        break;
                case 6:
                        lTrial = CONFIG_SNTRIAL_ONEHALFYEAR;
                        break;
                case 7:
                        lTrial = CONFIG_SNTRIAL_TWOYEAR;
                        break;
                case 8:
                        lTrial = CONFIG_SNTRIAL_THREEYEAR;
                        break;
                case 9:
                        lTrial = CONFIG_SNTRIAL_FOURYEAR;
                        break;
                case 10:
                        lTrial = CONFIG_SNTRIAL_FIVEYEAR;
                        break;
                default:
                        break;
                }
        }
        return lTrial;
}
//版本类型
RS_LONG	CCheckSn::GetProductType()
{
        //return m_stSnInfo.VerType;
        //  权值的名称                                  对应的数字 [4/11/2002]
        //	单机__2001单机版                            0
        //	单机__2002单机版                            1
        //	单机__2002增强版							2
        //	单机__服务器版								4
        //	单机__专用版								3
        //	单机__2003									5
        //	单机__千禧版								8
        //
        //	网络__网吧版								3
        //	网络__单级中心版							25
        //	网络__多级中心版							0
        //	网络__超级中心版							24
        //
        //	邮件__EXCHANG SERVER 邮件监控				6
        //	邮件__DOMINO SERVER 邮件监控				7
        //	邮件__服务器								21
        //
        //	UNIX__UNIX									22
        //	插件__插件									23

        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;

        RS_LONG lType = ERR_SN_VAL_INVALID;
        RS_LONG lBigType = GetProductBigType();
        if(lBigType==CONFIG_SNEDITION_PERSONAL)
        {
                switch(m_stSnInfo.VerType)
                {
                case 0:
                        lType = CONFIG_SNTYPE_PSN_2001;
                        break;
                case 1:
                        lType = CONFIG_SNTYPE_PSN_2002;
                        break;
                case 2:
                        lType = CONFIG_SNTYPE_PSN_2002PRO;
                        break;
                case 3:
                        lType = CONFIG_SNTYPE_PSN_SPECAIL;
                        break;
                case 4:
                        lType = CONFIG_SNTYPE_PSN_2002SVR;
                        break;
                case 5:
                        lType = CONFIG_SNTYPE_PSN_2003;
                        break;
                case 6:
                        lType = CONFIG_SNTYPE_PSN_2004;
                        break;
                case 7:
                        lType = CONFIG_SNTYPE_PSN_2005;
                        break;
                case 8:
                        lType = CONFIG_SNTYPE_PSN_2000;
                        break;
                case 9:
                        lType = CONFIG_SNTYPE_PSN_FWALL;
                        break;
                default:
                        break;
                }
        }
        else if(lBigType==CONFIG_SNEDITION_NETWORK)
        {
                switch(m_stSnInfo.VerType)
                {
                case 0:
                        lType = CONFIG_SNTYPE_NET_MULCENT;
                        break;
                case 1:		//把老号转换为应用版
                        lType = CONFIG_SNTYPE_NET_SINCENT;
                        m_stSnInfo.lEditionType = 1;		//这类版本都 < 3，专用版都 >= 3
                        break;
                case 2:		//把老号转换为限时版
                        lType = CONFIG_SNTYPE_NET_SINCENT;
                        break;
                case 3:		//把老号转换为网吧版
                        lType = CONFIG_SNTYPE_NET_SINCENT;
                        m_stSnInfo.lEditionType = 2;
                        break;
                case 19:	//把老号转换为专用版
                        lType = CONFIG_SNTYPE_NET_MULCENT;
                        m_stSnInfo.lEditionType = 1;
                        break;
                case 6:
                        lType = CONFIG_SNTYPE_MSVR_EXCHANGE;
                        break;
                case 7:
                        lType = CONFIG_SNTYPE_MSVR_DOMINO;
                        break;
                case 21:
                        lType = CONFIG_SNTYPE_MSVR_SERVER;
                        break;
                case 22:
                        lType = CONFIG_SNTYPE_UNIX;
                        break;
                case 23:
                        lType = CONFIG_SNTYPE_PLUGIN;
                        break;
                case 24:
                        lType = CONFIG_SNTYPE_NET_SUPCENT;
                        break;
                case 25:
                        lType = CONFIG_SNTYPE_NET_SINCENT;
                        break;
                case 26:
                        lType = CONFIG_SNTYPE_NOVELL;
                        break;
                case 27:
                        lType = CONFIG_SNTYPE_UNIXKDE;
                        break;
                case 28:
                        lType = CONFIG_SNTYPE_PLUGIN_HW;
                        break;
                default:
                        break;
                }
        }
        return lType;
}
// 区分网络还是单机版 [4/11/2002]  1 单机版 2 网络版
RS_LONG	CCheckSn::GetProductBigType()
{
        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;
        return m_stSnInfo.lNetOrPc;
}
// 版本类型
RS_LONG	CCheckSn::GetProductSubtype()
{
        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;

        RS_LONG lType = GetProductType();
        RS_LONG lSubType = ERR_SN_VAL_INVALID;
        RS_LONG lDecodeVer = GetDecodeVer();
        if(CONFIG_SNEDITION_NETWORK==GetProductBigType())
        {
                switch(lType)
                {
                case CONFIG_SNTYPE_NET_MULCENT:
                        if(m_stSnInfo.lEditionType == 0)
                                lSubType = CONFIG_SNSUBTYPE_STANDARD;		//企业
                        else if((m_stSnInfo.lEditionType == 1) && (lDecodeVer  >= CONFIG_SNENCRYPT_VERSION3))
                                lSubType = CONFIG_SNSUBTYPE_SPECIAL;		//专用
                        else if((m_stSnInfo.lEditionType == 19) && (lDecodeVer  < CONFIG_SNENCRYPT_VERSION3))
                                lSubType = CONFIG_SNSUBTYPE_SPECIAL;		//专用
                        break;
                case CONFIG_SNTYPE_NET_SINCENT:
                        if(m_stSnInfo.lEditionType == 0)
                                lSubType = CONFIG_SNSUBTYPE_STANDARD;		//中小企业
                        else if(m_stSnInfo.lEditionType == 1)
                        {
                                if(lDecodeVer  < CONFIG_SNENCRYPT_VERSION3)
                                        lSubType = CONFIG_SNSUBTYPE_APPLICATION;	//应用
                                else
                                        lSubType = CONFIG_SNSUBTYPE_SPECIAL;	//专用
                        }
                        else if(m_stSnInfo.lEditionType == 2)
                                lSubType = CONFIG_SNSUBTYPE_NETBAR;			//网吧
                        break;
                case CONFIG_SNTYPE_NET_SUPCENT:
                        if(m_stSnInfo.lEditionType == 0)
                                lSubType = CONFIG_SNSUBTYPE_STANDARD;
                        else if(m_stSnInfo.lEditionType == 1)
                                lSubType = CONFIG_SNSUBTYPE_SPECIAL;	//专用
                        break;
                case CONFIG_SNTYPE_PLUGIN:
                        lSubType = m_stSnInfo.lEditionType;
                        break;
                }
        }
        return lSubType;
}

// 硬件平台
RS_LONG	CCheckSn::GetHardwareType()
{
        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;

        RS_LONG lHdType = ERR_SN_VAL_INVALID;
        switch(m_stSnInfo.lHardWareID)
        {
        case 1:
                lHdType = CONFIG_SNHWTYPE_SPARC;
                break;
        case 2:
                lHdType = CONFIG_SNHWTYPE_INTEL;
                break;
        case 3:
                lHdType = CONFIG_SNHWTYPE_ALPHA;
                break;
        case 4:
                lHdType = CONFIG_SNHWTYPE_RS6000;
                break;
        case 5:
                lHdType = CONFIG_SNHWTYPE_PARISC;
                break;
        default:
                break;
        }
        return lHdType;
}


// 系统类型
RS_LONG	CCheckSn::GetOSType()
{
        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;

        RS_LONG lOSType = ERR_SN_VAL_INVALID;
        switch(m_stSnInfo.lSystemID)
        {
        case 0:
                lOSType = CONFIG_SNOSTYPE_WINDOWS;
                break;
        case 1:
                lOSType = CONFIG_SNOSTYPE_SOLARIS;
                break;
        case 2:
                lOSType = CONFIG_SNOSTYPE_REDHAT;
                break;
        case 3:
                lOSType = CONFIG_SNOSTYPE_TURBO;
                break;
        case 4:
                lOSType = CONFIG_SNOSTYPE_AIX;
                break;
        case 5:
                lOSType = CONFIG_SNOSTYPE_FREEBSD;
                break;
        case 6:
                lOSType = CONFIG_SNOSTYPE_XTREAM;
                break;
        case 7:
                lOSType = CONFIG_SNOSTYPE_HP;
                break;
        case 8:
                lOSType = CONFIG_SNOSTYPE_LINUXKNL;
                break;
        case 9:
                lOSType = CONFIG_SNOSTYPE_NETWARE;
                break;
        default:
                break;
        }
        return lOSType;
}
// 系统版本
RS_LONG	CCheckSn::GetOSVersion()
{
        if(!m_bIsChecked)
                return ERR_SN_VAL_INVALID;

        return m_stSnInfo.lSystemVer;
}

RS_CHAR* CCheckSn::CheckBatch0(RS_CHAR *OrgSer)
{
        RS_CHAR *pszRet = 0;

        RS_CHAR *ts;
        RS_CHAR t2[255] = { 0 };
        RS_BYTE t3 = 0;
        int i;
        int j;
        ts = OrgSer;
//	t2 = ""
//	j = 1
        j = 1;
//	For i = 1 To 12
//	  t3 = Mid(ts, i, 1)
//	  If t3 = "O" Then
//		If (0 = InStr(1, "6EUM", Mid(ts, i + 1, 1))) Then t3 = "0"
//	  ElseIf t3 = "0" Then
//		If InStr(1, "6EUM", Mid(ts, i + 1, 1)) Then t3 = "O"
//	  End If
//	  t2 = t2 + t3
//	Next i
        for( i = 0; i < 12; i++ )
        {
                t3 = ts[i];
                if ( t3 == 'O' )
                {
                        if ( !(	( '6' == ts[i + 1] ) ||
                                        ( 'E' == ts[i + 1] ) ||
                                        ( 'U' == ts[i + 1] ) ||
                                        ( 'M' == ts[i + 1] ) ))
                        {
                                t3 = '0';
                        }
                }
                else if ( t3 == '0' )
                {
                        if ( ( '6' == ts[i + 1] ) ||
                                 ( 'E' == ts[i + 1] ) ||
                                 ( 'U' == ts[i + 1] ) ||
                                 ( 'M' == ts[i + 1] ) )
                        {
                                t3 = 'O';
                        }
                }
                ts[i] = t3;
        }

//	For i = 13 To 20
//	  t3 = Mid(ts, i, 1)
//	  If t3 = "O" Then
//		If (0 = InStr(1, "37BFJNRV", Mid(ts, i + 1, 1))) Then t3 = "0"
//	  ElseIf t3 = "0" Then
//		If InStr(1, "37BFJNRV", Mid(ts, i + 1, 1)) Then t3 = "O"
//	  End If
//	  t2 = t2 + t3
//	Next i
//	For i = 21 To 24
//	  t3 = Mid(ts, i, 1)
//	  If t3 = "O" Then t3 = "0"
//	  t2 = t2 + t3
//	Next i
        for( i = 12; i < 20; i++ )
        {
                t3 = ts[i];
                if ( t3 == 'O' )
                {
                        if ( !(	( '3' == ts[i + 1] ) ||
                                        ( '7' == ts[i + 1] ) ||
                                        ( 'B' == ts[i + 1] ) ||
                                        ( 'F' == ts[i + 1] ) ||
                                        ( 'J' == ts[i + 1] ) ||
                                        ( 'N' == ts[i + 1] ) ||
                                        ( 'R' == ts[i + 1] ) ||
                                        ( 'V' == ts[i + 1] ) ))
                        {
                                t3 = '0';
                        }
                }
                else if ( t3 == '0' )
                {
                        if ( ( '3' == ts[i + 1] ) ||
                                 ( '7' == ts[i + 1] ) ||
                                 ( 'B' == ts[i + 1] ) ||
                                 ( 'F' == ts[i + 1] ) ||
                                 ( 'J' == ts[i + 1] ) ||
                                 ( 'N' == ts[i + 1] ) ||
                                 ( 'R' == ts[i + 1] ) ||
                                 ( 'V' == ts[i + 1] ) )
                        {
                                t3 = 'O';
                        }
                }
                ts[i] = t3;
        }

        ts[21] = '0';
        ts[22] = '0';
        ts[23] = '0';

//	ChkO = Left(t2, 6) + "-" + Mid(t2, 7, 6) + "-" + Mid(t2, 13, 6) + "-" + Mid(t2, 19, 6)


        return pszRet;
}


RS_BOOL CCheckSn::DecSNO_0( RS_CHAR *pszno, PSTSNINFO pNetSN, RS_LONG lBatchNo )
{
        RS_CHAR *TranTable = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        RS_CHAR sno[0x20] = { 0 };
        RS_BYTE abCharNo[25] = { 0 };
        RS_INT i,j;
        RS_CHAR *psno;

//	strupr(psno);
        psno = pszno;
        if ( psno == NULL )
        {
                return RS_FALSE;
        }
        for ( i = 0; i < ( RS_INT )strlen( psno ); i++ )
        {
                if ( psno[i] >= 'a' && psno[i] <= 'z' )
                {
                        psno[i] -= 32;
                }
        }

        if (strlen(psno)!=27)
                return 0;

        if (psno[6]!='-') return 0;
        if (psno[13]!='-') return 0;
        if (psno[20]!='-') return 0;

        memcpy(sno,psno,6);
        memcpy(sno+6,psno+7,6);
        memcpy(sno+12,psno+14,6);
        memcpy(sno+18,psno+21,6);

        if ( lBatchNo == 0 )
                CheckBatch0(sno);

    //Transfer char to number
//    For i = 1 To 20
//	    CharNo(i) = InStr(TranTable, Mid$(tStr, i, 1))
//      If CharNo(i) = 0 Then Exit Sub
//      CharNo(i) = CharNo(i) - 1
//    Next i
        for ( i = 0; i < 24; i++ )
        {
                abCharNo[i] = 0xFF;
                for ( j = 0; j < 36; j++ )
                {
                        if ( TranTable[j] == sno[i] )
                        {
                                abCharNo[i] = j;
                                break;
                        }
                }
                if ( abCharNo[i] == 0xFF )
                {
                        return RS_FALSE;
                }

        }

//  Check first 12 number
//    For i = 2 To 13
//      If (CharNo(i) And 7) <> (CharNo(i - 1) \ 4) Then Exit Sub
//    Next i
        for ( i = 1; i < 13; i++ )
        {
                if ( (abCharNo[i] & 7) != (abCharNo[i-1] / 4) )
                {
                        return RS_FALSE;
                }
        }
//    'Check next 8 number
//    For i = 14 To 21
//      If (CharNo(i) And 3) <> (CharNo(i - 1) \ 8) Then Exit Sub
//    Next i
        for ( i = 13; i < 21; i++ )
        {
                if ( (abCharNo[i] & 3) != (abCharNo[i-1] / 8) )
                {
                        return RS_FALSE;
                }
        }


        return RS_TRUE;

}


RS_BOOL CCheckSn::DecSNO_1(RS_CHAR *szSerialNo, PSTSNINFO pNetSN)
{
        RS_UCHAR RAVEnStr[ 0x21 ] = "0123456789ABCDEFGHIJKLMNPQRSTUVW" ;
        RS_INT i, j ;
        RS_UCHAR tems[ 0x10 ] , temc , *lpstr1 , ChkRS_BYTE ;
        RS_SHORT temsi ;

        //		pNetSerialNo = CtlCnt = SvrNo = CliNo = 0 ;
        memset( tems , 0 , 0x10 ) ;
        lpstr1 = (RS_UCHAR*)szSerialNo ;
        for( i = 0 ; i < 20 ; i ++ )
        {
                if( *lpstr1 == 0x2d )
                {
                        lpstr1 ++ ;
                }
                for( j = 14 ; j > 0 ; j-- )
                {
                        tems[ j ] = ( tems[ j ] >> 5 ) | ( tems[ j - 1 ] << 3 ) ;
                }
                tems[ 0 ] = tems[ 0 ] >> 5 ;
                temsi = Instr( RAVEnStr , *lpstr1 ++ ) ;
                if( temsi == -1 )	return false ;
                temc = temsi & 0x1f ;
                tems[ 0 ] |= temc << 3 ;
        }
        for( i = 0 ; i < 4 ; i ++ )
        {
                for( j = 14 ; j > 0 ; j -- )
                {
                        tems[ j ] = ( tems[ j ] >> 4 ) | ( tems [ j - 1 ] << 4 ) ;
                }
                tems[ 0 ] = tems[ 0 ] >> 4 ;
                temsi = Instr( RAVEnStr , *lpstr1 ++ ) ;
                if( temsi == -1 ) return RS_FALSE ;
                temc = temsi & 0x1f ;
                tems[ 0 ] |= temc << 4 ;
        }
        if( !( tems[ 0 ] & 0x80 ) )	return RS_FALSE ;
        for( i = 0 ; i < 31 ; i ++ )
        {
                if( tems[ 8 ] == ( RS_UCHAR ) ~( ( tems[ 0xd ] << 4 ) | ( tems [ 0xe ] >> 4 ) ) )
                {
                        i = 40 ;
                        break ;
                }
                for( j = 14 ; j > 8 ; j -- )
                {
                        tems[ j ] = ( tems[ j ] >> 1 ) | ( tems[ j - 1 ] << 7 ) ;
                }
                tems[ 8 ] = ( tems[ 8 ] >> 1 ) | ( tems[ 14 ] << 4 ) ;
                tems[ 14 ] &= 0xf0 ;
        }
        if( i != 40 )	return RS_FALSE ;
        for( i = 14 ; i > 8 ; i-- )
        {
                tems[ i ] = ( tems[ i ] >> 4 ) | ( tems[ i - 1 ] << 4 ) ;
        }
        tems[ 8 ] = tems[ 8 ] >> 4 ;
        for( i = 8 ; i < 13 ; i ++ )
        {
                tems[ i ] ^= tems[ i + 1 ] ;
        }
        tems[ 8 ] &= 0xF ;
        ChkRS_BYTE = 0 ;
        for( i = 0 ; i < 8 ; i++ )
        {
                ChkRS_BYTE += tems[ i ] ;
        }
        if( tems[ 13 ] != ChkRS_BYTE )	return RS_FALSE ;
        if( ( pNetSN->VerType = tems[ 8 ] ) > 15 ) return RS_FALSE ;
        if( ( pNetSN->SysCnt = tems[ 9 ] ) > 127 )	return RS_FALSE ;
        if( ( pNetSN->Svr = tems[ 10 ] ) > 127 )	return RS_FALSE ;
        if( tems[ 12 ] > 127 )	return RS_FALSE ;
        pNetSN->Clnt = tems[ 11 ] + ( RS_INT )tems[ 12 ] * 256;

        // Add by Donghx 2002-5-31 17:36:53 <
        // 旧的网吧版向新的版本映射 [5/31/2002]
        if ( pNetSN->VerType == 3 )
        {
                pNetSN->VerType = 25;
                pNetSN->lNetOrPc = 2;
                pNetSN->lEditionType = 2;
        }

        // Add by Donghx 2002-5-31 17:36:53 >
        return RS_TRUE;

}

// 批号是27以后 到 100 的序列号 [4/11/2002]
// 第一次修改的函数 [4/11/2002]
RS_UINT CCheckSn::DecSNO_2(RS_CHAR *psno,PSTSNINFO pNetSN, RS_ULONG SNoKey)
{
        RS_CHAR *TranTableN = "0123456789ABCDEFGHIJKLMNPQRSTUVWXYZ";
        RS_CHAR sno[0x20];
        RS_UCHAR orginfo[0x20],*p;
        RS_INT i,j;
        RS_SHORT ush;

//	strupr(psno);
        for ( i = 0; i < ( RS_INT )strlen( psno ); i++ )
        {
                if ( psno[i] >= 'a' && psno[i] <= 'z' )
                {
                        psno[i] -= 32;
                }
        }

        if (strlen(psno)!=27)
                return 0;

        if (psno[6]!='-') return 0;
        if (psno[13]!='-') return 0;
        if (psno[20]!='-') return 0;

        memcpy(sno,psno,6);
        memcpy(sno+6,psno+7,6);
        memcpy(sno+12,psno+14,6);
        memcpy(sno+18,psno+21,6);

        memset(orginfo,0,sizeof(orginfo));
        for (i=23; i>19; i--)
        {
                for (j=19; j>0; j--)
                        orginfo[j]=(orginfo[j] << 4) | (orginfo[j-1] >> 4);
                orginfo[0] <<=4;
                if (p=(RS_UCHAR *)strchr(TranTableN,sno[i]))
                {
                        j=(RS_INT)((RS_CHAR*)p-TranTableN);
                        if (j>15) return 0;
                        orginfo[0] |=j;
                }
                else
                        return 0;
        }
        for (i=19; i>=0; i--)
        {
                for (j=19; j>0; j--)
                        orginfo[j]=(orginfo[j] << 5) | (orginfo[j-1] >> 3);
                orginfo[0] <<=5;
                if (p=(RS_UCHAR *)strchr(TranTableN,sno[i]))
                {
                        j=(RS_INT)((RS_CHAR*)p-TranTableN);
                        orginfo[0] |=j;
                }
                else
                        return 0;
        }


        for (i=2; i<7; i+=2)
        {
                orginfo[i] ^= orginfo[0];
                orginfo[i+1] ^= orginfo[1];
        }
        ush=0;
        for (i=2; i<13; i+=2)
                ush +=orginfo[i]+(orginfo[i+1]<<8);
        ush += orginfo[14];
        if ((ush & 0xff) != orginfo[0])
                return 0;
        RS_BYTE bUsh = ush >> 8;
//	if ((ush >> 8) != orginfo[1])
        if ( bUsh != orginfo[1])
                return 0;
        if (!(orginfo[14] & 8))
                return 0;
        orginfo[14] &=7;

        pNetSN->Btch=(orginfo[14]<<12)+(orginfo[13]<<4)+(orginfo[12]>>4);
        pNetSN->Clnt=orginfo[2]+(orginfo[3]<<8);
        pNetSN->Svr=orginfo[4];
        pNetSN->SysCnt=orginfo[5];
        pNetSN->VerType=orginfo[6]+(orginfo[7]<<8);

        // Add by Donghx 2002-5-31 17:36:53 <
        // 旧的网吧版向新的版本映射 [5/31/2002]
        if ( pNetSN->VerType == 3 )
        {
                pNetSN->VerType = 25;
                pNetSN->lNetOrPc = 2;
                pNetSN->lEditionType = 2;
        }

        // Add by Donghx 2002-5-31 17:36:53 >


        // Add by WYH 2002-3-2 14:16:24 <
        orginfo[8] ^= SNoKey;
        orginfo[9] ^= SNoKey>>8;
        orginfo[10] ^= SNoKey>>16;
        orginfo[11] ^= SNoKey>>24;

        orginfo[11] ^=orginfo[9];
        orginfo[10] ^=orginfo[8];
        orginfo[9] ^=orginfo[8];
        orginfo[8] ^=(orginfo[8] & 0xf) << 4;
        orginfo[8] ^=(orginfo[8] & 0x3) << 2;
        orginfo[8] ^=(orginfo[8] & 0x1) << 1;
        RS_LONG aa =orginfo[8] + (orginfo[9]<<8) + (orginfo[10]<<16) + ((orginfo[11] & 0xf)<<24);
        pNetSN->SNo=orginfo[8] + (orginfo[9]<<8) + (orginfo[10]<<16) + ((orginfo[11] & 0xf)<<24);
        // Add by WYH 2002-3-2 14:16:24 >

        return 1;
}


// 第二次修改的函数 [4/11/2002]
RS_BOOL CCheckSn::DecSNO_3( RS_CHAR *aszSerialNo, RS_INT nSizeSCN, RS_CHAR *abSource, RS_INT nSizeSource, RS_CHAR *ErrDescription, RS_INT nSizeErr)
{
        const RS_CHAR TranTableN[36] = "0123456789ABCDEFGHIJKLMNPQRSTUVWXYZ";

        RS_BOOL bRet = RS_TRUE;
        RS_UINT i = 0;
        RS_UINT j = 0;
        RS_CHAR aszTempSCN[25] = { 0 };
        RS_BYTE abInfo[15] = { 0 };

        // 检查数据的合法性 [4/1/2002]
        if ( nSizeSCN != 27 || nSizeSource != 15 || nSizeErr != 128 )
        {
                return RS_FALSE;
        }
        // 去掉序列号中的'_' [4/1/2002]
        for ( i = 0; i < 27; i++ )
        {
                if ( aszSerialNo[i] != '-' )
                        aszTempSCN[j++] = aszSerialNo[i];

        }

        MyTrace("In  SeriNumber: %s\n", aszTempSCN);

        if ( strlen(aszTempSCN) != 24 )
        {
                return RS_FALSE;
        }

        // 第十六步 去出批号信息 [4/1/2002]
        RS_BYTE bTemp_16_0 = 0xFF;
        RS_BYTE bTemp_16_1 = 0xFF;
        RS_BYTE bTemp_16_2 = 0xFF;
        RS_BYTE bTemp_16_3 = 0xFF;

        for ( i = 0; i < 35; i++ )
        {
                if ( TranTableN[i] == aszTempSCN[20] )
                        bTemp_16_0 = i;
                if ( TranTableN[i] == aszTempSCN[21] )
                        bTemp_16_1 = i;
                if ( TranTableN[i] == aszTempSCN[22] )
                        bTemp_16_2 = i;
                if ( TranTableN[i] == aszTempSCN[23] )
                        bTemp_16_3 = i;
        }

        // Add by Donghx 2002-5-22 13:21:08 <
        if (  bTemp_16_0 == 0xFF ||
          bTemp_16_1 == 0xFF ||
                  bTemp_16_2 == 0xFF ||
                  bTemp_16_3 == 0xFF )
        {
                return RS_FALSE;
        }

        // Add by Donghx 2002-5-22 13:21:08 >

        abInfo[0] = bTemp_16_3;
        abInfo[0] <<= 4;
        abInfo[0] += bTemp_16_2;
        abInfo[1] = bTemp_16_1;
        abInfo[1] <<= 4;
        abInfo[1] += bTemp_16_0;

        // 第十五步 第十四步 还原出信息的长串 [4/1/2002]
        RS_BYTE bTemp_15_1[21] = { 0 };
        RS_UINT unTemp_15_2 = 0;
        RS_UINT unTemp_15_3 = 0;
        RS_BYTE bTemp_15_4 = 0;

        for ( i = 0; i < 20; i++ )
        {
                bTemp_15_1[i] = 0xFF;
        }

        RS_LONG lRes = 0;
        for ( i = 0; i < 20; i++ )
        {
                if ( aszTempSCN[i] == 'O' )
                        aszTempSCN[i] = '0';
                for ( j = 0; j < 35; j++ )
                {
                        if ( aszTempSCN[i] == TranTableN[j] )
                        {
                                bTemp_15_1[i] = j;
                                lRes += j;
                                break;
                        }
                }
        }
        if ( 0 == lRes )
        {
                return RS_FALSE;
        }

        for ( i = 0; i < 20; i++ )
        {
                if ( bTemp_15_1[i] == 0xFF )
                {
                        return RS_FALSE;
                }
        }

        for ( i = 1; i < 21; i++ )
        {
                // 计算出在哪一个字节 [3/27/2002]
                unTemp_15_2 = 5 * i / 8;
                // 计算出在哪一位 [3/27/2002]
                unTemp_15_3 = 5 * i % 8;
                bTemp_15_4 = 0;

                if ( unTemp_15_3 >= 5 )
                {
                        abInfo[2+unTemp_15_2] += ( bTemp_15_1[i-1] << ( 8 - unTemp_15_3 ) );
                }
                else
                {
                        abInfo[1+unTemp_15_2] += bTemp_15_1[i-1] >> unTemp_15_3;
                        abInfo[2+unTemp_15_2] += bTemp_15_1[i-1] << ( 8 - unTemp_15_3 );
                }

        }
        abInfo[14] >>= 4;

        MyTrace( "Check  14 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第十三步  [4/1/2002]
        for ( i = 2; i < 12; i += 2 )
        {
                abInfo[i] ^= abInfo[13];
                abInfo[i+1] ^= abInfo[14];
        }

        MyTrace("Check  13 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第十二步  [4/1/2002]
        for ( i = 2; i < 11; i++ )
        {
                abInfo[i] ^= abInfo[11];
        }

        MyTrace(  "Check  12 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第十一步  [4/1/2002]
        RS_BYTE bTemp_11_1 = 0;

        for ( i = 2; i < 14; i++ )
        {
                bTemp_11_1 += abInfo[i];
        }
        bTemp_11_1 <<= 4;
        bTemp_11_1 >>= 4;

        if ( bTemp_11_1 != abInfo[14] )
        {
                return RS_FALSE;
        }

        // 第十步  [4/1/2002]
        RS_WORD wTemp_10_1 = 0;
        RS_WORD wTemp_10_2 = 0;
        RS_BYTE bTemp_10_3 = 0;
        RS_BYTE bTemp_10_4 = 0;

        for ( i = 2; i < 12; i += 2 )
        {
                wTemp_10_1 = abInfo[i];
                wTemp_10_1 <<= 8;
                wTemp_10_1 += abInfo[i+1];

                wTemp_10_2 += wTemp_10_1;
        }
        bTemp_10_3 = wTemp_10_2 >> 8;
        bTemp_10_4 = wTemp_10_2 & 0xFF;

        if ( bTemp_10_4 != abInfo[12] || bTemp_10_3 != abInfo[13] )
        {
                return RS_FALSE;
        }

        // 第九步  [4/1/2002]

        abInfo[10] ^= abInfo[2];
        abInfo[11] ^= abInfo[3];

        MyTrace( "Check  09 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第八步  [4/1/2002]
        abInfo[6] ^= abInfo[2];
        abInfo[7] ^= abInfo[3];
        abInfo[8] ^= abInfo[4];
        abInfo[9] ^= abInfo[5];

        MyTrace( "Check  08 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第七步  [4/1/2002]
        abInfo[4] ^= abInfo[2];
        abInfo[5] ^= abInfo[3];

        MyTrace( "Check  07 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第六步  [4/1/2002]
        abInfo[3] ^= abInfo[2];

        MyTrace( "Check  06 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);


        // 第五步  [4/1/2002]
        RS_BYTE bTemp_05_1 = 0;
        RS_BYTE bTemp_05_2 = 0;

        bTemp_05_1 = abInfo[2] >> 4;
        bTemp_05_2 = abInfo[2] & 0xF;

        bTemp_05_2 ^= bTemp_05_1;
        abInfo[2] &= 0xF0;
        abInfo[2] += bTemp_05_2;

        MyTrace( "Check  05 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);


        // 第四步  [4/1/2002]
        RS_BYTE bTemp_04_1 = 0;
        RS_BYTE bTemp_04_2 = 0;

        bTemp_04_1 = abInfo[2];
        bTemp_04_2 = abInfo[2];
        bTemp_04_1 >>= 6;
        bTemp_04_2 <<= 2;
        bTemp_04_2 >>= 6;
        bTemp_04_1 ^= bTemp_04_2;
        bTemp_04_1 <<= 4;
        abInfo[2] &= 0xCF;
        abInfo[2] |= bTemp_04_1;

        MyTrace( "Check  04 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);


        // 第三步  [4/1/2002]
        RS_BYTE bTemp_03_1 = 0;
        RS_BYTE bTemp_03_2 = 0;

        bTemp_03_1 = abInfo[2];
        bTemp_03_2 = abInfo[2];

        bTemp_03_1 <<= 1;
        bTemp_03_1 >>= 7;
        bTemp_03_2 >>= 7;

        bTemp_03_2 ^= bTemp_03_1;
        bTemp_03_2 <<= 6;
        abInfo[2] &= 0xBF;
        abInfo[2] |= bTemp_03_2;

        MyTrace( "Check  03 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        for ( i = 0; i < 15; i++ )
                abSource[i] = abInfo[i];

        return bRet;

}

// 第二次修改的函数 [4/11/2002]
RS_BOOL CCheckSn::DecSNO_4( RS_CHAR *aszSerialNo, RS_INT nSizeSCN, RS_CHAR *abSource, RS_INT nSizeSource, RS_CHAR *ErrDescription, RS_INT nSizeErr)
{
        const RS_CHAR TranTableN[36] = "0123456789ABCDEFGHIJKLMNPQRSTUVWXYZ";

        RS_BOOL bRet = RS_TRUE;
        RS_UINT i = 0;
        RS_UINT j = 0;
        RS_CHAR aszTempSCN[25] = { 0 };
        RS_BYTE abInfo[15] = { 0 };

        // 检查数据的合法性 [4/1/2002]
        if ( nSizeSCN != 27 || nSizeSource != 15 || nSizeErr != 128 )
        {
                return RS_FALSE;
        }
        // 去掉序列号中的'_' [4/1/2002]
        for ( i = 0; i < 27; i++ )
        {
                if ( aszSerialNo[i] != '-' )
                        aszTempSCN[j++] = aszSerialNo[i];

        }
        MyTrace( "In  SeriNumber: %s\n", aszTempSCN);

        if ( strlen(aszTempSCN) != 24 )
        {
                return RS_FALSE;
        }

        // 第十六步 去出批号信息 [4/1/2002]
        RS_BYTE bTemp_16_0 = 0xFF;
        RS_BYTE bTemp_16_1 = 0xFF;
        RS_BYTE bTemp_16_2 = 0xFF;
        RS_BYTE bTemp_16_3 = 0xFF;

        for ( i = 0; i < 35; i++ )
        {
                if ( TranTableN[i] == aszTempSCN[20] )
                        bTemp_16_0 = i;
                if ( TranTableN[i] == aszTempSCN[21] )
                        bTemp_16_1 = i;
                if ( TranTableN[i] == aszTempSCN[22] )
                        bTemp_16_2 = i;
                if ( TranTableN[i] == aszTempSCN[23] )
                        bTemp_16_3 = i;
        }

        // Add by Donghx 2002-5-22 13:21:08 <
        if (  bTemp_16_0 == 0xFF ||
          bTemp_16_1 == 0xFF ||
                  bTemp_16_2 == 0xFF ||
                  bTemp_16_3 == 0xFF )
        {
                return RS_FALSE;
        }

        // Add by Donghx 2002-5-22 13:21:08 >

        abInfo[0] = bTemp_16_3;
        abInfo[0] <<= 4;
        abInfo[0] += bTemp_16_2;
        abInfo[1] = bTemp_16_1;
        abInfo[1] <<= 4;
        abInfo[1] += bTemp_16_0;

        // 第十五步 第十四步 还原出信息的长串 [4/1/2002]
        RS_BYTE bTemp_15_1[21] = { 0 };
        RS_UINT unTemp_15_2 = 0;
        RS_UINT unTemp_15_3 = 0;
        RS_BYTE bTemp_15_4 = 0;

        // Add by Donghx 2002-5-22 13:36:43 <
        for ( i = 0; i < 20; i++ )
        {
                bTemp_15_1[i] = 0xFF;
        }
        // Add by Donghx 2002-5-22 13:36:43 >


        RS_LONG lRes = 0;
        for ( i = 0; i < 20; i++ )
        {
                if ( aszTempSCN[i] == 'O' )
                        aszTempSCN[i] = '0';
                for ( j = 0; j < 35; j++ )
                {
                        if ( aszTempSCN[i] == TranTableN[j] )
                        {
                                bTemp_15_1[i] = j;
                                lRes += j;
                                break;
                        }
                }
        }
        if ( 0 == lRes )
        {
                return RS_FALSE;
        }

        for ( i = 0; i < 20; i++ )
        {
                if ( bTemp_15_1[i] == 0xFF )
                {
                        return RS_FALSE;
                }
        }

        for ( i = 1; i < 21; i++ )
        {
                // 计算出在哪一个字节 [3/27/2002]
                unTemp_15_2 = 5 * i / 8;
                // 计算出在哪一位 [3/27/2002]
                unTemp_15_3 = 5 * i % 8;
                bTemp_15_4 = 0;

                if ( unTemp_15_3 >= 5 )
                {
                        abInfo[2+unTemp_15_2] += ( bTemp_15_1[i-1] << ( 8 - unTemp_15_3 ) );
                }
                else
                {
                        abInfo[1+unTemp_15_2] += bTemp_15_1[i-1] >> unTemp_15_3;
                        abInfo[2+unTemp_15_2] += bTemp_15_1[i-1] << ( 8 - unTemp_15_3 );
                }

        }
        abInfo[14] >>= 4;

        MyTrace( "Check  14 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第十三步  [4/1/2002]
        for ( i = 2; i < 12; i += 2 )
        {
                abInfo[i] ^= abInfo[13];
                abInfo[i+1] ^= abInfo[14];
        }

        MyTrace( "Check  13 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);


        // 第十二步  [4/1/2002]
        for ( i = 2; i < 11; i++ )
        {
                abInfo[i] ^= abInfo[11];
        }

        MyTrace( "Check  12 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

//	// 第十一步  [4/1/2002]
//	RS_BYTE bTemp_11_1 = 0;
//
//	for ( i = 2; i < 14; i++ )
//	{
//		bTemp_11_1 += abInfo[i];
//	}
//	bTemp_11_1 <<= 4;
//	bTemp_11_1 >>= 4;
//
//	if ( bTemp_11_1 != abInfo[14] )
//	{
//		return RS_FALSE;
//	}

        // 第十步  [4/1/2002]
        RS_WORD wTemp_10_1 = 0;
        RS_WORD wTemp_10_2 = 0;
        RS_BYTE bTemp_10_3 = 0;
        RS_BYTE bTemp_10_4 = 0;
        RS_BYTE bTemp_10_5 = 0;

        for ( i = 2; i < 12; i += 2 )
        {
                wTemp_10_1 = abInfo[i];
                wTemp_10_1 <<= 8;
                wTemp_10_1 += abInfo[i+1];
                wTemp_10_2 += wTemp_10_1;
        }

        wTemp_10_2 &= 0x3FFF;

        bTemp_10_3 = ( wTemp_10_2 & 0xF000 )  >> 12;
        bTemp_10_4 = wTemp_10_2 & 0xFF;
        bTemp_10_5 = ( wTemp_10_2 & 0xF00 ) >> 8;

        if (  bTemp_10_3 != ( abInfo[12] & 0x3 ) || bTemp_10_4 != abInfo[13] || bTemp_10_5 != abInfo[14] )
        {
                return RS_FALSE;
        }

        // 第九步  [4/1/2002]

        abInfo[10] ^= abInfo[2];
        abInfo[11] ^= abInfo[3];

        MyTrace( "Check  09 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第八步  [4/1/2002]
        abInfo[6] ^= abInfo[2];
        abInfo[7] ^= abInfo[3];
        abInfo[8] ^= abInfo[4];
        abInfo[9] ^= abInfo[5];

        MyTrace( "Check  08 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第七步  [4/1/2002]
        abInfo[4] ^= abInfo[2];
        abInfo[5] ^= abInfo[3];

        MyTrace( "Check  07 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第六步  [4/1/2002]
        abInfo[3] ^= abInfo[2];

        MyTrace( "Check  06 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第五步  [4/1/2002]
        RS_BYTE bTemp_05_1 = 0;
        RS_BYTE bTemp_05_2 = 0;

        bTemp_05_1 = abInfo[2] >> 4;
        bTemp_05_2 = abInfo[2] & 0xF;

        bTemp_05_2 ^= bTemp_05_1;
        abInfo[2] &= 0xF0;
        abInfo[2] += bTemp_05_2;

        MyTrace( "Check  05 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);


        // 第四步  [4/1/2002]
        RS_BYTE bTemp_04_1 = 0;
        RS_BYTE bTemp_04_2 = 0;

        bTemp_04_1 = abInfo[2];
        bTemp_04_2 = abInfo[2];
        bTemp_04_1 >>= 6;
        bTemp_04_2 <<= 2;
        bTemp_04_2 >>= 6;
        bTemp_04_1 ^= bTemp_04_2;
        bTemp_04_1 <<= 4;
        abInfo[2] &= 0xCF;
        abInfo[2] |= bTemp_04_1;

        MyTrace( "Check  04 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第三步  [4/1/2002]
        RS_BYTE bTemp_03_1 = 0;
        RS_BYTE bTemp_03_2 = 0;

        bTemp_03_1 = abInfo[2];
        bTemp_03_2 = abInfo[2];

        bTemp_03_1 <<= 1;
        bTemp_03_1 >>= 7;
        bTemp_03_2 >>= 7;

        bTemp_03_2 ^= bTemp_03_1;
        bTemp_03_2 <<= 6;
        abInfo[2] &= 0xBF;
        abInfo[2] |= bTemp_03_2;

        MyTrace( "Check  03 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);


        for ( i = 0; i < 15; i++ )
                abSource[i] = abInfo[i];

        return bRet;

}

//第五次修改算法，加入批号参加运算的代码
RS_BOOL CCheckSn::DecSNO_5( RS_CHAR *aszSerialNo, RS_INT nSizeSCN, RS_CHAR *abSource, RS_INT nSizeSource, RS_CHAR *ErrDescription, RS_INT nSizeErr)
{
        const RS_CHAR TranTableN[36] = "0123456789ABCDEFGHIJKLMNPQRSTUVWXYZ";

        RS_BOOL bRet = RS_TRUE;
        RS_UINT i = 0;
        RS_UINT j = 0;
        RS_CHAR aszTempSCN[25] = { 0 };
        RS_BYTE abInfo[15] = { 0 };

        // 检查数据的合法性 [4/1/2002]
        if ( nSizeSCN != 27 || nSizeSource != 15 || nSizeErr != 128 )
        {
                return RS_FALSE;
        }
        // 去掉序列号中的'_' [4/1/2002]
        for ( i = 0; i < 27; i++ )
        {
                if ( aszSerialNo[i] != '-' )
                        aszTempSCN[j++] = aszSerialNo[i];

        }
        MyTrace( "In  SeriNumber: %s\n", aszTempSCN);

        if ( strlen(aszTempSCN) != 24 )
        {
                return RS_FALSE;
        }

        // 第十六步 去出批号信息 [4/1/2002]
        RS_BYTE bTemp_16_0 = 0xFF;
        RS_BYTE bTemp_16_1 = 0xFF;
        RS_BYTE bTemp_16_2 = 0xFF;
        RS_BYTE bTemp_16_3 = 0xFF;

        for ( i = 0; i < 35; i++ )
        {
                if ( TranTableN[i] == aszTempSCN[20] )
                        bTemp_16_0 = i;
                if ( TranTableN[i] == aszTempSCN[21] )
                        bTemp_16_1 = i;
                if ( TranTableN[i] == aszTempSCN[22] )
                        bTemp_16_2 = i;
                if ( TranTableN[i] == aszTempSCN[23] )
                        bTemp_16_3 = i;
        }

        // Add by Donghx 2002-5-22 13:21:08 <
        if (  bTemp_16_0 == 0xFF ||
          bTemp_16_1 == 0xFF ||
                  bTemp_16_2 == 0xFF ||
                  bTemp_16_3 == 0xFF )
        {
                return RS_FALSE;
        }

        // Add by Donghx 2002-5-22 13:21:08 >

        abInfo[0] = bTemp_16_3;
        abInfo[0] <<= 4;
        abInfo[0] += bTemp_16_2;
        abInfo[1] = bTemp_16_1;
        abInfo[1] <<= 4;
        abInfo[1] += bTemp_16_0;

        // 第十五步 第十四步 还原出信息的长串 [4/1/2002]
        RS_BYTE bTemp_15_1[21] = { 0 };
        RS_UINT unTemp_15_2 = 0;
        RS_UINT unTemp_15_3 = 0;
        RS_BYTE bTemp_15_4 = 0;

        // Add by Donghx 2002-5-22 13:36:43 <
        for ( i = 0; i < 20; i++ )
        {
                bTemp_15_1[i] = 0xFF;
        }
        // Add by Donghx 2002-5-22 13:36:43 >


        RS_LONG lRes = 0;
        for ( i = 0; i < 20; i++ )
        {
                if ( aszTempSCN[i] == 'O' )
                        aszTempSCN[i] = '0';
                for ( j = 0; j < 35; j++ )
                {
                        if ( aszTempSCN[i] == TranTableN[j] )
                        {
                                bTemp_15_1[i] = j;
                                lRes += j;
                                break;
                        }
                }
        }
        if ( 0 == lRes )
        {
                return RS_FALSE;
        }

        for ( i = 0; i < 20; i++ )
        {
                if ( bTemp_15_1[i] == 0xFF )
                {
                        return RS_FALSE;
                }
        }

        for ( i = 1; i < 21; i++ )
        {
                // 计算出在哪一个字节 [3/27/2002]
                unTemp_15_2 = 5 * i / 8;
                // 计算出在哪一位 [3/27/2002]
                unTemp_15_3 = 5 * i % 8;
                bTemp_15_4 = 0;

                if ( unTemp_15_3 >= 5 )
                {
                        abInfo[2+unTemp_15_2] += ( bTemp_15_1[i-1] << ( 8 - unTemp_15_3 ) );
                }
                else
                {
                        abInfo[1+unTemp_15_2] += bTemp_15_1[i-1] >> unTemp_15_3;
                        abInfo[2+unTemp_15_2] += bTemp_15_1[i-1] << ( 8 - unTemp_15_3 );
                }

        }
        abInfo[14] >>= 4;

        MyTrace( "Check  14 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);


        // 第十三步  [4/1/2002]
        for ( i = 2; i < 12; i += 2 )
        {
                abInfo[i] ^= abInfo[13];
                abInfo[i+1] ^= abInfo[14];
        }

        MyTrace( "Check  13 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第十二步  [4/1/2002]
        for ( i = 2; i < 11; i++ )
        {
                abInfo[i] ^= abInfo[11];
        }

        MyTrace( "Check  12 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

//	// 第十一步  [4/1/2002]
//	RS_BYTE bTemp_11_1 = 0;
//
//	for ( i = 2; i < 14; i++ )
//	{
//		bTemp_11_1 += abInfo[i];
//	}
//	bTemp_11_1 <<= 4;
//	bTemp_11_1 >>= 4;
//
//	if ( bTemp_11_1 != abInfo[14] )
//	{
//		return RS_FALSE;
//	}

        // 第十步  [4/1/2002]
        RS_WORD wTemp_10_1 = 0;
        RS_WORD wTemp_10_2 = 0;
        RS_BYTE bTemp_10_3 = 0;
        RS_BYTE bTemp_10_4 = 0;
        RS_BYTE bTemp_10_5 = 0;

// Modified by LauranceZhang 2003-9-20 18:32:01 <
/*
        for ( i = 2; i < 12; i += 2 )
        {
                wTemp_10_1 = abInfo[i];
                wTemp_10_1 <<= 8;
                wTemp_10_1 += abInfo[i+1];
                wTemp_10_2 += wTemp_10_1;
        }
*/
        for ( i = 0; i < 12; i += 2 )
        {
                wTemp_10_1 = abInfo[i];
                wTemp_10_1 <<= 8;
                wTemp_10_1 += abInfo[i+1];
                wTemp_10_2 += wTemp_10_1;
        }
// Modified by LauranceZhang 2003-9-20 18:32:01 >

        wTemp_10_2 &= 0x3FFF;

        bTemp_10_3 = ( wTemp_10_2 & 0xF000 )  >> 12;
        bTemp_10_4 = wTemp_10_2 & 0xFF;
        bTemp_10_5 = ( wTemp_10_2 & 0xF00 ) >> 8;

        if (  bTemp_10_3 != ( abInfo[12] & 0x3 ) || bTemp_10_4 != abInfo[13] || bTemp_10_5 != abInfo[14] )
        {
                return RS_FALSE;
        }

        // 第九步  [4/1/2002]

        abInfo[10] ^= abInfo[2];
        abInfo[11] ^= abInfo[3];

        MyTrace( "Check  09 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第八步  [4/1/2002]
        abInfo[6] ^= abInfo[2];
        abInfo[7] ^= abInfo[3];
        abInfo[8] ^= abInfo[4];
        abInfo[9] ^= abInfo[5];

        MyTrace( "Check  08 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第七步  [4/1/2002]
        abInfo[4] ^= abInfo[2];
        abInfo[5] ^= abInfo[3];

        MyTrace( "Check  07 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);
        // 第六步  [4/1/2002]
        abInfo[3] ^= abInfo[2];

        MyTrace( "Check  06 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第五步  [4/1/2002]
        RS_BYTE bTemp_05_1 = 0;
        RS_BYTE bTemp_05_2 = 0;

        bTemp_05_1 = abInfo[2] >> 4;
        bTemp_05_2 = abInfo[2] & 0xF;

        bTemp_05_2 ^= bTemp_05_1;
        abInfo[2] &= 0xF0;
        abInfo[2] += bTemp_05_2;

        MyTrace( "Check  05 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第四步  [4/1/2002]
        RS_BYTE bTemp_04_1 = 0;
        RS_BYTE bTemp_04_2 = 0;

        bTemp_04_1 = abInfo[2];
        bTemp_04_2 = abInfo[2];
        bTemp_04_1 >>= 6;
        bTemp_04_2 <<= 2;
        bTemp_04_2 >>= 6;
        bTemp_04_1 ^= bTemp_04_2;
        bTemp_04_1 <<= 4;
        abInfo[2] &= 0xCF;
        abInfo[2] |= bTemp_04_1;

        MyTrace( "Check  04 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        // 第三步  [4/1/2002]
        RS_BYTE bTemp_03_1 = 0;
        RS_BYTE bTemp_03_2 = 0;

        bTemp_03_1 = abInfo[2];
        bTemp_03_2 = abInfo[2];

        bTemp_03_1 <<= 1;
        bTemp_03_1 >>= 7;
        bTemp_03_2 >>= 7;

        bTemp_03_2 ^= bTemp_03_1;
        bTemp_03_2 <<= 6;
        abInfo[2] &= 0xBF;
        abInfo[2] |= bTemp_03_2;

        MyTrace( "Check  03 : %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
                abInfo[0], abInfo[1],abInfo[2],abInfo[3], abInfo[4],
                abInfo[5], abInfo[6], abInfo[7],abInfo[8], abInfo[9],
                abInfo[10], abInfo[11],abInfo[12],abInfo[13],abInfo[14]);

        for ( i = 0; i < 15; i++ )
                abSource[i] = abInfo[i];

        return bRet;

}

void CCheckSn::InitParam()
{
        //初始化参数
        m_bIsChecked = RS_FALSE;
        m_dwFirstErrCode = ERR_SN_VAL_SUCCESS;
        m_dwLastErrCode = ERR_SN_VAL_SUCCESS;
        memset( &m_stSnInfo, 0, sizeof(STSNINFO) );
        memset( m_szSnInfo, 0, 16);

        memset(m_aszSn,0,sizeof(m_aszSn));
        m_nSnSize = 0;
        m_dwSnKey = 0;
}

// 取返回的结果 [4/11/2002]
RS_BOOL CCheckSn::GetSnInfo( RS_CHAR *Source )
{
        if(!m_bIsChecked)
                return RS_FALSE;

        memcpy( Source, m_szSnInfo, 16 );
        return RS_TRUE;
}
RS_BOOL CCheckSn::SetInfo( RS_UCHAR *pSCNInfo )
{

        RS_LONG lTemp = 0;

        m_stSnInfo.VerType = pSCNInfo[2];

        m_stSnInfo.SNo = pSCNInfo[3];
        m_stSnInfo.SNo <<= 8;
        m_stSnInfo.SNo += pSCNInfo[4];
        m_stSnInfo.SNo <<= 8;
        m_stSnInfo.SNo += pSCNInfo[5];

        m_stSnInfo.lAreaID = pSCNInfo[6] >> 4;

        if ( m_stSnInfo.Btch > 32768 )
        {
                m_stSnInfo.lNetOrPc = 2;
                switch( m_stSnInfo.VerType )
                {
                case 0:		//多中心
                case 1:		//
                case 25:	//单中心
                case 24:	//超级中心

                        m_stSnInfo.SysCnt = ( pSCNInfo[6] & 0x8 ) >> 3;

                        m_stSnInfo.lLimitTime = ( pSCNInfo[6] & 0x6) >> 1;

                        m_stSnInfo.lEditionType = ( pSCNInfo[6] & 0x1 );
                        m_stSnInfo.lEditionType <<= 4;
                        m_stSnInfo.lEditionType += ( pSCNInfo[7] & 0xF0 ) >> 4;

                        m_stSnInfo.Svr = (pSCNInfo[7] & 0xF );
                        m_stSnInfo.Svr <<= 8;
                        m_stSnInfo.Svr += pSCNInfo[8];

                        m_stSnInfo.Clnt = pSCNInfo[9];
                        m_stSnInfo.Clnt <<= 8;
                        m_stSnInfo.Clnt += pSCNInfo[10];

                        break;
                case 6:		//Exchange
                case 7:		//Domino
                        m_stSnInfo.lLimitTime = ( pSCNInfo[6] & 15 ) >> 2;
                        m_stSnInfo.Clnt = ( pSCNInfo[8] & 31 );
                        m_stSnInfo.Clnt <<= 8;
                        m_stSnInfo.Clnt += pSCNInfo[9];
                        m_stSnInfo.Clnt <<= 8;
                        m_stSnInfo.Clnt += pSCNInfo[10];
                        break;
                case 21:	//邮件服务器
                case 28:	//硬件中间件
                        m_stSnInfo.lLimitTime = ( pSCNInfo[6] & 15 ) >> 2;
                        m_stSnInfo.lHardWareID = ( pSCNInfo[6] & 3) * 4 + ( pSCNInfo[7] & 192 ) / 64;
                        m_stSnInfo.lSystemID = ( pSCNInfo[7] & 60 ) / 4;
                        m_stSnInfo.lSystemVer = ( pSCNInfo[7] & 3 ) * 8 + ( pSCNInfo[8] & 224 ) / 32;
                        m_stSnInfo.Clnt = ( pSCNInfo[8] & 31 );
                        m_stSnInfo.Clnt <<= 8;
                        m_stSnInfo.Clnt += pSCNInfo[9];
                        m_stSnInfo.Clnt <<= 8;
                        m_stSnInfo.Clnt += pSCNInfo[10];

                        break;
                case 22:	//UNIX
                case 27:	//UNIX_KDE版
                        m_stSnInfo.lLimitTime = ( pSCNInfo[6] & 15 ) >> 2;

                        m_stSnInfo.lHardWareID = ( pSCNInfo[6] & 3) * 4 + ( pSCNInfo[7] & 192 ) / 64;
                        m_stSnInfo.lSystemID = ( pSCNInfo[7] & 60 ) / 4;
                        m_stSnInfo.lSystemVer = ( pSCNInfo[7] & 3 ) * 8 + ( pSCNInfo[8] & 224 ) / 32;

                        break;
                case 26:	// novell 版的序列号 [11/27/2003]

                        m_stSnInfo.lLimitTime = ( pSCNInfo[6] & 15 ) >> 2;

                        m_stSnInfo.lHardWareID = ( pSCNInfo[6] & 3) * 4 + ( pSCNInfo[7] & 192 ) / 64;
                        m_stSnInfo.lSystemID = ( pSCNInfo[7] & 60 ) / 4;
                        m_stSnInfo.lSystemVer = ( pSCNInfo[7] & 3 ) * 8 + ( pSCNInfo[8] & 224 ) / 32;

                        break;
                case 23:	//软件中间件
                        m_stSnInfo.lActiveXType = ( pSCNInfo[6] & 15 ) * 64 + ( pSCNInfo[7] & 252 ) / 4;
                        m_stSnInfo.lHardWareID = ( pSCNInfo[7] & 3 ) * 4 + ( pSCNInfo[8] & 192 ) / 64;
                        m_stSnInfo.lSystemID = ( pSCNInfo[8] & 60 ) / 4;
                        m_stSnInfo.lSystemVer = ( pSCNInfo[8] & 3 ) * 8 + ( pSCNInfo[9] & 224 ) / 32;
                        break;
                default:
                        return RS_FALSE;
                }

        }
        else
        {
                m_stSnInfo.lNetOrPc = 1;
                switch( m_stSnInfo.VerType )
                {
                case 0:
                case 1:

                        m_stSnInfo.lOEMID = pSCNInfo[6] & 0xF;
                        m_stSnInfo.lOEMID <<= 8;
                        m_stSnInfo.lOEMID += pSCNInfo[7];
                        m_stSnInfo.lOEMID <<= 4;
                        m_stSnInfo.lOEMID += pSCNInfo[8] >> 4;

                        m_stSnInfo.Clnt = pSCNInfo[8] & 0xF;
                        m_stSnInfo.Clnt <<= 8;
                        m_stSnInfo.Clnt += pSCNInfo[9];
                        m_stSnInfo.Clnt <<= 8;
                        m_stSnInfo.Clnt += pSCNInfo[10];
                        break;

                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 9:
                        // 为了兼容以前出过的版本 [5/22/2002]
                        if ( m_stSnInfo.Btch == CONFIG_SNBATCH_DIVIDELINE2 )
                        {
                                m_stSnInfo.lLimitTime = 5;
                        }
                        else
                        {
                                m_stSnInfo.lLimitTime = pSCNInfo[6] & 0xF;
                        }

                        m_stSnInfo.lOEMID = pSCNInfo[7];
                        m_stSnInfo.lOEMID <<= 4;
                        m_stSnInfo.lOEMID += pSCNInfo[8] >> 4;

                        m_stSnInfo.Clnt = pSCNInfo[8] & 0xF;
                        m_stSnInfo.Clnt <<= 8;
                        m_stSnInfo.Clnt += pSCNInfo[9];
                        m_stSnInfo.Clnt <<= 8;
                        m_stSnInfo.Clnt += pSCNInfo[10];
                        break;
                default:
                        return RS_FALSE;
                }

        }
//	m_stSnInfo.Btch &= 0x7FFF;
        return RS_TRUE;
}
// 用于对序列号进行解码的函数
short CCheckSn::Instr( RS_UCHAR *tar, RS_UCHAR src )
{
        RS_INT i ;
        for( i = 0 ; i < 32 ; i++ )
        {
                if( tar[ i ] == src ) return i ;
        }
        return -1 ;
}

void CCheckSn::SetErrCode(RS_DWORD dwErrCode)
{
        if(m_dwFirstErrCode==0)
                m_dwFirstErrCode = dwErrCode;
        m_dwLastErrCode = dwErrCode;
}
RS_LONG	CCheckSn::GetFirstError()
{
        return m_dwFirstErrCode;
}
RS_LONG	CCheckSn::GetLastError()
{
        return m_dwLastErrCode;
}


