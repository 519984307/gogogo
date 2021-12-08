#ifdef UNIXPLATFORM
    #include <string.h>
    #include <time.h>
#else
    #include "stdafx.h"
    #include "time.h"
#endif

#include "regapi.h"
#include "configimpl.h"
#include "checkvalid.h"
#include "cfgmutex.h"

CConfig::CConfig(const CHAR* pszFileName)
{
        ZeroData();
        m_tree = RS_NULL;
        m_bTreeLoaded = RS_FALSE;
        m_pParent = RS_NULL;
        if(pszFileName)
        {
                m_tree = new CTree;
                if(m_tree)
                {
                        m_tree->m_dwReference = 1;
                        m_tree->InitTree(pszFileName);
                        OPExpireFlag(&g_dwExpiredFlag, RS_FALSE);
                        OPInstallFlag(g_dwInstallFlag, RS_FALSE);
                }
        }
}

RS_VOID CConfig::ZeroData()
{
        memset(m_adwPath, 0, sizeof(RS_DWORD) * RS_MAX_CFGPATH_LEN);
        m_nPosition = 0;

        memset(m_adwUser, 0, sizeof(m_adwUser));
        m_hItem = RS_NULL;
        m_dwAccess = 0;
}

CConfig::~CConfig()
{
        if(m_tree)
        {
// Deleted by LauranceZhang 2004-11-11 19:25:20 <
//因为没有真正实现共享打开，所以结点状态一直保持打开，避免被其他进程关闭的情况
/*
                CloseNode();
*/
// Deleted by LauranceZhang 2004-11-11 19:25:20 >
                if(m_tree->m_dwReference == 1)
                {
                        delete m_tree;
                        m_tree = RS_NULL;
                }
                else
                        m_tree->m_dwReference --;
        }
}

RS_RESULT CConfig::HaveRights(RS_DWORD dwRights, RS_BOOL bLockCheck)
{
        if(!m_tree)
                return ERR_CFG_TREE_ISEMPTY;

        if(!m_hItem)
                return ERR_CFG_NODE_ISNOTOPENED;

        //是否已打开
        if(!((CNode *)m_hItem)->IsStatusMatchRule(CONFIG_STATE_OPENED))
                return ERR_CFG_NODE_ISNOTOPENED;

        if(bLockCheck)
                if(((CNode *)m_hItem)->IsStatusMatchRule(CONFIG_STATE_LOCKED))	//如果已锁定
                        return ERR_CFG_NODE_LOCKED;

        //是否有权限
        if(!((CNode *)m_hItem)->IsAccessMatchRule(dwRights))
                return ERR_CFG_NODE_NORIGHT;

        return ERR_SUCCESS;
}

// 按全路径创建或打开一个结点，一个结点创建成功后自然打开
RS_RESULT CConfig::Create(const RS_DWORD* pdwIdPath, RS_DWORD &dwAccess, const RS_DWORD* pdwUserId)
{
        CMemMutex memMutex(&g_cMutexMem);

        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;
        if(!m_tree)
                return ERR_CFG_TREE_ISEMPTY;

        //检测参数的合法性
        int i=0;
        for(i=0; i<RS_MAX_CFGPATH_LEN; i++)
        {
                if(pdwIdPath[i] == 0)
                        break;
        }
        if(i >= RS_MAX_CFGPATH_LEN)
                return ERR_CFG_PARAM_INVALID;
        //为成员参数赋值
        memcpy(m_adwPath, pdwIdPath, sizeof(RS_DWORD) * (i + 1));
        m_nPosition = i;

        m_dwAccess = dwAccess;
        memcpy(m_adwUser, pdwUserId, sizeof(RS_DWORD) * RS_USERID_LEN);
        RS_DWORD dwRet = ERR_SUCCESS;
        dwRet = m_tree->OpenItem(pdwIdPath, &m_hItem, dwAccess, pdwUserId);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        ((CNode *)m_hItem)->AppendStatus(CONFIG_STATE_IS_BRANCH);
        return dwRet;
}

//创建一个结点,并返回一个对象指针,此函数不影响现有对象
RS_RESULT CConfig::CreateNode(const RS_DWORD* pdwIdPath, RS_DWORD &dwAccess, const RS_DWORD* pdwUserId, IConfig** ppConfig)
{
        CMemMutex memMutex(&g_cMutexMem);
        MyTrace("CreateNode [--->] Path = %x->%x->%x->%x; Access=%x", pdwIdPath[0], pdwIdPath[1], pdwIdPath[2], pdwIdPath[3], dwAccess);

        RS_RESULT dwRet = ERR_SUCCESS;
        if(!m_bTreeLoaded)
        {
                FormatErrorMessage(ERR_CFG_TREE_LOADFAILED, __FILE__, __LINE__, "CreateNode");
                return ERR_CFG_TREE_LOADFAILED;
        }
        if(ppConfig == RS_NULL)		//使用当前对象创建
        {
                dwRet = Create(pdwIdPath, dwAccess, pdwUserId);
                if(ERR_SUCCESS!=dwRet)
                {
                        FormatErrorMessage(dwRet, __FILE__, __LINE__, "CreateNode");
                        return dwRet;
                }
                MyTrace("CreateNode [Ret=%x]Addr=%x, Path = %x->%x->%x->%x; Access=%x", dwRet, this, pdwIdPath[0], pdwIdPath[1], pdwIdPath[2], pdwIdPath[3], dwAccess);
                return dwRet;
        }
        //New 一个新的对象
        CConfig* pConfig = new CConfig;
        if(!pConfig)
        {
                FormatErrorMessage(ERR_CFG_MEM_ALLOCFAILED, __FILE__, __LINE__, "CreateNode");
                return ERR_CFG_MEM_ALLOCFAILED;
        }

        dwRet = pConfig->LoadConfig(m_tree);
        if(dwRet != ERR_SUCCESS)
        {
                delete pConfig;
                pConfig = RS_NULL;
                MyTrace("CreateNode [Error2] Path = %x->%x->%x->%x", dwRet, pdwIdPath[0], pdwIdPath[1], pdwIdPath[2], pdwIdPath[3]);
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "CreateNode");
                return dwRet;
        }

        //创建此结点
        dwRet = pConfig->Create(pdwIdPath, dwAccess, pdwUserId);
        if(ERR_SUCCESS != dwRet)
        {
                delete pConfig;
                pConfig = RS_NULL;
                MyTrace("CreateNode [Error1] Path = %x->%x->%x->%x", dwRet, pdwIdPath[0], pdwIdPath[1], pdwIdPath[2], pdwIdPath[3]);
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "CreateNode");
                return dwRet;
        }

        //返回参数
        *ppConfig = (IConfig*)pConfig;

        MyTrace("CreateNode [Ret=%x]Addr=%x, Path = %x->%x->%x->%x; Access=%x", dwRet, pConfig,  pdwIdPath[0], pdwIdPath[1], pdwIdPath[2], pdwIdPath[3], dwAccess);
        return ERR_SUCCESS;
}

//关闭当前对象结点,可利用此对象继续操作其他结点
RS_RESULT CConfig::CloseNode()
{
        CMemMutex memMutex(&g_cMutexMem);
        MyTrace("CloseNode [--->]Addr=%x, Path = %x->%x->%x->%x", this, m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3]);
        if(!m_bTreeLoaded)
        {
                FormatErrorMessage(ERR_CFG_TREE_LOADFAILED, __FILE__, __LINE__, "CloseNode");
                return ERR_CFG_TREE_LOADFAILED;
        }
        if(!m_tree)
        {
                FormatErrorMessage(ERR_CFG_TREE_ISEMPTY, __FILE__, __LINE__, "CloseNode");
                return ERR_CFG_TREE_ISEMPTY;
        }
        RS_DWORD dwRet = ERR_SUCCESS;
        dwRet = m_tree->CloseItem(&m_hItem);
        if(ERR_SUCCESS!=dwRet)
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "CloseNode");
                return dwRet;
        }

        ZeroData();
        MyTrace("CloseNode [Ret=%x] Path = %x->%x->%x->%x", dwRet, m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3]);
        return dwRet;
}

//删除当前对象结点,成功删除后对象的数据成员都保持对象初始状态
RS_RESULT CConfig::DeleteNode()
{
        CMemMutex memMutex(&g_cMutexMem);
        MyTrace("DeleteNode [--->] Path = %x->%x->%x->%x", m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3]);
        if(!m_bTreeLoaded)
        {
                FormatErrorMessage(ERR_CFG_TREE_LOADFAILED, __FILE__, __LINE__, "DeleteNode");
                return ERR_CFG_TREE_LOADFAILED;
        }

        RS_DWORD dwRet = ERR_SUCCESS;
        dwRet = Query(0);
        if(ERR_SUCCESS != dwRet)
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "DeleteNode");
                return dwRet;
        }

        dwRet = HaveRights(CONFIG_ACCESS_DELETE);
        if(ERR_SUCCESS != dwRet)
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "DeleteNode");
                return dwRet;
        }
        dwRet=m_tree->DeleteItem(m_hItem);
        if(ERR_SUCCESS!=dwRet)
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "DeleteNode");
                return dwRet;
        }
        ZeroData();
        MyTrace("DeleteNode [Ret=%x] Path = %x->%x->%x->%x", dwRet, m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3]);
        return dwRet;
}

//用父对象打开子结点,不管成功与否,当前结点都被关闭
RS_RESULT CConfig::GetChildNode(RS_DWORD dwId, RS_DWORD dwAccess, const RS_DWORD* pdwUserId)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;


        RS_DWORD adwPath[RS_MAX_CFGPATH_LEN] = {0};
        memcpy(adwPath, m_adwPath, sizeof(RS_DWORD) * m_nPosition);
        adwPath[m_nPosition] = dwId;

        //先关闭当前结点
        RS_DWORD dwRet = ERR_SUCCESS;
        dwRet = CloseNode();
        if(ERR_SUCCESS!=dwRet)
                return dwRet;

        dwRet = Create(adwPath, dwAccess, pdwUserId);

        return dwRet;
}

//不是用父对象打开子结点，而是用一个新对象
RS_RESULT CConfig::GetChildNode(RS_DWORD dwId, RS_DWORD &dwAccess, const RS_DWORD* pdwUserId, IConfig** ppConfig)
{
        CMemMutex memMutex(&g_cMutexMem);

        MyTrace("GetChildNode [--->]Addr=%x, Path = %x->%x->%x->%x, ItemNo=%x", this, m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3], dwId);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD adwPath[RS_MAX_CFGPATH_LEN] = {0};
        memcpy(adwPath, m_adwPath, sizeof(RS_DWORD) * m_nPosition);
        adwPath[m_nPosition] = dwId;

        RS_RESULT dwRet = ERR_SUCCESS;
        //用父对象打开子结点,不管成功与否,当前结点都被关闭
        if(RS_NULL==ppConfig)
        {
                //先关闭当前结点
                dwRet = CloseNode();
                if(ERR_SUCCESS != dwRet)
                        return dwRet;
                dwRet = Create(adwPath, dwAccess, pdwUserId);
                if(ERR_SUCCESS != dwRet)
                        return dwRet;
                MyTrace("GetChildNode [Ret=%x]Addr=%x, Path = %x->%x->%x->%x, ItemNo=%x", dwRet, this, m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3], dwId);
                return dwRet;
        }

        CConfig* pConfig = new CConfig;
        if(!pConfig)
                return ERR_CFG_MEM_ALLOCFAILED;

        dwRet = pConfig->LoadConfig(m_tree);
        if(dwRet != ERR_SUCCESS)
        {
                delete pConfig;
                pConfig = RS_NULL;
                return dwRet;
        }

        dwRet = pConfig->Create(adwPath, dwAccess, pdwUserId);
        if(ERR_SUCCESS != dwRet)
        {
                delete pConfig;
                pConfig = RS_NULL;
                return dwRet;
        }

        *ppConfig = (IConfig*)pConfig;

        MyTrace("GetChildNode [Ret=%x]Addr=%x, Path = %x->%x->%x->%x, ItemNo=%x", dwRet, pConfig, m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3], dwId);

        return ERR_SUCCESS;
}

//用父对象打开兄弟结点,不管成功与否,当前结点都被关闭
RS_RESULT CConfig::GetSiblingNode(RS_DWORD dwId, RS_DWORD dwAccess, const RS_DWORD* pdwUserId)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD adwPath[RS_MAX_CFGPATH_LEN] = {0};
        memcpy(adwPath, m_adwPath, sizeof(RS_DWORD) * m_nPosition);
        adwPath[m_nPosition-1] = dwId;

        //先关闭当前结点
        RS_DWORD dwRet = ERR_SUCCESS;
        dwRet = CloseNode();
        if(ERR_SUCCESS!=dwRet)
                return dwRet;

        dwRet = Create(adwPath, dwAccess, pdwUserId);
        return dwRet;
}
//不是用父对象打开子结点，而是用一个新对象
RS_RESULT CConfig::GetSiblingNode(RS_DWORD dwId, RS_DWORD &dwAccess, const RS_DWORD* pdwUserId, IConfig** ppConfig)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD adwPath[RS_MAX_CFGPATH_LEN] = {0};
        memcpy(adwPath, m_adwPath, sizeof(RS_DWORD) * m_nPosition);
        adwPath[m_nPosition-1] = dwId;

        RS_RESULT dwRet = ERR_SUCCESS;
        if(RS_NULL == ppConfig)
        {
                //先关闭当前结点
                dwRet = CloseNode();
                if(ERR_SUCCESS!=dwRet)
                        return dwRet;

                return Create(adwPath, dwAccess, pdwUserId);
        }

        CConfig* pConfig = new CConfig;
        if(!pConfig)
                return ERR_CFG_MEM_ALLOCFAILED;

        dwRet = pConfig->LoadConfig(m_tree);
        if(dwRet != ERR_SUCCESS)
        {
                delete pConfig;
                pConfig = RS_NULL;
                return dwRet;
        }

        dwRet = pConfig->Create(adwPath, dwAccess, pdwUserId);
        if(ERR_SUCCESS != dwRet)
        {
                delete pConfig;
                pConfig = RS_NULL;
                return dwRet;
        }

        *ppConfig = (IConfig*)pConfig;
        return ERR_SUCCESS;
}

RS_RESULT CConfig::GetNodePath(RS_DWORD* pdwPath, RS_DWORD& dwPathLen)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        if(m_nPosition <0 )
                return ERR_CFG_PARAM_INVALID;

        if(dwPathLen <  (m_nPosition + 1))
        {
                dwPathLen = m_nPosition + 1;
                return ERR_CFG_BUFFER_OVERFLOW;
        }

        memcpy(pdwPath, m_adwPath, sizeof(RS_DWORD) * (m_nPosition + 1));
        dwPathLen = m_nPosition;
        return ERR_SUCCESS;
}

//创建一个配置项,要求其父结点必须是打开的
RS_RESULT CConfig::CreateItem(RS_DWORD dwItemId)
{
        CMemMutex memMutex(&g_cMutexMem);
        MyTrace("CreateItem [--->] Path = %x->%x->%x->%x", m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3]);
        if(!m_bTreeLoaded)
        {
                FormatErrorMessage(ERR_CFG_TREE_LOADFAILED, __FILE__, __LINE__, "CreateItem");
                return ERR_CFG_TREE_LOADFAILED;
        }

        RS_DWORD dwRet = ERR_SUCCESS;
        dwRet = HaveRights(CONFIG_ACCESS_CREATE, RS_TRUE);
        if(ERR_SUCCESS != dwRet)
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "CreateItem");
                return dwRet;
        }
        RS_DWORD adwPath[RS_MAX_CFGPATH_LEN] = {0};
        memcpy(adwPath, m_adwPath, sizeof(RS_DWORD) * m_nPosition);
        adwPath[m_nPosition] = dwItemId;

        HITEM hItem = RS_NULL;
        dwRet = m_tree->OpenItem(adwPath, &hItem, m_dwAccess, m_adwUser);
        if(ERR_SUCCESS != dwRet)
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "CreateItem");
                return dwRet;
        }
        ((CNode *)hItem)->ExcludeStatus(CONFIG_STATE_IS_BRANCH);	//类型要设为叶子
        MyTrace("CreateItem [Ret=%x] Path = %x->%x->%x->%x", dwRet, m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3]);
        return dwRet;
}

RS_RESULT CConfig::Delete(RS_DWORD dwId)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwRet = ERR_SUCCESS;
        dwRet = Query(0);
        if(ERR_SUCCESS != dwRet)
        {
                return dwRet;
        }

        dwRet = HaveRights(CONFIG_ACCESS_DELETE);
        if(ERR_SUCCESS != dwRet)
                return dwRet;

        RS_DWORD adwPath[RS_MAX_CFGPATH_LEN] = {0};
        memcpy(adwPath, m_adwPath, sizeof(RS_DWORD) * m_nPosition);
        adwPath[m_nPosition] = dwId;

        //先打开此项，然后进行删除
        HITEM hItem = RS_NULL;
        dwRet = m_tree->OpenItem(adwPath, &hItem, m_dwAccess, m_adwUser);
        if(ERR_SUCCESS != dwRet)
                return dwRet;

        dwRet = m_tree->DeleteItem(hItem);
        if(ERR_SUCCESS != dwRet)
                return dwRet;

        return ERR_SUCCESS;
}

RS_RESULT CConfig::DeleteItem(RS_DWORD dwItemId)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        return Delete(dwItemId);
}

RS_RESULT CConfig::Query(RS_DWORD dwItemId)
{
        HITEM hItem = 0;
        //先查询出此项的句柄
        if(dwItemId != 0)
                m_adwPath[m_nPosition] = dwItemId;
        RS_DWORD dwOpAccess = CONFIG_ACCESS_QUERY;
        RS_DWORD dwRet = m_tree->OpenItem(m_adwPath, &hItem, dwOpAccess, m_adwUser);
        if((ERR_SUCCESS == dwRet) && hItem)	//此项是存在的
                dwRet = ERR_SUCCESS;
        else
                dwRet = ERR_FAILED;
        if(dwItemId != 0)
                m_adwPath[m_nPosition] = 0;

        return dwRet;
}
//打开一个配置项,供配置项的读写操作
RS_RESULT CConfig::OpenItem(RS_DWORD dwItemId, HITEM &hItem, RS_DWORD dwAccess)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwRet = ERR_SUCCESS;

        if(dwItemId == INVALID_ITEMID)	// 这时是结点本身
        {
                hItem = m_hItem;
                return dwRet;
        }
        RS_DWORD adwPath[RS_MAX_CFGPATH_LEN] = {0};
        memcpy(adwPath, m_adwPath, sizeof(RS_DWORD) * m_nPosition);
        adwPath[m_nPosition] = dwItemId;

        // 先打开此项
        dwRet = m_tree->OpenItem(adwPath, &hItem, dwAccess, m_adwUser);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        return dwRet;
}
RS_RESULT CConfig::CloseItem(PHITEM phItem)
{
        CMemMutex memMutex(&g_cMutexMem);
        MyTrace("CConfig::CloseItem == %x", *phItem);
        return m_tree->CloseItem(phItem);
}
//读取数据的入口
RS_RESULT CConfig::GetItemValue(RS_DWORD dwItemId, RS_BYTE* pbyData, RS_DWORD& dwSize)
{
        CMemMutex memMutex(&g_cMutexMem);
        MyTrace("GetItemValue [--->]Addr=%x, Path = %x->%x->%x->%x, ItemID=%x", this, m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3], dwItemId);

        if(!m_bTreeLoaded)
        {
                FormatErrorMessage(ERR_CFG_TREE_LOADFAILED, __FILE__, __LINE__, "GetItemValue");
                return ERR_CFG_TREE_LOADFAILED;
        }


        RS_DWORD dwRet = ERR_SUCCESS;
        RS_DWORD dwDataType = CONFIG_DATA_UNKOWN;
        HITEM hItem = RS_NULL;

        dwRet = OpenItem(dwItemId, hItem, m_dwAccess & ~CONFIG_ACCESS_CREATE);
        if(ERR_SUCCESS != dwRet)
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "GetItemValue");
                return dwRet;
        }
        dwRet = HaveRights(CONFIG_ACCESS_READ);
        if(ERR_SUCCESS != dwRet)
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "GetItemValue");
                return dwRet;
        }
        dwRet = m_tree->GetItemValue(hItem, dwDataType, pbyData, dwSize);
        if(ERR_SUCCESS != dwRet)
        {
                if(dwItemId!=INVALID_ITEMID)
                        CloseItem(&hItem);
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "GetItemValue");
                return dwRet;
        }

        if(dwItemId!=INVALID_ITEMID)
        {
                dwRet = CloseItem(&hItem);
                if(ERR_SUCCESS != dwRet)
                {
                        FormatErrorMessage(dwRet, __FILE__, __LINE__, "GetItemValue");
                        return dwRet;
                }
        }

        dwRet = HookCheck(dwItemId, dwDataType, pbyData, dwSize, RS_FALSE);
        if(ERR_SUCCESS != dwRet)
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "GetItemValue");
                return dwRet;
        }
        MyTrace("GetItemValue [Ret=%x] Path = %x->%x->%x->%x", dwRet, m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3]);
        return dwRet;
}
//**************************************************//
//在此进行序列号的验证,并且在序列号正确的情况下将序列号解析,
//将详细信息自动写入到指定的位置
//**************************************************//
RS_RESULT CConfig::HookCheck(RS_DWORD dwItemId, RS_DWORD &dwDataType, RS_BYTE* pbyData, RS_DWORD &dwSize, RS_BOOL bIsWrite)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwRet = ERR_SUCCESS;

        RS_DWORD adwTmpPath[10] = {0};
        adwTmpPath[0] = 0xFFFFFFFF;
        adwTmpPath[1] = 1;
        RS_DWORD dwDataTypeRead =0;
        RS_DWORD dwIndicator = 0, dwSizeRead = sizeof(RS_DWORD);
        dwRet = SuperGetValue(adwTmpPath, dwDataTypeRead, (RS_BYTE *)&dwIndicator, dwSizeRead);
        if(dwRet!=ERR_SUCCESS)
                return dwRet;
        adwTmpPath[1] = RS_SWAPDWORDEX(dwIndicator);

        if(bIsWrite)
        {
                if( memcmp(adwTmpPath, m_adwPath, sizeof(RS_DWORD)* 2) != 0 )		//不符合条件不做检查
                        return ERR_SUCCESS;
                if(	(m_adwPath[2] == 3) &&
                        ((dwItemId == CONFIG_ITEMNO_PRODUCT_ID)	||
                        (dwItemId == CONFIG_ITEMNO_PRODUCT_ID_UP)	||
                        (dwItemId == CONFIG_ITEMNO_DEMO_COUNT)) )
                        return ERR_FAILED;
        }

        if( m_adwPath[0] != 0x1 && m_adwPath[0] != 0xFFFFFFFF && !g_dwInstallFlag)
        {
                OPInstallFlag(g_dwInstallFlag, RS_FALSE);
                if(!g_dwInstallFlag)			//如果没有安装过则直接返回失败
                {
                        MyTrace("g_dwInstallFlag==%x", g_dwInstallFlag);
                        return ERR_FAILED;
                }
        }

        //各项Hook入口判断
        RS_BOOL bIsInsallSn = RS_FALSE, bIsCheckSn = RS_FALSE, bIsExpireTime = RS_FALSE, bIsAddTime = RS_FALSE;
        RS_BOOL bIsExpireFlag = RS_FALSE, bIsTimeLeft = RS_FALSE;
        if(m_adwPath[2] == 1)
        {
                if(dwItemId == CONFIG_ITEMNO_SN_STRING)			//是安装程序检验序列号
                        bIsInsallSn = RS_TRUE;
        }
        else if(m_adwPath[2] == 0x80000001)
        {
                if(dwItemId == CONFIG_ITEMNO_SN_STRING)			//是单纯检验序列号
                        bIsCheckSn = RS_TRUE;
        }
        else if(m_adwPath[2] == 3)
        {
                if(dwItemId == CONFIG_ITEMNO_TIME_EXPIRED)		//是写过期时间
                        bIsExpireTime = RS_TRUE;
                else if(dwItemId == CONFIG_ITEMNO_TIME_ADD)		//写续费增加的时间段
                        bIsAddTime = RS_TRUE;
                else if(dwItemId == CONFIG_ITEMNO_FLAG_EXPIRED)	//是读过期标志
                        bIsExpireFlag = RS_TRUE;
                else if(dwItemId == CONFIG_ITEMNO_TIME_LEFT)	//是读剩余时间
                        bIsTimeLeft = RS_TRUE;
        }


        //下面是对各项Hook进行检查
        if(bIsInsallSn || bIsCheckSn)
        {
                dwRet = OPCheckSn(dwItemId, dwDataType, pbyData, dwSize, bIsWrite, bIsInsallSn);
                if(ERR_SUCCESS != dwRet)
                        return dwRet;
        }

        if(bIsExpireFlag)
        {
                //这里是得到真正的过期标志
                RS_DWORD dwExpiredFlag = 0;
                dwRet = OPExpireFlag((RS_DWORD *)pbyData, bIsWrite);
                if(ERR_SUCCESS != dwRet)
                        return dwRet;
                if(g_dwExpiredFlag != *(RS_DWORD *)pbyData)
                        g_dwExpiredFlag = *(RS_DWORD *)pbyData;
        }
        if(bIsExpireTime)	//读过期截止时间
        {
                dwRet = OPExpireTime((RS_DWORD *)pbyData, bIsWrite);
                if(ERR_SUCCESS != dwRet)
                        return dwRet;
        }
        if(bIsAddTime)		//冲值
        {
                dwRet = OPAddTime(*((RS_DWORD *)pbyData), bIsWrite);
                if(ERR_SUCCESS != dwRet)
                        return dwRet;
        }

        if(bIsTimeLeft)		//读取过期的剩余时间
        {
                dwRet = OPLeftTime((RS_DWORD *)pbyData, bIsWrite);
                if(ERR_SUCCESS != dwRet)
                        return dwRet;
        }

        //这一项永远在最后, 如过期了则读相应的0x40000000的值
        if(g_dwExpiredFlag && !bIsWrite)
        //修改在DealwithHookExpired中每次都判断是否过期
        //if(!bIsWrite)
        {
                dwRet = DealwithHookExpired(dwItemId, pbyData, dwSize);
                if(ERR_SUCCESS != dwRet)
                        return dwRet;
        }

        return dwRet;
}
RS_RESULT CConfig::OPCheckSn(RS_DWORD dwItemId, RS_DWORD &dwDataType, RS_BYTE* pbyData, RS_DWORD &dwSize, RS_BOOL bIsWrite, RS_BOOL bIsInstall)
{
        if(bIsWrite)
        {
                if(dwSize != sizeof(RS_BYTE)*27)
                        return ERR_CFG_PARAM_INVALID;
                RS_CHAR achSn[30] = {0};
                memcpy(achSn, pbyData, dwSize);		//输入的序列号

                RS_DWORD dwRet = 0;
                dwRet = bIsInstall ? CheckSNDetail(CONFIG_ITEMNO_SN_STRING, achSn, 0) : ERR_SUCCESS;
                if(ERR_SUCCESS != dwRet)
                        return dwRet;

                CCheckSn *pCheckSn = RS_NULL;
                //验证序列号的合法性
                pCheckSn = new CCheckSn;
                if(!pCheckSn)
                        return ERR_CFG_MEM_ALLOCFAILED;
                if( ERR_SN_VAL_SUCCESS != pCheckSn->CheckSn(achSn, dwSize, 0))
                {
                        delete pCheckSn;
                        return ERR_SN_CHECK_FAILED;
                }
                //验证序列号的各项限制
                dwRet = WriteSnInfo(bIsInstall, pCheckSn);
                if(ERR_SUCCESS != dwRet)
                {
                        delete pCheckSn;
                        return dwRet;
                }
                //验证此序列号是否过期，不过期则写入安装信息
                RS_LONG lEndTime = 0;
                if(bIsInstall)
                {
                        if(IsSnExpired(achSn, pCheckSn))
                        {
                                delete pCheckSn;
                                return ERR_SN_CHECK_EXPIRED;
                        }
                }

                delete pCheckSn;
                dwDataType = CONFIG_DATA_STRING;
        }
        return ERR_SUCCESS;
}
RS_RESULT CConfig::ReadProductItemSTRING(RS_DWORD dwItemID, RS_CHAR *pszData, RS_DWORD &dwSize)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwRet = ERR_SUCCESS;
        RS_DWORD adwTmpPath[10] = {0};
        adwTmpPath[0] = 0xFFFFFFFF;
        adwTmpPath[1] = 1;
        RS_DWORD dwDataTypeRead =0, dwIndicator = 0, dwSizeRead = sizeof(RS_DWORD);
        dwRet = SuperGetValue(adwTmpPath, dwDataTypeRead, (RS_BYTE *)&dwIndicator, dwSizeRead);
        if(dwRet!=ERR_SUCCESS)
                return dwRet;
        RS_SWAPDWORDEX(dwIndicator);
        adwTmpPath[1] = dwIndicator;
        adwTmpPath[2] = 3;		//这里是产品配置

        adwTmpPath[3] = dwItemID;
        RS_DWORD dwDataType = 0;
        dwRet = SuperGetValue(adwTmpPath, dwDataType, (RS_BYTE *)pszData, dwSize);

        return dwRet;
}

RS_RESULT CConfig::OPGetInstallPath(RS_CHAR *pszData, RS_DWORD &dwSize)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwRet = ERR_SUCCESS;
        RS_DWORD adwTmpPath[10] = {0};
        adwTmpPath[0] = 1;
        adwTmpPath[1] = 1;
        RS_DWORD dwDataTypeRead =0, dwIndicator = 0, dwSizeRead = sizeof(RS_DWORD);
        dwRet = SuperGetValue(adwTmpPath, dwDataTypeRead, (RS_BYTE *)&dwIndicator, dwSizeRead);
        if(dwRet!=ERR_SUCCESS)
                return dwRet;
        adwTmpPath[1] = RS_SWAPDWORDEX(dwIndicator);
        adwTmpPath[2] = 3;		//这里是安装路径
        adwTmpPath[3] = 0;
        RS_DWORD dwDataType = 0;
        dwRet = SuperGetValue(adwTmpPath, dwDataType, (RS_BYTE *)pszData, dwSize);

        return dwRet;
}

RS_RESULT CConfig::ReadProductItemDWORD(RS_DWORD dwItemID, RS_DWORD &dwValue)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwRet = ERR_SUCCESS;
        RS_DWORD adwTmpPath[10] = {0};
        adwTmpPath[0] = 0xFFFFFFFF;
        adwTmpPath[1] = 1;
        RS_DWORD dwDataTypeRead =0, dwIndicator = 0, dwSizeRead = sizeof(RS_DWORD);
        dwRet = SuperGetValue(adwTmpPath, dwDataTypeRead, (RS_BYTE *)&dwIndicator, dwSizeRead);
        if(dwRet!=ERR_SUCCESS)
                return dwRet;
        RS_SWAPDWORDEX(dwIndicator);
        adwTmpPath[1] = dwIndicator;
        adwTmpPath[2] = 3;		//这里是产品配置

        adwTmpPath[3] = dwItemID;
        dwSizeRead = sizeof(RS_DWORD);
        RS_DWORD dwDataType = 0;
        dwRet = SuperGetValue(adwTmpPath, dwDataType, (RS_BYTE *)&dwValue, dwSizeRead);
        RS_SWAPDWORDEX(dwValue);
        return dwRet;
}

RS_RESULT CConfig::WriteProductItemDWORD(RS_DWORD dwItemID, RS_DWORD dwValue)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwRet = ERR_SUCCESS;
        RS_DWORD adwTmpPath[10] = {0};
        adwTmpPath[0] = 0xFFFFFFFF;
        adwTmpPath[1] = 1;
        RS_DWORD dwDataTypeRead =0, dwIndicator = 0, dwSizeRead = sizeof(RS_DWORD);
        dwRet = SuperGetValue(adwTmpPath, dwDataTypeRead, (RS_BYTE *)&dwIndicator, dwSizeRead);
        if(dwRet!=ERR_SUCCESS)
                return dwRet;
        RS_SWAPDWORDEX(dwIndicator);
        adwTmpPath[1] = dwIndicator;
        adwTmpPath[2] = 3;		//这里是产品配置

        adwTmpPath[3] = dwItemID;
        RS_DWORD dwSizeWrite = sizeof(RS_DWORD);
        RS_SWAPDWORDEX(dwValue);
        dwRet = SuperSetValue(adwTmpPath, CONFIG_DATA_DWORD, (RS_BYTE *)&dwValue, dwSizeWrite);

        return dwRet;
}
RS_RESULT CConfig::OPExpireFlag(RS_DWORD *pdwFlag, RS_BOOL bIsWrite)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;
        if(bIsWrite)
                return ERR_FAILED;

        *pdwFlag = 1;	//过期

        RS_DWORD dwRet = ERR_SUCCESS;

        //取配置中当前的序列号
        RS_CHAR szSnCFG[50] = {0};
        RS_DWORD dwSize = 50;
        dwRet = ReadProductItemSTRING(CONFIG_ITEMNO_CURRENT_SN, szSnCFG, dwSize);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        //获得产品ID
        RS_LONG dwPrtID = 0;
        GetProductID(szSnCFG, dwPrtID);

        //取配置中的过期标志0：不过期，非0：过期
        RS_DWORD dwFlagCFG = 1;
        dwRet = ReadProductItemDWORD(CONFIG_ITEMNO_FLAG_EXPIRED, dwFlagCFG);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        CCheckValid chkValid;

        //通过产品ID获得GUID并设置GUID
        if(!chkValid.SetGUID(dwPrtID))
                return ERR_CFG_GUID_SETFAILED;

        //解决安装多个瑞星软件时，主号更换的问题
// Deleted by LauranceZhang 2004-12-29 10:54:05 <
/*
        //取注册表中的主序列号
        RS_CHAR szSnREG[50] = {0};
        if(!chkValid.GetKeySN(szSnREG))
                return ERR_CFG_REGISTRY_READFAILED;
*/
// Deleted by LauranceZhang 2004-12-29 10:54:05 >

        //获得当前版本的状态
        RS_DWORD dwFlagREG = 1;
        RS_LONG lDemoCount = 0;
        RS_LONG lSnStatus = chkValid.GetDemoStatus(szSnCFG,lDemoCount);
        //表示已经过期
        if((lSnStatus > DEMOSTATUS_ERR) && (lSnStatus < DEMOSTATUS_DIE))
        {
                dwFlagREG = 0;
        }
        else if(lSnStatus == DEMOSTATUS_DIE)
        {
                *pdwFlag = dwFlagREG = DEMOSTATUS_DIE;
                RS_CHAR szTime[64] = {0};
                //获得安装时间
                if(chkValid.GetInstallTime(szSnCFG, szTime, sizeof(szTime)) < 0 )
                        return ERR_FAILED;
                RS_LONG lInstallTime = atol(szTime);
                if(lInstallTime > 0)
                {
                        //重新设置截止时间
                        sprintf(szTime, "%ld", lInstallTime);
                        if(!chkValid.SetEndTime(szSnCFG, szTime))
                                return ERR_FAILED;
                }
                else//用户一定修改了注册表
                {
                        if(chkValid.GetEndTime(szSnCFG, szTime, sizeof(szTime)) < 0 )
                                return ERR_FAILED;
                        //重新设置安装时间
                        if(!chkValid.SetInstallTime(szSnCFG, szTime))
                                return ERR_FAILED;
                }
        }

        *pdwFlag = dwFlagCFG;
        if(dwFlagREG != dwFlagCFG)
        {
                //不相同时，按照注册表
                *pdwFlag = dwFlagREG;
                dwRet = WriteProductItemDWORD(CONFIG_ITEMNO_FLAG_EXPIRED, dwFlagREG);
                if(dwRet != ERR_SUCCESS)
                        return dwRet;
                Synchronize();
        }
        g_dwExpiredFlag = *pdwFlag;
        return dwRet;
}

RS_RESULT CConfig::OPLeftTime(RS_DWORD *pdwValue, RS_BOOL bIsWrite)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        if(bIsWrite)
                return ERR_FAILED;

        RS_DWORD dwRet = ERR_SUCCESS;

        RS_CHAR szCurrentSn[100] = {0};
        RS_DWORD dwSnSize = 100;
        dwRet = ReadProductItemSTRING(CONFIG_ITEMNO_CURRENT_SN, szCurrentSn, dwSnSize);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        RS_LONG dwPrtID = 0;
        GetProductID(szCurrentSn, dwPrtID);		//获得产品ID

        CCheckValid chkValid;
        if(!chkValid.SetGUID(dwPrtID))			//通过产品ID获得GUID并设置GUID
                return ERR_CFG_GUID_SETFAILED;

        CHAR szTime[64] = {0};
        RS_LONG lEndTime = 0;
        if(0 != chkValid.GetEndTime(szCurrentSn, szTime, sizeof(szTime)))
                return ERR_FAILED;
        else
                lEndTime = atol(szTime);

        time_t t;
        time(&t);			//当前时间
        if(lEndTime < 0)
                return ERR_FAILED;
        *pdwValue = (RS_DWORD)((RS_DWORD)lEndTime - t);
        return dwRet;
}

RS_RESULT CConfig::OPExpireTime(RS_DWORD *pdwValue, RS_BOOL bIsWrite)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        if(bIsWrite)
                return ERR_FAILED;

        RS_DWORD dwRet = ERR_SUCCESS;

        RS_CHAR szCurrentSn[100] = {0};
        RS_DWORD dwSnSize = 100;
        dwRet = ReadProductItemSTRING(CONFIG_ITEMNO_CURRENT_SN, szCurrentSn, dwSnSize);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        RS_DWORD dwEndTimeCFG = 0;
        dwRet = ReadProductItemDWORD(CONFIG_ITEMNO_TIME_EXPIRED, dwEndTimeCFG);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        RS_LONG dwPrtID = 0;
        //获得产品ID
        GetProductID(szCurrentSn, dwPrtID);

        CCheckValid chkValid;
        //通过产品ID获得GUID并设置GUID
        if(!chkValid.SetGUID(dwPrtID))
                return ERR_CFG_GUID_SETFAILED;

        CHAR szTime[64] = {0};
        RS_DWORD dwEndTime = 0;
        if(0 != chkValid.GetEndTime(szCurrentSn, szTime, sizeof(szTime)))
                *pdwValue = 0;
        else
                *pdwValue = (RS_DWORD)atol(szTime);

        if(dwEndTimeCFG != *pdwValue)
        {
                WriteProductItemDWORD(CONFIG_ITEMNO_TIME_EXPIRED, *pdwValue);
                Synchronize();
        }
        return dwRet;
}
/*
RS_RESULT CConfig::OPAddTime(RS_DWORD dwValue, RS_BOOL bIsWrite)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        if(!bIsWrite)
                return ERR_FAILED;

        RS_DWORD dwRet = ERR_SUCCESS;

        RS_CHAR szCurrentSn[100] = {0};
        RS_DWORD dwSnSize = 100;
        dwRet = ReadProductItemSTRING(CONFIG_ITEMNO_CURRENT_SN, szCurrentSn, dwSnSize);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        RS_LONG dwPrtID = 0;
        //获得产品ID
        GetProductID(szCurrentSn, dwPrtID);

        CCheckValid chkValid;
        //通过产品ID获得GUID并设置GUID
        if(!chkValid.SetGUID(dwPrtID))
                return ERR_CFG_GUID_SETFAILED;

        RS_LONG lTimeSeg = (RS_LONG)dwValue;
        RS_LONG lEndTime = 0;

        CHAR szTime[64] = {0};
        if(0 != chkValid.GetEndTime(szCurrentSn, szTime, sizeof(szTime)))
                lEndTime = lTimeSeg>0 ? lTimeSeg:-lTimeSeg;
        else
                lEndTime = atol(szTime) + lTimeSeg;

        time_t t;
        RS_LONG lCurTime = time(&t);

        //如果当前时间仍然大于截止时间，则重新设置安装时间和截止时间
        if(lCurTime > lEndTime)
        {
                sprintf(szTime, "%ld", lCurTime);
                if(!chkValid.SetInstallTime(szCurrentSn,szTime))
                        return ERR_FAILED;
                lEndTime = lCurTime + lTimeSeg;
                sprintf(szTime, "%ld", lEndTime);
                if(!chkValid.SetEndTime(szCurrentSn, szTime))
                        return ERR_FAILED;
        }
        else
        {
                sprintf(szTime, "%ld", lEndTime);
                if(!chkValid.SetEndTime(szCurrentSn, szTime))
                        return ERR_FAILED;
        }
        //写截止时间
        dwRet = WriteProductItemDWORD(CONFIG_ITEMNO_TIME_EXPIRED, (RS_DWORD)lEndTime);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        Synchronize();
        return dwRet;
}
*/
RS_RESULT CConfig::OPGetSNTime(RS_BYTE *pbyData, RS_BOOL bIsWrite)
{
        if(!bIsWrite)
                return ERR_FAILED;

        RS_DWORD dwSize = 27;
        CCheckSn pCheckSn;
        RS_CHAR *pszData = (RS_CHAR *)pbyData;
        //验证序列号的合法性
        if( ERR_SN_VAL_SUCCESS != pCheckSn.CheckSn(pszData, dwSize, 0))
                return ERR_SN_CHECK_FAILED;

        CCheckValid chkValid;
        //通过产品ID获得GUID并设置GUID
        if(!chkValid.SetGUID(pCheckSn.GetProductID()))
                return RS_TRUE;

        //获得当前版本的状态
        RS_LONG lDemoCount = 0;
        RS_LONG lSnStatus = chkValid.GetDemoStatus(pszData, lDemoCount);
        RS_CHAR szTime[30] = {0};
        RS_LONG lInstallTime = 0, lEndTime = 0;
        if(-1 == chkValid.GetInstallTime(pszData, szTime, 30))
                return ERR_FAILED;
        lInstallTime = atol(szTime);
        if(-1 == chkValid.GetEndTime(pszData, szTime, 30))
                return ERR_FAILED;

        lEndTime = atol(szTime);
        *((RS_DWORD *)pbyData) = lSnStatus;
        *((RS_DWORD *)(pbyData + 4)) = lInstallTime;
        *((RS_DWORD *)(pbyData + 8)) = lEndTime;

        return ERR_SUCCESS;
}
RS_RESULT CConfig::OPAddTime(RS_DWORD dwValue, RS_BOOL bIsWrite)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        if(!bIsWrite)
                return ERR_FAILED;

        RS_DWORD dwRet = ERR_SUCCESS;

        RS_CHAR szCurrentSn[100] = {0};			//获得产品SN
        RS_DWORD dwSnSize = 100;
        dwRet = ReadProductItemSTRING(CONFIG_ITEMNO_CURRENT_SN, szCurrentSn, dwSnSize);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        RS_LONG dwPrtID = 0;
        GetProductID(szCurrentSn, dwPrtID);		//获得产品ID

        CCheckValid chkValid;
        if(!chkValid.SetGUID(dwPrtID))			//通过产品ID获得GUID并设置GUID
                return ERR_CFG_GUID_SETFAILED;

        RS_LONG lTimeSeg = (RS_LONG)dwValue;	//时间段
        RS_LONG lEndTime = 0;					//原截止时间
        RS_LONG lInstallTime = 0;				//原安装时间
        RS_LONG lNewInstallTime = 0;			//新安装时间
        RS_LONG lNewEndTime = 0;				//新截止时间
        CHAR szTime[64] = {0};
        time_t t;
        RS_LONG lCurTime = time(&t);			//当前时间

        //获得安装时间
        if(chkValid.GetInstallTime(szCurrentSn, szTime, sizeof(szTime)) < 0 )
// Modified by LauranceZhang 2004-12-29 14:33:50 <
/*
                return ERR_FAILED;
        lInstallTime = atol(szTime);
*/
                lInstallTime = lCurTime;
        else
                lInstallTime = atol(szTime);
// Modified by LauranceZhang 2004-12-29 14:33:50 >
        if(lInstallTime < 0)					//用户一定修改了注册表
                return ERR_FAILED;

        //获得截止时间，但截止时间有可能没得到
        if(chkValid.GetEndTime(szCurrentSn, szTime, sizeof(szTime)) >=0 )
                lEndTime = atol(szTime);

        if(lInstallTime < lCurTime)		//正常情况
        {
                if(lTimeSeg > 0)			//更新截止时间
                {
                        lNewInstallTime = (lEndTime > lCurTime)? 0 : lCurTime;
                        lNewEndTime = lCurTime + lTimeSeg;
                }
                else						//需要过期
                {
                        lNewEndTime = (lEndTime > lCurTime) ? lCurTime:lEndTime;
                        lNewEndTime = (lNewEndTime == 0) ? lCurTime : lNewEndTime;
                }
        }
        else							//用户一定修改了系统时间，使时间提前了，这时也允许用户冲值
        {
                if(lTimeSeg > 0)			//如果用户已经冲值
                {
                        lNewInstallTime = lCurTime;
                        lNewEndTime = lCurTime + lTimeSeg;
                }
                else						//需要过期
                {
                        lNewEndTime = (lEndTime > lCurTime) ? lCurTime:lEndTime;
                        lNewEndTime = (lNewEndTime == 0) ? lCurTime : lNewEndTime;
                }
        }

        if(lNewEndTime < lInstallTime && lNewInstallTime==0)
                lNewInstallTime = lNewEndTime;


        if(lNewEndTime < 0)				//非正常情况
                return ERR_CFG_PARAM_INVALID;

        if(lNewInstallTime > 0)			//重新设置安装时间
        {
                sprintf(szTime, "%ld", lNewInstallTime);
                if(!chkValid.SetInstallTime(szCurrentSn,szTime))
                        return ERR_FAILED;
        }

        //重新设置截止时间
        sprintf(szTime, "%ld", lNewEndTime);
        if(!chkValid.SetEndTime(szCurrentSn, szTime))
                return ERR_FAILED;

        //写配置截止时间
        dwRet = WriteProductItemDWORD(CONFIG_ITEMNO_TIME_EXPIRED, (RS_DWORD)lNewEndTime);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        Synchronize();

        //重新取过期标志 2004.12.14
        OPExpireFlag(&g_dwExpiredFlag, RS_FALSE);
        return dwRet;
}
RS_RESULT CConfig::OPInstallFlag(RS_DWORD &dwInstallFlag, RS_BOOL bIsWrite)
{
#ifdef UNIXPLATFORM
// TODO
        dwInstallFlag = 1;
        return ERR_SUCCESS;
#else

#ifdef FOR_INNER_INSTALL
        dwInstallFlag = 1;
        return ERR_SUCCESS;
#endif
        dwInstallFlag = 0;
        if(bIsWrite)
                return ERR_FAILED;

        RS_CHAR szCurrentSn[100] = {0};
        RS_DWORD dwSnSize = 100;
        RS_DWORD dwRet = ReadProductItemSTRING(CONFIG_ITEMNO_CURRENT_SN, szCurrentSn, dwSnSize);
        if(dwRet != ERR_SUCCESS)
                return dwRet;
        if(strlen(szCurrentSn)!=27)
                return ERR_FAILED;

        RS_CHAR szSubKey[100] = {0};
        RS_DWORD dwSubLen = 100;
        dwRet = ReadProductItemSTRING(CONFIG_ITEMNO_PRODUCT_KEY, szSubKey, dwSubLen);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        //判断注册表中是否已经有安装标志
        RS_CHAR szKey[100] = {"SOFTWARE\\RISING\\"};
        strncat(szKey, szSubKey, 100);
        RS_CHAR szBuffer[1024] = {0};
        RS_INT nDataLen = 1024;
        RS_INT nRetVal = ReadREGString(HKEY_LOCAL_MACHINE, szKey, "installpath", szBuffer, nDataLen) ;
        if(ERROR_SUCCESS != nRetVal)
                return ERR_CFG_REG_QUERYFAILED;

        RS_CHAR szPath[1024] = {0};
        RS_DWORD dwPathLen = 1024;
        dwRet = OPGetInstallPath(szPath, dwPathLen);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        if(strnicmp(szBuffer, szPath, 1024) != 0)
                return ERR_FAILED;
        dwInstallFlag = 1;
        return dwRet;
#endif
}

RS_RESULT CConfig::DealwithHookExpired(RS_DWORD dwItemId, RS_BYTE* pbyData, RS_DWORD &dwSize)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        if(dwItemId==INVALID_ITEMID)
                return ERR_CFG_PARAM_INVALID;


        RS_DWORD dwRet = ERR_SUCCESS;
        //这里是得到真正的过期标志
// Deleted by LauranceZhang 2004-12-15 17:01:41 <
/*
        if(g_dwInstallFlag)
        {
                RS_DWORD dwExpiredFlag = 0;
                dwRet = OPExpireFlag(&dwExpiredFlag, RS_FALSE);
                if(ERR_SUCCESS != dwRet)
                        return dwRet;
                g_dwExpiredFlag = dwExpiredFlag;
                if(!g_dwExpiredFlag)
                        return dwRet;
        }
*/
// Deleted by LauranceZhang 2004-12-15 17:01:41 >
        HITEM hItem = RS_NULL;
        dwRet = OpenItem(0x40000000 | dwItemId, hItem, m_dwAccess & ~CONFIG_ACCESS_CREATE);
        if(ERR_SUCCESS != dwRet)
        {
                //如果失败则说明没有相应的过期项
                dwRet = OpenItem(dwItemId, hItem, m_dwAccess & ~CONFIG_ACCESS_CREATE);
                if(ERR_SUCCESS != dwRet)
                        return dwRet;
        }


        dwRet = HaveRights(CONFIG_ACCESS_READ);
        if(ERR_SUCCESS != dwRet)
                return dwRet;

        RS_DWORD dwDataType = CONFIG_DATA_UNKOWN;
        dwRet = m_tree->GetItemValue(hItem, dwDataType, pbyData, dwSize);
        if(ERR_SUCCESS != dwRet)
        {
                if(dwItemId!=INVALID_ITEMID)
                        CloseItem(&hItem);
                return dwRet;
        }
        if(dwItemId!=INVALID_ITEMID)
                dwRet = CloseItem(&hItem);
        return dwRet;
}

//写数据的入口
RS_RESULT CConfig::SetItemValueEx(RS_DWORD dwItemId, RS_DWORD dwDataType,  RS_BYTE* pbyData, RS_DWORD& dwSize, RS_BOOL bHookCheck)
{
        CMemMutex memMutex(&g_cMutexMem);
        MyTrace("SetItemValueEx [--->] Path = %x->%x->%x->%x", m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3]);
        if(!m_bTreeLoaded)
        {
                FormatErrorMessage(ERR_CFG_TREE_LOADFAILED, __FILE__, __LINE__, "SetItemValueEx");
                return ERR_CFG_TREE_LOADFAILED;
        }

        RS_DWORD dwRet = ERR_SUCCESS;
        dwRet = HaveRights(CONFIG_ACCESS_WRITE, RS_TRUE);
        if(ERR_SUCCESS != dwRet)
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "SetItemValueEx");
                return dwRet;
        }
        //**************************************************//
        //序列号的验证
        if(bHookCheck)
        {
                dwRet = HookCheck(dwItemId, dwDataType, pbyData, dwSize, RS_TRUE);
                if(ERR_SUCCESS != dwRet)
                {
                        FormatErrorMessage(dwRet, __FILE__, __LINE__, "SetItemValueEx");
                        return dwRet;
                }
        }
        //**************************************************//

        HITEM hItem = RS_NULL;
        dwRet = OpenItem(dwItemId, hItem, m_dwAccess & ~CONFIG_ACCESS_CREATE);
        if(ERR_SUCCESS != dwRet)
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "SetItemValueEx");
                return dwRet;
        }
        //写入配置项值
        dwRet = m_tree->SetItemValue(hItem, dwDataType, pbyData, dwSize);
        if(ERR_SUCCESS != dwRet)
        {
                if(dwItemId!=INVALID_ITEMID)
                        CloseItem(&hItem);
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "SetItemValueEx");
                return dwRet;
        }
        if(dwItemId!=INVALID_ITEMID)
        {
                dwRet = CloseItem(&hItem);
                if(ERR_SUCCESS != dwRet)
                {
                        FormatErrorMessage(dwRet, __FILE__, __LINE__, "SetItemValueEx");
                        return dwRet;
                }
        }

        MyTrace("SetItemValueEx [Ret=%x] Path = %x->%x->%x->%x", dwRet, m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3]);
        return dwRet;
}

//写相关安装信息，安装时间
RS_RESULT CConfig::WriteInstallInfo(RS_LONG lProductID, RS_LONG lTranslatedPrtID, RS_LONG lEndTime)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwRet = ERR_SUCCESS;
        RS_DWORD adwTmpPath[10] = {0};
        adwTmpPath[0] = 0xFFFFFFFF;
        adwTmpPath[1] = 1;
        RS_DWORD dwDataTypeRead =0, dwIndicator = 0, dwSizeRead = sizeof(RS_DWORD);
        dwRet = SuperGetValue(adwTmpPath, dwDataTypeRead, (RS_BYTE *)&dwIndicator, dwSizeRead);
        if(dwRet!=ERR_SUCCESS)
                return dwRet;
        RS_SWAPDWORDEX(dwIndicator);
        adwTmpPath[1] = dwIndicator;
        adwTmpPath[2] = 3;		//这里是产品配置

        //取得产品ID
        adwTmpPath[3] = CONFIG_ITEMNO_PRODUCT_ID;	//过期标志
        RS_LONG lPrdID = 0;
        dwSizeRead = sizeof(RS_DWORD);
// Modified by LauranceZhang 2004-11-12 14:54:03 <
/*
        dwRet = SuperGetValue(adwTmpPath, dwDataTypeRead, (RS_BYTE *)&lPrdID, dwSizeRead);
        if(dwRet!=ERR_SUCCESS)
                return dwRet;
*/
        dwRet = SuperGetValue(adwTmpPath, dwDataTypeRead, (RS_BYTE *)&lPrdID, dwSizeRead);
        if(dwRet==ERR_SUCCESS)
                RS_SWAPDWORDEX(lPrdID);
// Modified by LauranceZhang 2004-11-12 14:54:03 >
        //写产品ID
        dwSizeRead = sizeof(RS_LONG);
        RS_SWAPDWORDEX(lProductID);
        dwRet = SuperSetValue(adwTmpPath, CONFIG_DATA_DWORD, (RS_BYTE *)&lProductID, dwSizeRead);
        if(dwRet!=ERR_SUCCESS)
                return dwRet;

        //写升级后的产品ID
        adwTmpPath[3] = CONFIG_ITEMNO_PRODUCT_ID_UP;
        RS_DWORD dwSizeWrite = sizeof(RS_DWORD);
        RS_SWAPDWORDEX(lTranslatedPrtID);
        dwRet = SuperSetValue(adwTmpPath, CONFIG_DATA_DWORD, (RS_BYTE *)&lTranslatedPrtID, dwSizeWrite);
        if(dwRet!=ERR_SUCCESS)
                return dwRet;

        //写过期标志
        adwTmpPath[3] = CONFIG_ITEMNO_FLAG_EXPIRED;	//过期标志
        RS_DWORD dwFlag = 0;
        dwSizeWrite = sizeof(RS_DWORD);
        RS_SWAPDWORDEX(dwFlag);
        dwRet = SuperSetValue(adwTmpPath, CONFIG_DATA_DWORD, (RS_BYTE *)&dwFlag, dwSizeWrite);
        if(dwRet!=ERR_SUCCESS)
                return dwRet;

        //写安装时间
        adwTmpPath[3] = CONFIG_ITEMNO_TIME_INSTALL;	//安装时间
        time_t t;
        time(&t);
        dwSizeWrite = sizeof(time_t);
        RS_SWAPDWORDEX(t);
        dwRet = SuperSetValue(adwTmpPath, CONFIG_DATA_DWORD, (RS_BYTE *)&t, dwSizeWrite);
        if(dwRet!=ERR_SUCCESS)
                return dwRet;

        //写过期时间
        adwTmpPath[3] = CONFIG_ITEMNO_TIME_EXPIRED;	//过期时间
        RS_SWAPDWORDEX(lEndTime);
        dwRet = SuperSetValue(adwTmpPath, CONFIG_DATA_DWORD, (RS_BYTE *)&lEndTime, dwSizeWrite);

        return dwRet;
}

//写相关信息,比如安装时间,第一次的使用时间
RS_RESULT CConfig::WriteRelatedInfo()
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        //判断用户类型
        RS_DWORD adwUserInstall[RS_USERID_LEN]={0xFFFFFFFF, 0x0, 0x0, 0x1};
        RS_DWORD adwUserRav[RS_USERID_LEN]={0xFFFFFFFF, 0x0, 0x0, 0x2};
        RS_BOOL bIsInstall = RS_FALSE;
        RS_BOOL bIsRun = RS_FALSE;
        if(0 == memcmp(m_adwUser, adwUserInstall, sizeof(RS_DWORD) * RS_USERID_LEN))
                bIsInstall = RS_TRUE;
        if(0 == memcmp(m_adwUser, adwUserRav, sizeof(RS_DWORD) * RS_USERID_LEN))
                bIsRun = RS_TRUE;

        RS_DWORD dwRet = ERR_SUCCESS;
        if( bIsInstall || bIsRun )
        {
                //取系统时间
#ifdef UNIXPLATFORM
                time_t lcTime;
                struct tm *stTime={0};
                char aszTime[20] = {0};
                lcTime=time(0);
                stTime=localtime(&lcTime);
                sprintf(aszTime, "%04d-%02d-%02d %02d:%02d:%02d", stTime->tm_year+1900,
                        stTime->tm_mon+1, stTime->tm_mday, stTime->tm_hour, stTime->tm_min,
                         stTime->tm_sec);
#else
                SYSTEMTIME stTime = {0};
                GetSystemTime(&stTime);
                RS_CHAR aszTime[20] = {0};
                sprintf(aszTime, "%04d-%02d-%02d %02d:%02d:%02d", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
#endif

                IConfig *pConfig = RS_NULL;
                RS_DWORD dwAcc = m_dwAccess & ~CONFIG_ACCESS_CREATE;
                dwRet = GetSiblingNode(3, dwAcc, m_adwUser, &pConfig);
                if(ERR_SUCCESS != dwRet)
                        return dwRet;

                RS_DWORD dwItemID = bIsInstall ? 0x15 : 0x16;

                //读第一次的安装时间或使用时间
                RS_DWORD dwBufSize = 0;
                dwRet = pConfig->GetItemValue(dwItemID, RS_NULL, dwBufSize);
                if(ERR_CFG_BUFFER_OVERFLOW != dwRet)
                        return dwRet;
                dwRet = ERR_SUCCESS;		//恢复此错误

                if((dwBufSize < 0) || (dwBufSize > 19))
                        return ERR_CFG_PARAM_INVALID;

                //写第一次的安装时间或使用时间
                if(dwBufSize==0)
                {
                        dwBufSize = strlen(aszTime);

                        dwRet = ((CConfig *)pConfig)->SetItemValueEx(dwItemID, CONFIG_DATA_STRING, (RS_BYTE *)aszTime, dwBufSize);
                        if(dwRet != ERR_SUCCESS)
                                return dwRet;
                }

                pConfig->CloseNode();
                delete pConfig;
        }

        return dwRet;
}

RS_RESULT CConfig::CheckSNDetail(RS_DWORD dwItemID, RS_CHAR *pszSrc, RS_DWORD dwValue)
{
        RS_CHAR *pszComp = RS_NULL;
        if(pszSrc)
        {
                pszComp = pszSrc;
        }
        else
        {
                pszComp = new RS_CHAR[10];
                if(!pszComp)
                        return ERR_CFG_MEM_ALLOCFAILED;
                memset(pszComp, 0, sizeof(RS_CHAR)*10);
                sprintf(pszComp,"%x",dwValue);
        }

        //获得序列号的限制,然后进行对比验证
        RS_DWORD dwRet = ERR_SUCCESS;
        RS_DWORD dwORID = 0x80000000 | dwItemID;
        RS_CHAR *pszSNLimit = RS_NULL;
        RS_DWORD dwBufSize = 0;
        GetItemValue(dwORID,RS_NULL,dwBufSize);
        if(dwBufSize > 0)
        {
                pszSNLimit = new RS_CHAR[dwBufSize+1];
                memset(pszSNLimit, 0, dwBufSize+1);
                dwRet = GetItemValue(dwORID, (RS_BYTE *)pszSNLimit, dwBufSize);
                if(ERR_SUCCESS != dwRet)
                {
                        if(!pszSrc)
                                delete pszComp;
                        delete pszSNLimit;
                        return dwRet;
                }
                //判断是否真的匹配上了
                RS_CHAR *pszFind = strstr(pszSNLimit, pszComp);
                RS_INT nCompLen = strlen(pszComp);
                if(pszFind)
                {
                        if(pszFind > pszSNLimit)
                        {
                                if(	((*(pszFind-1) != ';') &&
                                        (*(pszFind-1) != '-') &&
                                        (*(pszFind-1) != '0') ) ||
                                        ((*(pszFind + nCompLen) != ';') &&
                                        (*(pszFind + nCompLen) != '-') &&
                                        (*(pszFind + nCompLen) != 0)) )
                                        pszFind = RS_NULL;

                        }
                        else
                        {
                                if(	(*(pszFind + nCompLen) != ';') &&
                                        (*(pszFind + nCompLen) != '-') &&
                                        (*(pszFind + nCompLen) != 0) )
                                {
                                        if(strcmp(pszComp, "0") != 0)
                                                pszFind = RS_NULL;
                                }
                        }
                }

                if(!pszFind)
                {
                        //没找到的情况下,要进一步看是否存在范围
                        RS_CHAR *pszFirst = pszSNLimit, *pszLast = strstr(pszSNLimit, "-");
                        if(!pszLast)
                        {
                                if(!pszSrc)
                                        delete pszComp;
                                delete pszSNLimit;
                                return ERR_SN_NOT_MATCH;
                        }
                        pszLast ++;

                        RS_CHAR szFirst[50] = {0}, szLast[50] = {0};
                        while(pszFirst && pszLast)
                        {
                                RS_CHAR *pTmp = RS_NULL;
                                strncpy(szFirst, pszFirst, 50);
                                szFirst[50] = 0;
                                pTmp = strchr(szFirst, '-');
                                if(pTmp)
                                        *pTmp = 0;
                                strncpy(szLast, pszLast, 50);
                                szLast[50] = 0;
                                pTmp = strchr(szLast, ';');
                                if(pTmp)
                                        *pTmp = 0;

                                RS_LONG lVal = 0;
                                RS_LONG lFirst = 0;
                                RS_LONG lLast = 0;

                                sscanf(pszComp, "%x", &lVal);
                                sscanf(szFirst, "%x", &lFirst);
                                sscanf(szLast, "%x", &lLast);

// Modified by LauranceZhang 2004-11-30 11:44:55 <
/*
                                if(	(stricmp(pszComp, pszFirst) >= 0) &&
                                        (stricmp(pszComp, pszLast) <= 0)	)
                                        break;
*/
                                if((lVal >= lFirst) && (lVal <= lLast))
                                        break;
// Modified by LauranceZhang 2004-11-30 11:44:55 >

                                pszFirst = strstr(pszFirst, ";");
                                if(!pszFirst)
                                {
                                        if(!pszSrc)
                                                delete pszComp;
                                        delete pszSNLimit;
                                        return ERR_SN_NOT_MATCH;
                                }
                                pszFirst ++;
                                pszLast = strstr(pszFirst, "-");
                                if(!pszLast)
                                {
                                        if(!pszSrc)
                                                delete pszComp;
                                        delete pszSNLimit;
                                        return ERR_SN_NOT_MATCH;
                                }
                                pszLast ++;
                        }
                        if(!pszFirst || !pszLast)
                        {
                                if(!pszSrc)
                                        delete pszComp;
                                delete pszSNLimit;
                                return ERR_SN_NOT_MATCH;
                        }
                }
                delete pszSNLimit;
        }

        if(!pszSrc)
                delete pszComp;
        return ERR_SUCCESS;
}

RS_RESULT CConfig::WriteSnInfo(RS_BOOL bIsInsall, CCheckSn *pCheckSn)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwRet = ERR_SUCCESS;
        RS_DWORD dwValue = 0;
        RS_DWORD dwDataType = CONFIG_DATA_BINARY;
        RS_DWORD dwSize = sizeof(dwSize);
        RS_DWORD dwOR = 0;

        dwValue = pCheckSn->GetProductBigType();
        if(dwValue == ERR_SN_VAL_INVALID)
                return ERR_SN_GETINFO_FAILED;
        dwRet = !bIsInsall ? ERR_SUCCESS : CheckSNDetail(CONFIG_ITEMNO_SN_EDITION, RS_NULL, dwValue);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        dwRet = SetItemValueDWORD(dwOR | CONFIG_ITEMNO_SN_EDITION, dwValue);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        dwValue = pCheckSn->GetProductType();
        if(dwValue == ERR_SN_VAL_INVALID)
                return ERR_SN_GETINFO_FAILED;
        dwRet = !bIsInsall ? ERR_SUCCESS : CheckSNDetail(CONFIG_ITEMNO_SN_TYPE, RS_NULL, dwValue);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        dwRet = SetItemValueDWORD(dwOR | CONFIG_ITEMNO_SN_TYPE, dwValue);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        dwValue = pCheckSn->GetProductSubtype();
        dwRet = !bIsInsall ? ERR_SUCCESS : CheckSNDetail(CONFIG_ITEMNO_SN_SUBTYPE, RS_NULL, dwValue);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        dwRet = SetItemValueDWORD(dwOR | CONFIG_ITEMNO_SN_SUBTYPE, dwValue);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        dwValue = pCheckSn->GetTrialType();
        if(dwValue == ERR_SN_VAL_INVALID)
                return ERR_SN_GETINFO_FAILED;
        dwRet = !bIsInsall ? ERR_SUCCESS : CheckSNDetail(CONFIG_ITEMNO_SN_TRIAL, RS_NULL, dwValue);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        dwRet = SetItemValueDWORD(dwOR | CONFIG_ITEMNO_SN_TRIAL, dwValue);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        dwValue = pCheckSn->GetBatchNo();
        if(dwValue == ERR_SN_VAL_INVALID)
                return ERR_SN_GETINFO_FAILED;
        dwRet = !bIsInsall ? ERR_SUCCESS : CheckSNDetail(CONFIG_ITEMNO_SN_BATCHNO, RS_NULL, dwValue);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        dwRet = SetItemValueDWORD(dwOR | CONFIG_ITEMNO_SN_BATCHNO, dwValue);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        dwValue = pCheckSn->GetFlowNo();
        if(dwValue == ERR_SN_VAL_INVALID)
                return ERR_SN_GETINFO_FAILED;
        dwRet = !bIsInsall ? ERR_SUCCESS : CheckSNDetail(CONFIG_ITEMNO_SN_FLOWNO, RS_NULL, dwValue);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        dwRet = SetItemValueDWORD(dwOR | CONFIG_ITEMNO_SN_FLOWNO, dwValue);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        dwValue = pCheckSn->GetRegionNo();
        if(dwValue == ERR_SN_VAL_INVALID)
                return ERR_SN_GETINFO_FAILED;
#if 0
        dwRet = !bIsInsall ? ERR_SUCCESS : CheckSNDetail(CONFIG_ITEMNO_SN_REGION, RS_NULL, dwValue);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
#endif
        dwRet = SetItemValueDWORD(dwOR | CONFIG_ITEMNO_SN_REGION, dwValue);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        dwValue = pCheckSn->GetOEMID();
        dwRet = !bIsInsall ? ERR_SUCCESS : CheckSNDetail(CONFIG_ITEMNO_SN_OEMID, RS_NULL, dwValue);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        dwRet = SetItemValueDWORD(dwOR | CONFIG_ITEMNO_SN_OEMID, dwValue);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        dwValue = pCheckSn->GetHardwareType();
        dwRet = !bIsInsall ? ERR_SUCCESS : CheckSNDetail(CONFIG_ITEMNO_SN_HWTYPE, RS_NULL, dwValue);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        dwRet = SetItemValueDWORD(dwOR | CONFIG_ITEMNO_SN_HWTYPE, dwValue);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        dwValue = pCheckSn->GetOSType();
        dwRet = !bIsInsall ? ERR_SUCCESS : CheckSNDetail(CONFIG_ITEMNO_SN_OSTYPE, RS_NULL, dwValue);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        dwRet = SetItemValueDWORD(dwOR | CONFIG_ITEMNO_SN_OSTYPE, dwValue);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        dwValue = pCheckSn->GetOSVersion();
        dwRet = !bIsInsall ? ERR_SUCCESS : CheckSNDetail(CONFIG_ITEMNO_SN_OSVER, RS_NULL, dwValue);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        dwRet = SetItemValueDWORD(dwOR | CONFIG_ITEMNO_SN_OSVER, dwValue);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        dwValue = pCheckSn->GetNumCenters();
        dwRet = !bIsInsall ? ERR_SUCCESS : CheckSNDetail(CONFIG_ITEMNO_SN_CENTERNO, RS_NULL, dwValue);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        dwRet = SetItemValueDWORD(dwOR | CONFIG_ITEMNO_SN_CENTERNO, dwValue);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        dwValue = pCheckSn->GetNumServers();
        dwRet = !bIsInsall ? ERR_SUCCESS : CheckSNDetail(CONFIG_ITEMNO_SN_SERVERNO, RS_NULL, dwValue);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        dwRet = SetItemValueDWORD(dwOR | CONFIG_ITEMNO_SN_SERVERNO, dwValue);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        dwValue = pCheckSn->GetNumClients();
        dwRet = !bIsInsall ? ERR_SUCCESS : CheckSNDetail(CONFIG_ITEMNO_SN_CLIENTNO, RS_NULL, dwValue);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        dwRet = SetItemValueDWORD(dwOR | CONFIG_ITEMNO_SN_CLIENTNO, dwValue);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        dwValue = pCheckSn->GetDecodeVer();
        if(dwValue == ERR_SN_VAL_INVALID)
                return ERR_SN_GETINFO_FAILED;
        dwRet = !bIsInsall ? ERR_SUCCESS : CheckSNDetail(CONFIG_ITEMNO_SN_ENCRYPT, RS_NULL, dwValue);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        dwRet = SetItemValueDWORD(dwOR | CONFIG_ITEMNO_SN_ENCRYPT, dwValue);
        if(dwRet != ERR_SUCCESS)
                return dwRet;

        return ERR_SUCCESS;
}

//判断是否过期并且把过期时间，过期类型和截止时间写入注册表中
//任何时候此函数返回RS_TRUE则不允许安装
RS_BOOL CConfig::IsSnExpired(RS_CHAR *pchSn, CCheckSn *pCheckSn)
{
        //查看是否是受限序列号，用来限制试用号的安装
        RS_DWORD dwLimitedSize = 0;
        RS_CHAR *pszLimitedSN = RS_NULL;
        RS_DWORD dwRet = ReadProductItemSTRING(CONFIG_ITEMNO_LIMITED_SN, RS_NULL, dwLimitedSize);
        if(dwRet == ERR_CFG_BUFFER_OVERFLOW && dwLimitedSize != 0)
        {
                pszLimitedSN = new RS_CHAR[dwLimitedSize + 1];
                if(!pszLimitedSN)
                        return RS_TRUE;
                dwRet = ReadProductItemSTRING(CONFIG_ITEMNO_LIMITED_SN, pszLimitedSN, dwLimitedSize);
                if(dwRet != ERR_SUCCESS)
                {
                        delete []pszLimitedSN;
                        return RS_TRUE;
                }
                pszLimitedSN[dwLimitedSize] = 0;
                if(strstr(pszLimitedSN, pchSn))
                {
                        delete []pszLimitedSN;
                        return RS_TRUE;
                }
                delete []pszLimitedSN;
                pszLimitedSN = RS_NULL;
        }
        //查看是否是需要立刻过期的序列号
        RS_BOOL bNeedExpired = RS_FALSE;
        dwLimitedSize = 0;
        dwRet = ReadProductItemSTRING(CONFIG_ITEMNO_EXPIRED_SN, RS_NULL, dwLimitedSize);
        if(dwRet == ERR_CFG_BUFFER_OVERFLOW && dwLimitedSize != 0)
        {
                pszLimitedSN = new RS_CHAR[dwLimitedSize + 1];
                if(!pszLimitedSN)
                        return RS_TRUE;
                dwRet = ReadProductItemSTRING(CONFIG_ITEMNO_EXPIRED_SN, pszLimitedSN, dwLimitedSize);
                if(dwRet != ERR_SUCCESS)
                {
                        delete []pszLimitedSN;
                        return RS_TRUE;
                }
                pszLimitedSN[dwLimitedSize] = 0;
                if(strstr(pszLimitedSN, pchSn))
                        bNeedExpired = RS_TRUE;
                delete []pszLimitedSN;
                pszLimitedSN = RS_NULL;
        }

        //通过产品ID获得GUID并设置GUID
        CCheckValid chkValid;
        if(!chkValid.SetGUID(pCheckSn->GetProductID()))
                return RS_TRUE;

        //获得限时版安装的限制次数
        RS_DWORD dwDemoLimiteCount = 0;
        ReadProductItemDWORD(CONFIG_ITEMNO_DEMO_COUNT, dwDemoLimiteCount);

        //获得当前版本的状态
        RS_LONG lDemoCount = 0;
        RS_LONG lSnStatus = chkValid.GetDemoStatus(pchSn, lDemoCount);
        if(lSnStatus == DEMOSTATUS_DIE)	//表示已经过期
        {
                //获得是否允许安装的标志
                RS_DWORD dwInstallFlag = 1;
                ReadProductItemDWORD(CONFIG_ITEMNO_FLAG_INSTALL, dwInstallFlag);

                //获得是否允许升级的标志
                RS_DWORD dwUpdateFlag = 0;
                ReadProductItemDWORD(CONFIG_ITEMNO_FLAG_UPDATE, dwUpdateFlag);

                //获得是否已经安装过
                RS_DWORD dwUpID = 0;
                ReadProductItemDWORD(CONFIG_ITEMNO_PRODUCT_ID_UP, dwUpID);

                //国际版过期后即允许安装，又允许升级，但其他版本只允许安装，不允许升级
                if(dwUpID !=0 )				//已经安装过
                {
                        if(dwUpdateFlag==0 && m_adwUser[3] != 0x4000)
                                return RS_TRUE;
                }
                else
                {
                        if(dwInstallFlag==0)	//过期后不允许安装
                                return RS_TRUE;
                }
        }

        //判断是否超过了限时版安装的限制次数
        if((dwDemoLimiteCount != 0) && (lDemoCount > (RS_LONG)dwDemoLimiteCount) )
                return RS_TRUE;

        RS_CHAR szTime[30] = {0};
        time_t t;
        sprintf(szTime, "%ld", time(&t));
        if(	(lSnStatus <= DEMOSTATUS_ERR) || (lSnStatus > DEMOSTATUS_DIE) )	//表示没有安装过
        {
                //写入安装时间
                if(!chkValid.SetInstallTime(pchSn,szTime))
                        return RS_TRUE;

                RS_LONG lTrialType = pCheckSn->GetTrialType();
                RS_CHAR szType[20] = {0};
                sprintf(szType, "%ld", lTrialType);
                //写入限时类型
                if(!chkValid.SetLimitType(pchSn, szType))
                        return RS_TRUE;

                RS_LONG lTrialSeconds = pCheckSn->GetTrialDays() * 24 * 60 * 60;


    if(!bNeedExpired)	//不需要立刻过期
                {
                        t += lTrialSeconds;
                        sprintf(szTime, "%ld", t);
                }
                //写入截止时间
                if(!chkValid.SetEndTime(pchSn, szTime))
                        return RS_TRUE;
        }
        else
        {
                if(!bNeedExpired)	//不需要立刻过期
                {
                        //获得安装时间
                        if(-1 == chkValid.GetInstallTime(pchSn, szTime, 30))
                        {
                                //写入安装时间
                                sprintf(szTime, "%ld", t);
                                if(!chkValid.SetInstallTime(pchSn, szTime))
                                        return RS_TRUE;
                        }

                        //获得截止时间
                        if(-1 == chkValid.GetEndTime(pchSn, szTime, 30))
                        {
                                RS_LONG lTrialSeconds = pCheckSn->GetTrialDays() * 24 * 60 * 60;
                                t += lTrialSeconds;

                                //写入截止时间
                                sprintf(szTime, "%ld", t);
                                if(!chkValid.SetEndTime(pchSn, szTime))
                                        return RS_TRUE;
                        }
                        else
                        {
                                //能取到截止时间的时候不作任何处理
                                t = atol(szTime);
                        }
                }
                else
                {
                        //获得安装时间
                        if(-1 == chkValid.GetInstallTime(pchSn, szTime, 30))
                        {
                                sprintf(szTime, "%ld", t);
                                //写入安装时间
                                if(!chkValid.SetInstallTime(pchSn, szTime))
                                        return RS_TRUE;
                        }
                        else
                                t = atol(szTime);
                        //写入截止时间
                        if(!chkValid.SetEndTime(pchSn, szTime))
                                return RS_TRUE;
                }
        }

        //把安装信息写入配置文件
        if(ERR_SUCCESS != WriteInstallInfo(pCheckSn->GetProductID(), pCheckSn->GetTranslatedProductID(), t))
                return RS_TRUE;
        //写入产品的主序列号(不管以前是否安装过，都要写)
        if(!chkValid.SetKeySN(pchSn))
                return RS_TRUE;

        //保存配置文件
        Synchronize();

        return RS_FALSE;
}
RS_RESULT CConfig::GetInfo(RS_DWORD dwItemId, RS_DWORD dwInfoId, RS_BYTE* pbyBuffer, RS_DWORD& dwBufSize)
{
        CMemMutex memMutex(&g_cMutexMem);
        MyTrace("GetInfo [--->] Path = %x->%x->%x->%x", m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3]);
        if(!m_bTreeLoaded)
        {
                FormatErrorMessage(ERR_CFG_TREE_LOADFAILED, __FILE__, __LINE__, "GetInfo");
                return ERR_CFG_TREE_LOADFAILED;
        }
        RS_DWORD dwRet = ERR_SUCCESS;
        dwRet = HaveRights(CONFIG_ACCESS_READ);
        if(ERR_SUCCESS != dwRet)
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "GetInfo");
                return dwRet;
        }
        HITEM hItem = RS_NULL;

        dwRet = OpenItem(dwItemId, hItem, m_dwAccess & ~CONFIG_ACCESS_CREATE);
        if(ERR_SUCCESS != dwRet)
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "GetInfo");
                return dwRet;
        }
        dwRet = m_tree->GetInfo(hItem, dwInfoId, pbyBuffer, dwBufSize);
        if(ERR_SUCCESS != dwRet)
        {
                if(dwItemId != 0)
                        CloseItem(&hItem);
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "GetInfo");
                return dwRet;
        }

        if(dwItemId != 0)
                dwRet = CloseItem(&hItem);
        MyTrace("GetInfo [Ret=%x] Path = %x->%x->%x->%x", dwRet, m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3]);
        return dwRet;
}

RS_RESULT CConfig::SetInfo(RS_DWORD dwItemId, RS_DWORD dwInfoId, const RS_BYTE* pbyBuffer, RS_DWORD dwBufSize)
{
        CMemMutex memMutex(&g_cMutexMem);
        MyTrace("SetInfo [--->] Path = %x->%x->%x->%x", m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3]);
        if(!m_bTreeLoaded)
        {
                FormatErrorMessage(ERR_CFG_TREE_LOADFAILED, __FILE__, __LINE__, "SetInfo");
                return ERR_CFG_TREE_LOADFAILED;
        }
        RS_DWORD dwRet = ERR_SUCCESS;
        dwRet = HaveRights(CONFIG_ACCESS_WRITE, RS_TRUE);
        if(ERR_SUCCESS != dwRet)
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "SetInfo");
                return dwRet;
        }
        HITEM hItem = RS_NULL;
        dwRet = OpenItem(dwItemId, hItem, m_dwAccess & ~CONFIG_ACCESS_CREATE);
        if(ERR_SUCCESS != dwRet)
        {
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "SetInfo");
                return dwRet;
        }
        dwRet = m_tree->SetInfo(hItem, dwInfoId, pbyBuffer, dwBufSize);
        if(ERR_SUCCESS != dwRet)
        {
                if(dwItemId != 0)
                        CloseItem(&hItem);
                FormatErrorMessage(dwRet, __FILE__, __LINE__, "SetInfo");
                return dwRet;
        }

        if(dwItemId != 0)
                dwRet = CloseItem(&hItem);
        MyTrace("SetInfo [Ret=%x] Path = %x->%x->%x->%x", dwRet, m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3]);
        return dwRet;
}

//导入
RS_RESULT CConfig::Import(RS_CHAR *pszFileName)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwRet = ERR_SUCCESS;

        dwRet = HaveRights(CONFIG_ACCESS_WRITE & CONFIG_ACCESS_IMPORT, RS_TRUE);
        if(ERR_SUCCESS != dwRet)
                return dwRet;

        if(RS_FALSE == m_tree->DisposeFile(pszFileName,pszFileName,RS_FALSE,RS_FALSE))
                return ERR_CFG_FILE_DISPOSEFAILED;

        //装入文件
        CConfig *pConfig = new CConfig;
        if(pConfig == RS_NULL)
                return ERR_CFG_MEM_ALLOCFAILED;

        dwRet = pConfig->LoadConfigEx(pszFileName, RS_NULL, LOAD_MODE_ADD);
        if(dwRet != ERR_SUCCESS)
        {
                delete pConfig;
                pConfig = RS_NULL;
                return dwRet;
        }
        RS_DWORD dwAccessOpen = CONFIG_ACCESS_ALL;
        RS_DWORD adwPath[1] = {0};
        dwRet = pConfig->Create(adwPath, dwAccessOpen, m_adwUser);
        if(ERR_SUCCESS != dwRet)
        {
                dwAccessOpen &= ~CONFIG_ACCESS_WRITE;
                dwRet = pConfig->Create(adwPath, dwAccessOpen, m_adwUser);
                if(ERR_SUCCESS != dwRet)
                {
                        dwAccessOpen &= ~CONFIG_ACCESS_READ;
                        dwRet = pConfig->Create(adwPath, dwAccessOpen, m_adwUser);
                        if(ERR_SUCCESS != dwRet)
                                return dwRet;
                }
        }

        dwRet = m_tree->Import(m_hItem,pConfig->m_hItem);

        delete pConfig;
        return dwRet;
}
//导入到根
RS_RESULT CConfig::ImportToRoot(RS_CHAR *pszFileName)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwRet = ERR_SUCCESS;

        dwRet = HaveRights(CONFIG_ACCESS_WRITE & CONFIG_ACCESS_IMPORT, RS_TRUE);
        if(	(dwRet==ERR_CFG_NODE_ISNOTOPENED) || (dwRet==ERR_CFG_NODE_LOCKED) )
                dwRet = ERR_SUCCESS;
        if(ERR_SUCCESS != dwRet)
                return dwRet;

        if(RS_FALSE == m_tree->DisposeFile(pszFileName,pszFileName,RS_FALSE,RS_FALSE))
                return ERR_CFG_FILE_DISPOSEFAILED;

        //装入文件
        CConfig *pConfig = new CConfig;
        if(pConfig == RS_NULL)
                return ERR_CFG_MEM_ALLOCFAILED;

        dwRet = pConfig->LoadConfigEx(pszFileName, RS_NULL, LOAD_MODE_ADD);
        if(dwRet != ERR_SUCCESS)
        {
                delete pConfig;
                pConfig = RS_NULL;
                return dwRet;
        }

        RS_DWORD dwAccessOpen = CONFIG_ACCESS_ALL;
        RS_DWORD adwPath[1] = {0};
        dwRet = pConfig->Create(adwPath, dwAccessOpen, m_adwUser);
        if(ERR_SUCCESS != dwRet)
        {
                dwAccessOpen &= ~CONFIG_ACCESS_WRITE;
                dwRet = pConfig->Create(adwPath, dwAccessOpen, m_adwUser);
                if(ERR_SUCCESS != dwRet)
                {
                        dwAccessOpen &= ~CONFIG_ACCESS_READ;
                        dwRet = pConfig->Create(adwPath, dwAccessOpen, m_adwUser);
                        if(ERR_SUCCESS != dwRet)
                                return dwRet;
                }
        }

        dwRet = m_tree->Import(RS_NULL,pConfig->m_hItem);

        delete pConfig;
        return dwRet;
}
//导出
RS_RESULT CConfig::Export(RS_CHAR *pszFileName)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwRet = ERR_SUCCESS;
        dwRet = HaveRights(CONFIG_ACCESS_EXPORT);
        if(ERR_SUCCESS != dwRet)
                return dwRet;

        dwRet = m_tree->Export(m_hItem,pszFileName);
        return dwRet;
}
RS_RESULT CConfig::ExportAll(RS_CHAR *pszFileName)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwRet = ERR_SUCCESS;
        dwRet = HaveRights(CONFIG_ACCESS_EXPORT);
        if(ERR_SUCCESS != dwRet)
                return dwRet;

        dwRet = m_tree->ExportAll(m_hItem,pszFileName);
        return dwRet;
}

//加锁
RS_RESULT CConfig::Lock(RS_DWORD dwId)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwRet = ERR_SUCCESS;
        dwRet = HaveRights(CONFIG_ACCESS_LOCK);
        if(ERR_SUCCESS != dwRet)
                return dwRet;

        HITEM hItem = RS_NULL;
        dwRet = OpenItem(dwId, hItem, m_dwAccess & ~CONFIG_ACCESS_CREATE);
        if(ERR_SUCCESS != dwRet)
                return dwRet;

        RS_DWORD dwLock = 0;
        RS_DWORD dwSize = sizeof(RS_DWORD);
        dwRet = GetInfo(dwId, CONFIG_NODE_STATUS, (RS_BYTE*)&dwLock, dwSize);
        if(ERR_SUCCESS != dwRet)
        {
                if(dwId != 0)
                        CloseItem(&hItem);
                return dwRet;
        }

        dwLock |= CONFIG_STATE_LOCKED;
        dwRet = SetInfo(dwId, CONFIG_NODE_STATUS, (RS_BYTE*)&dwLock, sizeof(RS_DWORD));
        if(ERR_SUCCESS != dwRet)
        {
                if(dwId != 0)
                        CloseItem(&hItem);
                return dwRet;
        }

        if(dwId != 0)
                dwRet = CloseItem(&hItem);
        return dwRet;
}

//解锁
RS_RESULT CConfig::Unlock(RS_DWORD dwId)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwRet = ERR_SUCCESS;
        dwRet = HaveRights(CONFIG_ACCESS_UNLOCK);
        if(ERR_SUCCESS != dwRet)
                return dwRet;

        HITEM hItem = RS_NULL;
        dwRet = OpenItem(dwId, hItem, m_dwAccess & ~CONFIG_ACCESS_CREATE);
        if(ERR_SUCCESS != dwRet)
                return dwRet;

        RS_DWORD dwLock = 0;
        RS_DWORD dwSize = sizeof(RS_DWORD);
        dwRet = GetInfo(dwId, CONFIG_NODE_STATUS, (RS_BYTE*)&dwLock, dwSize);
        if(ERR_SUCCESS != dwRet)
        {
                if(dwId != 0)
                        CloseItem(&hItem);
                return dwRet;
        }

        dwLock &= ~CONFIG_STATE_LOCKED;
        dwRet = m_tree->SetInfo(hItem, CONFIG_NODE_STATUS, (RS_BYTE*)&dwLock, sizeof(RS_DWORD));
        if(ERR_SUCCESS != dwRet)
        {
                if(dwId != 0)
                        CloseItem(&hItem);
                return dwRet;
        }
        if(dwId != 0)
                dwRet = CloseItem(&hItem);
        return dwRet;
}

RS_RESULT CConfig::GetItemValueBYTE(RS_DWORD dwItemId, RS_BYTE& byData)
{
        RS_DWORD dwLen = sizeof(RS_BYTE);
        return GetItemValue(dwItemId, &byData, dwLen);
}
RS_RESULT CConfig::GetItemValueWORD(RS_DWORD dwItemId, RS_WORD& wData)
{
        RS_DWORD dwLen = sizeof(RS_WORD);
        RS_RESULT dwRet = GetItemValue(dwItemId, (RS_BYTE*)&wData, dwLen);
        if(dwRet==ERR_SUCCESS)
                RS_SWAPWORDEX(wData);
        return dwRet;
}
RS_RESULT CConfig::GetItemValueDWORD(RS_DWORD dwItemId, RS_DWORD& dwData)
{
        RS_DWORD dwLen = sizeof(RS_DWORD);
        RS_RESULT dwRet = GetItemValue(dwItemId, (RS_BYTE*)&dwData, dwLen);
        if(dwRet==ERR_SUCCESS)
                RS_SWAPDWORDEX(dwData);
        return dwRet;
}
RS_RESULT CConfig::GetItemValueString(RS_DWORD dwItemId, RS_CHAR* pbyData, RS_DWORD& dwSize)
{
        return GetItemValue(dwItemId, (RS_BYTE*)pbyData, dwSize);
}

RS_RESULT CConfig::SetItemValue(RS_DWORD dwItemId, RS_BYTE* pbyData, RS_DWORD& dwSize)
{
        return SetItemValueEx(dwItemId, CONFIG_DATA_BINARY, pbyData, dwSize, RS_TRUE);
}
RS_RESULT CConfig::SetItemValueBYTE(RS_DWORD dwItemId, RS_BYTE byData)
{
        RS_DWORD dwLen = sizeof(RS_BYTE);
        return SetItemValueEx(dwItemId, CONFIG_DATA_BYTE, &byData, dwLen, RS_TRUE);
}
RS_RESULT CConfig::SetItemValueWORD(RS_DWORD dwItemId, RS_WORD wData)
{
        RS_DWORD dwLen = sizeof(RS_WORD);
        RS_SWAPWORDEX(wData);
        return SetItemValueEx(dwItemId, CONFIG_DATA_WORD, (RS_BYTE*)&wData, dwLen, RS_TRUE);
}
RS_RESULT CConfig::SetItemValueDWORD(RS_DWORD dwItemId, RS_DWORD dwData)
{
        RS_DWORD dwLen = sizeof(RS_DWORD);
        RS_SWAPDWORDEX(dwData);
        return SetItemValueEx(dwItemId, CONFIG_DATA_DWORD, (RS_BYTE*)&dwData, dwLen, RS_TRUE);
}
RS_RESULT CConfig::SetItemValueString(RS_DWORD dwItemId, RS_CHAR* pbyData, RS_DWORD& dwSize)
{
        return SetItemValueEx(dwItemId, CONFIG_DATA_STRING, (RS_BYTE*)pbyData, dwSize, RS_TRUE);
}

// Deleted by LauranceZhang 2004-3-10 16:24:02 <
/*
RS_RESULT CConfig::GetItemId(RS_DWORD &dwId)
{
        RS_DWORD dwLevel=0, dwStatus=0;
        return m_tree.GetItemInfo(m_hItem, dwId, dwLevel, dwStatus, RS_NULL, RS_NULL);
}
RS_RESULT CConfig::SetItemId(RS_DWORD dwId)
{
        RS_DWORD dwRet = 0, dwLevel=0, dwStatus=0, dwIdTemp;
        if((dwRet = IGetInfo(dwIdTemp,dwLevel,dwStatus,RS_NULL,RS_NULL)) != ERR_SUCCESS)
                return dwRet;
        return m_tree.SetItemInfo(m_hItem, dwId, dwLevel, dwStatus, RS_NULL, RS_NULL);
}
RS_RESULT CConfig::GetNodeLevel(RS_DWORD &dwLevel)
{
        RS_DWORD dwId=0, dwStatus=0;
        return m_tree.GetItemInfo(m_hItem, dwId, dwLevel, dwStatus, RS_NULL, RS_NULL);
}
RS_RESULT CConfig::SetNodeLevel(RS_DWORD dwLevel)
{
        RS_DWORD dwRet = 0, dwLevelTmp=0, dwStatus=0, dwId=0;
        if((dwRet = IGetInfo(dwId,dwLevelTmp,dwStatus,RS_NULL,RS_NULL)) != ERR_SUCCESS)
                return dwRet;
        return m_tree.SetItemInfo(m_hItem, dwId, dwLevel, dwStatus, RS_NULL, RS_NULL);
}
RS_RESULT CConfig::SetNodeStatus(RS_DWORD dwStatus)
{
        RS_DWORD dwRet = 0, dwLevel=0, dwStatusTmp=0, dwId=0;
        if((dwRet = IGetInfo(dwId,dwLevel,dwStatusTmp,RS_NULL,RS_NULL)) != ERR_SUCCESS)
                return dwRet;
        return m_tree.SetItemInfo(m_hItem, dwId, dwLevel, dwStatus, RS_NULL, RS_NULL);
}
RS_RESULT CConfig::GetItemName(CHAR *pszName)
{
        RS_DWORD dwId=0, dwLevel=0, dwStatus=0;
        return m_tree.GetItemInfo(m_hItem, dwId, dwLevel, dwStatus, pszName, RS_NULL);
}
RS_RESULT CConfig::SetItemName(CHAR *pszName)
{
        RS_DWORD dwRet = 0, dwLevel=0, dwStatus=0, dwId=0;
        if((dwRet = IGetInfo(dwId,dwLevel,dwStatus,RS_NULL,RS_NULL)) != ERR_SUCCESS)
                return dwRet;
        return m_tree.SetItemInfo(m_hItem, dwId, dwLevel, dwStatus, pszName, RS_NULL);
}
RS_RESULT CConfig::GetItemMemo(CHAR *pszMemo)
{
        RS_DWORD dwId=0, dwLevel=0, dwStatus=0;
        return m_tree.GetItemInfo(m_hItem, dwId, dwLevel, dwStatus, RS_NULL, pszMemo);
}
RS_RESULT CConfig::SetItemMemo(CHAR *pszMemo)
{
        RS_DWORD dwRet = 0, dwLevel=0, dwStatus=0, dwId=0;
        if((dwRet = IGetInfo(dwId,dwLevel,dwStatus,RS_NULL,RS_NULL)) != ERR_SUCCESS)
                return dwRet;
        return m_tree.SetItemInfo(m_hItem, dwId, dwLevel, dwStatus, RS_NULL, pszMemo);
}
*/
// Deleted by LauranceZhang 2004-3-10 16:24:02 >

RS_RESULT CConfig::GetNodeStatus(RS_DWORD& dwStatus)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        RS_DWORD dwId = 0, dwLevel = 0;
        return !m_tree ? ERR_CFG_TREE_ISEMPTY : m_tree->GetItemInfo(m_hItem, dwId, dwLevel, dwStatus, RS_NULL, RS_NULL);
}

RS_RESULT CConfig::GetNodeAccess(RS_DWORD &dwAccess)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        return !m_tree ? ERR_CFG_TREE_ISEMPTY : m_tree->GetAccess(m_hItem, dwAccess);
}

RS_RESULT CConfig::SetNodeAccess(RS_DWORD dwAccess)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        return !m_tree ? ERR_CFG_TREE_ISEMPTY : m_tree->SetAccess(m_hItem, dwAccess);
}

RS_RESULT CConfig::LoadConfigEx(const RS_CHAR* pszFileName, IConfig** ppConfig, RS_DWORD dwLoadMode)
{
        CMemMutex memMutex(&g_cMutexMem);
        RS_RESULT dwRet = ERR_SUCCESS;
        if(ppConfig==RS_NULL)	//这时使用此对象本身装入配置文件
        {
                //因为是重新装入文件，所以要清除原来的内存
                if(m_tree)
                {
                        if(m_tree->m_dwReference > 1)
                                return ERR_CFG_TREE_REFNOTEMPTY;

                        delete m_tree;
                        m_tree = RS_NULL;
                        CSharedMem::Finalize();			//释放共享内存
                }

                CTree *pTree = new CTree;			//申请树对象，并装入文件
                if(!pTree)
                        return ERR_CFG_MEM_ALLOCFAILED;

                pTree->m_dwLoadMode = dwLoadMode;
                dwRet = pTree->InitTree(pszFileName);
                if(dwRet != ERR_SUCCESS)
                        return dwRet;

                ZeroData();
                m_tree = pTree;
                m_tree->m_dwReference ++;

                m_bTreeLoaded = RS_TRUE;

                OPExpireFlag(&g_dwExpiredFlag, RS_FALSE);
                OPInstallFlag(g_dwInstallFlag, RS_FALSE);
        }
        else
        {
                //创建一个新的对象
                IConfig *pConfig = (IConfig*)new CConfig;
                if(pConfig == RS_NULL)
                        return ERR_CFG_MEM_ALLOCFAILED;

                dwRet = pConfig->LoadConfig(pszFileName);
                if(dwRet != ERR_SUCCESS)
                {
                        delete pConfig;
                        pConfig = RS_NULL;
                        return dwRet;
                }
                *ppConfig = pConfig;
                m_bTreeLoaded = RS_TRUE;
        }
        return ERR_SUCCESS;
}
RS_RESULT CConfig::LoadConfig(const RS_CHAR* pszFileName, IConfig** ppConfig)
{
        return LoadConfigEx(pszFileName, ppConfig, LOAD_MODE_INIT);
}

RS_RESULT CConfig::LoadConfig(CTree *pTree)
{
        if(!pTree)
                return ERR_CFG_TREE_ISEMPTY;

        //清除原来的内存
        if(m_tree)
        {
                if(m_tree->m_dwReference > 1)
                        return ERR_CFG_TREE_REFNOTEMPTY;
                delete m_tree;
                ZeroData();
                m_tree = pTree;
                m_tree->m_dwReference++;
        }
        else
        {
                ZeroData();
                m_tree = pTree;
                m_tree->m_dwReference++;
        }

        m_bTreeLoaded = RS_TRUE;
        return ERR_SUCCESS;
}

RS_RESULT CConfig::AddConfig(IConfig* pConfig)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        return ERR_SUCCESS;
}


//==============================以下方法供编辑器使用//==============================
RS_RESULT CConfig::GetProductID(RS_CHAR *pszSN, RS_LONG &lPrtID)
{
        CCheckSn *pCheckSn = RS_NULL;
        //验证序列号的合法性
        pCheckSn = new CCheckSn;
        RS_DWORD dwSize = strlen(pszSN);
        if(!pCheckSn)
                return ERR_CFG_MEM_ALLOCFAILED;
        if( ERR_SN_VAL_SUCCESS != pCheckSn->CheckSn(pszSN, dwSize, 0))
        {
                delete pCheckSn;
                return ERR_SN_CHECK_FAILED;
        }
        if(lPrtID==-1)
                lPrtID = pCheckSn->GetTranslatedProductID();
        else
                lPrtID = pCheckSn->GetProductID();
        delete pCheckSn;
        return ERR_SUCCESS;
}

HITEM CConfig::GetRoot()
{
        CNode* pRoot = (CNode *)((CNode *)m_tree->m_pRoot);
        return (HITEM)pRoot;
}

HITEM CConfig::GetSibling(HITEM hParent)
{
        if(hParent == RS_NULL)
                return RS_NULL;
        CNode* pSibling = (CNode*)((CNode*)hParent)->GetSibling();
        return (HITEM)pSibling;
}

HITEM CConfig::GetChild(HITEM hParent)
{
        if(hParent == RS_NULL)
                return RS_NULL;
        CNode* pChild = (CNode*)((CNode*)hParent)->GetChild();
        return (HITEM)pChild;
}

RS_RESULT CConfig::Create(const RS_DWORD* pdwIdPath, PHITEM phItem, RS_DWORD &dwAccess, const RS_DWORD* pdwUserId)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        if(!m_tree)
                return ERR_CFG_TREE_ISEMPTY;

        //检测参数的合法性
        int i = 0;
        for(i=0; i<RS_MAX_CFGPATH_LEN; i++)
        {
                if(pdwIdPath[i] == 0)
                        break;
        }
        if(i >= RS_MAX_CFGPATH_LEN)
                return ERR_CFG_PARAM_INVALID;

        //为成员参数赋值
        memcpy(m_adwPath, pdwIdPath, sizeof(RS_DWORD) * (i + 1));
        m_nPosition = i;

        m_dwAccess = dwAccess;
        memcpy(m_adwUser, pdwUserId, sizeof(RS_DWORD) * RS_USERID_LEN);

        RS_RESULT dwRet = ERR_SUCCESS;
        dwRet = m_tree->OpenItem(pdwIdPath, phItem, dwAccess, pdwUserId);
        if(ERR_SUCCESS != dwRet)
                return dwRet;

        m_hItem = *phItem;
        return dwRet;
}

RS_RESULT CConfig::Close(PHITEM phItem)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        MyTrace("CConfig::Close == %x", *phItem);
        return m_tree->CloseItem(phItem);
}

RS_RESULT CConfig::GetValue(HITEM hItem, RS_DWORD& dwDataType, RS_BYTE* pbyData, RS_DWORD& dwSize)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        return m_tree->GetItemValue(hItem, dwDataType, pbyData, dwSize);
}

RS_DWORD CConfig::SetValue(HITEM hItem, RS_DWORD dwDataType, RS_BYTE* pbyData, RS_DWORD dwSize)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        return m_tree->SetItemValue(hItem, dwDataType, pbyData, dwSize);
}

RS_RESULT CConfig::GetInfo(HITEM hItem, RS_DWORD& dwId, RS_DWORD& dwLevel, RS_DWORD& dwStatus, RS_CHAR* pszName, RS_CHAR* pszMemo)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        return m_tree->GetItemInfo(hItem, dwId, dwLevel, dwStatus, pszName, pszMemo);
}

RS_DWORD CConfig::SetInfo(HITEM hItem, RS_DWORD dwId, RS_DWORD dwLevel, RS_DWORD dwStatus, RS_CHAR* pszName, RS_CHAR* pszMemo)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        return m_tree->SetItemInfo(hItem, dwId, dwLevel, dwStatus, pszName, pszMemo);
}

RS_DWORD CConfig::GetMemo(HITEM hItem, RS_CHAR* pszMemo)
{
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        return m_tree->GetItemMemo(hItem, pszMemo);
}

RS_DWORD CConfig::SetMemo(HITEM hItem, RS_CHAR* pszMemo)
{
        if(pszMemo)
        {
                if(strncmp(pszMemo, "OPGETSNTIME:", 12)==0)
                        return OPGetSNTime((RS_BYTE *)(pszMemo + 12), RS_TRUE);
        }

        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        return m_tree->SetItemMemo(hItem, pszMemo);
}

RS_BOOL CConfig::Save(HITEM hItem)
{
        return (ERR_SUCCESS==m_tree->SaveToFile()) ? RS_TRUE : RS_FALSE;
}

RS_DWORD CConfig::DeleteNode(HITEM hItem)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        return m_tree->DeleteItem(hItem);
}

RS_RESULT CConfig::Synchronize(RS_VOID *pParam)
{
        CMemMutex memMutex(&g_cMutexMem);
        if(!m_bTreeLoaded)
                return ERR_CFG_TREE_LOADFAILED;

        return m_tree->SaveToFile(pParam);
}

//以超级权限读值
RS_RESULT CConfig::SuperGetValue(const RS_DWORD* pdwIdPath, RS_DWORD &dwDataType,  RS_BYTE* pbyData, RS_DWORD& dwSize)
{
        if(pdwIdPath[0] == INVALID_ITEMID)
                return ERR_CFG_PARAM_INVALID;

        CNode *pNode = RS_NULL;
        RS_DWORD dwStatus = 0, dwAccess = 0, dwOpAccess = 0;
        RS_DWORD dwRet = ERR_SUCCESS;
        RS_DWORD adwUser[RS_USERID_LEN] = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
        HITEM hItem = 0;
        // 先查询出此项的句柄
        dwOpAccess = CONFIG_ACCESS_QUERY;
        dwRet = m_tree->OpenItem(pdwIdPath, &hItem, dwOpAccess, adwUser);
        if((ERR_SUCCESS == dwRet) && hItem)	//此项是存在的
        {
                //保存状态和权限
                pNode = (CNode *)hItem;
                dwStatus = pNode->GetStatus();
                dwAccess = pNode->GetAccess();
                hItem = 0;
        }
        else
                return dwRet;

        // 先打开此项
        dwOpAccess = CONFIG_ACCESS_ALL | CONFIG_ACCESS_SUPER;
        dwRet = m_tree->OpenItem(pdwIdPath, &hItem, dwOpAccess, adwUser);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        //得到配置项值
        dwRet = m_tree->GetItemValue(hItem, dwDataType, pbyData, dwSize);
        if(ERR_SUCCESS != dwRet)
        {
                m_tree->CloseItem(&hItem);
                return dwRet;
        }
        //关闭
        dwRet = m_tree->CloseItem(&hItem);
        if(ERR_SUCCESS != dwRet)
                return dwRet;

        if(pNode)	//恢复
        {
                pNode->SetStatus(dwStatus);
                pNode->SetAccess(dwAccess);
        }
        return dwRet;
}

//以超级权限写值
RS_RESULT CConfig::SuperSetValue(const RS_DWORD* pdwIdPath, RS_DWORD dwDataType,  const RS_BYTE* pbyData, RS_DWORD& dwSize)
{
        if(pdwIdPath[0] == INVALID_ITEMID)
                return ERR_CFG_PARAM_INVALID;

        CNode *pNode = RS_NULL;
        RS_DWORD dwStatus = 0, dwAccess = 0, dwOpAccess = 0;
        RS_DWORD dwRet = ERR_SUCCESS;
        RS_DWORD adwUser[RS_USERID_LEN] = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
        HITEM hItem = 0;
        // 先查询出此项的句柄
        dwOpAccess = CONFIG_ACCESS_QUERY;
        dwRet = m_tree->OpenItem(pdwIdPath, &hItem, dwOpAccess, adwUser);
        if((ERR_SUCCESS == dwRet) && hItem)	//此项是存在的
        {
                //保存状态和权限
                pNode = (CNode *)hItem;
                dwStatus = pNode->GetStatus();
                dwAccess = pNode->GetAccess();
                hItem = 0;
        }

        // 先打开此项
        dwOpAccess = CONFIG_ACCESS_ALL | CONFIG_ACCESS_SUPER;
        dwRet = m_tree->OpenItem(pdwIdPath, &hItem, dwOpAccess, adwUser);
        if(ERR_SUCCESS != dwRet)
                return dwRet;
        //写入配置项值
        dwRet = m_tree->SetItemValue(hItem, dwDataType, pbyData, dwSize);
        if(ERR_SUCCESS != dwRet)
        {
                m_tree->CloseItem(&hItem);
                return dwRet;
        }
        //关闭
        dwRet = m_tree->CloseItem(&hItem);
        if(ERR_SUCCESS != dwRet)
                return dwRet;

        if(pNode)	//恢复
        {
                pNode->SetStatus(dwStatus);
                pNode->SetAccess(dwAccess);
        }
        return dwRet;
}

RS_RESULT CConfig::GetConfigHeader(RS_DDWORD &dwHeaderAddr)
{
        if(!g_pBasePtr)
                return ERR_CFG_FILE_HEADINVALID;
        dwHeaderAddr = (RS_DDWORD)g_pBasePtr;
        return ERR_SUCCESS;
}
RS_RESULT CConfig::QueryUser(RS_DWORD *pUserID)
{
        RS_DWORD dwAccess = 0;
        if(ERR_SUCCESS != m_tree->GetUserAccess(pUserID, 7, dwAccess))
                return ERR_FAILED;
        return ERR_SUCCESS;
}

RS_RESULT CConfig::Create(PSTConfigItem pstItem, IConfig** ppConfig)
{
        CMemMutex memMutex(&g_cMutexMem);
        MyTrace("Create [--->] Path = %x->%x->%x->%x", m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3]);
        if(!m_bTreeLoaded)
        {
                FormatErrorMessage(ERR_CFG_TREE_LOADFAILED, __FILE__, __LINE__, "Create");
                return ERR_CFG_TREE_LOADFAILED;
        }

        //==================================================
        if((pstItem->m_dwStatus & CONFIG_STATE_IS_BRANCH) != CONFIG_STATE_IS_BRANCH)
        {
                if(ppConfig)
                        return ERR_CFG_PARAM_INVALID;
        }
        else
        {
                if( pstItem->m_dwLevel == 0 )
                        return ERR_CFG_PARAM_INVALID;
        }

        RS_RESULT dwRet = ERR_SUCCESS;
        RS_DWORD adwPath[RS_MAX_CFGPATH_LEN] = {0};
        RS_DWORD nPosition = 0;
        if(m_pParent && !m_hItem)
        {
                dwRet = m_pParent->GetNodePath(adwPath, nPosition);
                if(ERR_CFG_BUFFER_OVERFLOW == dwRet)
                        dwRet = m_pParent->GetNodePath(adwPath, nPosition);
                if(ERR_SUCCESS != dwRet)
                        return dwRet;
        }
        else
        {
                memcpy(adwPath, m_adwPath, sizeof(RS_DWORD) * m_nPosition);
                nPosition = m_nPosition;
        }
        adwPath[nPosition] = pstItem->m_dwId;
        nPosition++;
        //==================================================

        //查询此项是否存在
        dwRet = Query(pstItem->m_dwId);
        if(ERR_SUCCESS != dwRet)
        {
                if((pstItem->m_dwAccess & CONFIG_ACCESS_CREATE) != CONFIG_ACCESS_CREATE)
                        return ERR_CFG_NODE_NORIGHT;
        }

        if(!ppConfig)
        {
                HITEM hItem = RS_NULL;
                dwRet = m_tree->OpenItem(adwPath, &hItem, pstItem->m_dwAccess, pstItem->m_pdwUserId);
                if(ERR_SUCCESS != dwRet)
                {
                        FormatErrorMessage(dwRet, __FILE__, __LINE__, "Create");
                        return dwRet;
                }

                //如果是结点
                if((pstItem->m_dwStatus & CONFIG_STATE_IS_BRANCH) == CONFIG_STATE_IS_BRANCH)
                {
                        ((CNode *)hItem)->SetLevel(pstItem->m_dwLevel);
                        ((CNode *)hItem)->AppendStatus(pstItem->m_dwStatus);
                }
                else
                {
                        //((CNode *)hItem)->ExcludeStatus(CONFIG_STATE_IS_BRANCH);	//类型要设为叶子
                }

                if(pstItem->m_dwDataLen)
                {
                        dwRet = ((CNode *)hItem)->SetDataType(pstItem->m_dwDataType);
                        dwRet = ((CNode *)hItem)->ResetData(pstItem->m_pData, pstItem->m_dwDataLen);
                }
                if(pstItem->m_pName)
                        dwRet = ((CNode *)hItem)->ResetName(pstItem->m_pName);
                if(pstItem->m_pMemo)
                        dwRet = ((CNode *)hItem)->ResetMemo(pstItem->m_pMemo);
                if(dwRet != ERR_SUCCESS)
                {
                        m_tree->DeleteItem(hItem);
                        return dwRet;
                }
                if((pstItem->m_dwStatus & CONFIG_STATE_IS_BRANCH) == CONFIG_STATE_IS_BRANCH)
                {
                        m_hItem = hItem;
                        memcpy(m_adwPath, adwPath, sizeof(RS_DWORD) * nPosition);
                        m_nPosition = nPosition;
                        m_dwAccess = pstItem->m_dwAccess;
                        memcpy(m_adwUser, pstItem->m_pdwUserId, sizeof(RS_DWORD) * RS_USERID_LEN);
                }
        }
        else
        {
                //New 一个新的对象
                CConfig* pConfig = new CConfig;
                if(!pConfig)
                {
                        FormatErrorMessage(ERR_CFG_MEM_ALLOCFAILED, __FILE__, __LINE__, "Create");
                        return ERR_CFG_MEM_ALLOCFAILED;
                }

                dwRet = pConfig->LoadConfig(m_tree);
                if(dwRet != ERR_SUCCESS)
                {
                        delete pConfig;
                        pConfig = RS_NULL;
                        FormatErrorMessage(dwRet, __FILE__, __LINE__, "Create");
                        return dwRet;
                }

                pConfig->SetParentEnv(this);

                //创建此结点
                dwRet = pConfig->Create(pstItem, 0);
                if(ERR_SUCCESS != dwRet)
                {
                        delete pConfig;
                        pConfig = RS_NULL;
                        FormatErrorMessage(dwRet, __FILE__, __LINE__, "Create");
                        return dwRet;
                }
                //返回参数
                *ppConfig = (IConfig*)pConfig;
        }
        MyTrace("Create [Ret=%x] Path = %x->%x->%x->%x", dwRet, m_adwPath[0], m_adwPath[1], m_adwPath[2], m_adwPath[3]);
        return ERR_SUCCESS;
}

RS_RESULT CConfig::SetParentEnv(CConfig *pParent)
{
        m_pParent = pParent;
        return ERR_SUCCESS;
}
