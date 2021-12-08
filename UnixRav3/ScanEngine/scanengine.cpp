#include "scanengine.h"
#include "scanconfig.h"
#include "engineobserver.h"
#include "baseconfig.h"
#include "commonapi.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <public/rame.h>
#include "scanengine.ver"

#define SECURITY_FREE(x) if(x) {delete x; x=NULL;}
#define SECURITY_CLOSE(x) if(x) {fclose(x); x=NULL;}
#define SECURITY_CHECK(x, y, z) if(x != y) return z;
#define ASSERT_ZERO(x, y) if(!x) return y;


ScanEngine* ScanEngine::NEW(EngineObserver *observer,
                            const std::string& rmdPath,
                            const std::string& libpath,
                            ScanConfig *config) {
    ScanEngine *self = new ScanEngine(observer);
    if (self) {
        if (self->Construct(rmdPath, libpath, config)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

ScanEngine::ScanEngine(EngineObserver *observer) {
    mObserver = observer;
    mScanConfig = NULL;
    mStatus = INITIALIZE;
}

ScanEngine::~ScanEngine() {
    SECURITY_FREE(mScanConfig);
    if(mObjectTable) mObjectTable->Dispose();
}

int ScanEngine::Construct(const std::string& rmdPath, const std::string& libpath, ScanConfig *config) {
    // path is empty, not load engine
    if (rmdPath.empty() || libpath.empty()) {
    	LOG_GJCZ("scan engine construction failed(%s,%s)", rmdPath.c_str(), libpath.c_str());
        return 0;
    }

    mRmdPath = rmdPath;
    mLibPath = libpath;

    mScanConfig = new ScanConfig;
    if (config) {
        *mScanConfig = *config;
    }

    if (S_OK == LoadScanEngine(rmdPath, libpath)) {
        mCallback = ScanCallBack::NEW(mObjectTable, mEngineParams);
        ASSERT_ZERO(mCallback, -1);

        mCallback->SetParent(this);
        return 0;
    }
    return -1;
}
void ScanEngine::SetConfig(const ScanConfig& config) {
    *mScanConfig = config;
}

std::string ScanEngine::GetCachePath() {
    char temp[128];

    int pos = mRmdPath.rfind("/");
    if (pos != std::string::npos) {
        strcpy(temp, mRmdPath.substr(0, pos).c_str());
    }
    return temp;
}

HRESULT ScanEngine::LoadScanEngine(const std::string& rmdPath, const std::string& libpath) {
    LOG_GJCZ_DEBUG("init scan engine core");
    RxSetHome(libpath.c_str());
//    MssSetCacheSize( 16*1024*1024 );

    RFAILED(ez::CreateInstance(mEngineParams, CLSID_Parameter));

    mEngineParams->SetParamString("kill=1");
    mEngineParams->SetParamString("mdu/rxxdu0.MduDMEV1=0");
    mEngineParams->SetParamString("precise-format=0");

    char temp[128];
    sprintf(temp, "viruslib_cache_path=#%s", GetCachePath().c_str());
    mEngineParams->SetParamString(temp);

    if (mScanConfig->mScanIntelligent) {
        // 此外为天津送检而屏掉，如果把这个打开，有些样本扫不出来
        if(mScanConfig->mScanCompressFile) {
            char szMaxCompSize[128] = {0};
    //        snprintf(szMaxCompSize, 63, "max-compound-size=%ld", mMaxZipSize*1024*1024);
    //        mEngineParams->SetParamString(szMaxCompSize);

            snprintf(szMaxCompSize, 63, "class.package=#%ld", (long)mScanConfig->mCompressSize*1024*1024);
            mEngineParams->SetParamString(szMaxCompSize);
        }
        else {
            mEngineParams->SetParamString("class.package=#0");
        }
    }

	if(mScanConfig->mCloudEnable && !mScanConfig->mCloudUrl.empty()) {
		// 引擎私有云
		const char * p;
		std::string  strUrl = mScanConfig->mCloudUrl;
		p = strstr(strUrl.c_str(), "http://");
		if(!p) strUrl.insert(0, "http://");
		p = strstr(strUrl.c_str(), "zdr/q3.php");
		if(!p) {
			if('/' != *strUrl.rbegin()) strUrl.append("/");
			strUrl.append("zdr/q3.php");
		}
		strUrl.insert(0, "cav.http.url=");
		mEngineParams->SetParamString("cav=1");
		mEngineParams->SetParamString(strUrl.c_str());
		mEngineParams->SetParamString("product=esm_xc");
	}

    RFAILED(ez::CreateInstance(mObjectTable, CLSID_ObjectTable));
    RFAILED(ez::CreateInstance(mEngine, CLSID_FrontEnd, 0,mObjectTable));   
    RFAILED(mObjectTable->Register(PID_ROT_EngineParameter, mEngineParams));

    UTIL::com_ptr<IMssLibFile> oLibFile;
    RFAILED (MssLoadLibFile2(rmdPath.c_str(), 0, &oLibFile.m_p, mObjectTable));
    RFAILED (mEngine->Init2( oLibFile, mEngineParams ));
    RFAILED(ez::RunTimeGet(mFileSystem, mObjectTable, PID_ROT_FileSystem ));

    LOG_GJCZ("init scan engine core OK(%d,%s)", mScanConfig->mCloudEnable, mScanConfig->mCloudUrl.c_str());

    return S_OK;
}

HRESULT ScanEngine::ScanOneFile(const char *fileName) {
    if (access(fileName, R_OK) == 0) {
        UTIL::com_ptr<IRXStream> stm;
        RFAILED( ez::XCreateStream( stm, mFileSystem, fileName, FOMRM ) );
        return mEngine->Process(stm, mCallback);
    }
    return S_OK;
}


ScanEngine::ScanCallBack* ScanEngine::ScanCallBack::NEW(IRXObjectTable *ot, IRXParameters *lp) {
    ScanEngine::ScanCallBack *self = new ScanEngine::ScanCallBack;
    if (self) {
        if (self->Construct(ot, lp)) {
            delete self;
            self = NULL;
        }
    }
    return self;
}

int ScanEngine::ScanCallBack::Construct(IRXObjectTable *ot, IRXParameters *lp) {
    mObjectTable = ot;
    mEngineParams = lp;

    UTIL::com_ptr<IUnknown> punk;
    RFAILED(mObjectTable->GetObject(PID_ROT_ObjectLoader, &punk.m_p));

    UTIL::com_ptr<IREObjectLoader> pol(punk);
    RFAILED(pol->CreateInstance(mObjectTable, CLSID_CREVirusInfoLib, NULL,IREVirusInfoLib::iid, (VOID **)&mVirusInfo.m_p));

    xv::xvalue_t doKill;
    if( SUCCEEDED(mEngineParams->GetParam( "kill", doKill))) {
        _Opt = _13::PROC_WILL_TREAT;
        INT32 doKillI = 0;
        if( doKill.get( doKillI ) && !doKillI) {
            _Opt = 0;
        }
    }
    return 0;
}

EFC ScanEngine::ScanCallBack::EnterFile(IRXStream *pstm, SIZE_T uNest) {
    LOG_GJCZ_DEBUG("EnterFile start");
    if (mParent->mStatus == ScanEngine::TERMINAL) {
        return EFC_ABORT;
    }

    string fileName;
    if (S_OK == GetFullName(fileName, pstm)) {
        if (mParent) {
            if (mParent->mObserver) {
                mParent->mObserver->OnVirusEvent(EngineObserver::VIRUS_JUDGE, EngineObserver::JUDGE_START, &fileName);
            }
        }
    }
    LOG_GJCZ_DEBUG("EnterFile %s", fileName.c_str());
    return EFC_CONTINUE;
}

PROCOPT ScanEngine::ScanCallBack::GetProcOptions() {
    return _Opt | PROC_NO_SEH;
}

void ScanEngine::ScanCallBack::LeaveFile( IRXStream * pstm, SIZE_T uNest, LEAVECASE uCase) {
    LOG_GJCZ_DEBUG("LeaveFile start");
    string fileName;
    if (S_OK == GetFullName(fileName, pstm)) {
        if (mParent) {
            if (mParent->mObserver) {
                mParent->mObserver->OnVirusEvent(EngineObserver::VIRUS_JUDGE, EngineObserver::JUDGE_END, &fileName);
            }
        }
    }
    //
    if( uNest == 0 )
    {
        UTIL::com_ptr<IRXLogicFile> oLogicFile(pstm);
        UTIL::com_ptr<IREStream> oBackStream;
        RFAILEDV( oLogicFile->GetBackStream( &oBackStream.m_p, NULL ) );
        if( pstm->GetStatus() & FOSDeleted )
        {
            oLogicFile->ModifyMode( IRXLogicFile::COW_DELIVER_REMOVE, 0 );
            pstm->Remove();
        }
        else if( pstm->GetStatus() & FOSChanged )
        {
            HRESULT hrClean = oBackStream->SetAccess(FOMRW);
            if( SUCCEEDED(hrClean))
                hrClean = oLogicFile->FlushBack();
        }
    }

    xv::xvalue_t opt;
    INT32 uOpt = 0;

    if( SUCCEEDED(mEngineParams->GetParam("dtf",opt) ) && opt.get(uOpt) && uOpt )
    {
        UTIL::com_ptr<IRXDTDataBox> oBox;
        RX_BLOB oDTBlk;
        if( SUCCEEDED(ez::GetAuxDataBox(oBox,pstm)) &&
            SUCCEEDED(oBox->DTFCGetBlock( oDTBlk )) )
        {
            std::string tmp;
            tmp.assign( (LPCSTR)oDTBlk.Data, (LPCSTR)oDTBlk.Data + oDTBlk.Length );
        }
    }

    opt.clear();
    if( SUCCEEDED(mEngineParams->GetParam("tj11",opt) ) && opt.get(uOpt) && uOpt )
    {
        UTIL::com_ptr<IRXSig64List> oSigList;
        SIG64EX oSig64;
        if( SUCCEEDED( ez::GetSig64List( oSigList, pstm, FALSE, FALSE ) ) &&
            SUCCEEDED( oSigList->GetByClass( SIG64_PE_TJ11, &oSig64 ) ) )
        {
            std::string hex;
            ez::HexDump( hex, &oSig64.Hash, sizeof( oSig64.Hash ) );
        }
    }
    if( SUCCEEDED(mEngineParams->GetParam("ssi",opt) ) && opt.get(uOpt) && uOpt )
    {
        xv::xvalue_t msg;
        UTIL::com_ptr<IRXDTDataBox> oBox;
        if( SUCCEEDED(ez::GetAuxDataBox(oBox,pstm)) && SUCCEEDED( oBox->DTXVGet( "CON::ISUBMIT", msg )) && msg.isMBS() )
        {
        }
    }
    LOG_GJCZ_DEBUG("LeaveFile %s", fileName.c_str());
}

void ScanEngine::ScanCallBack::EnterArchive( IRXArchive * arch ) {
//    UTIL::com_ptr<IREStream> pstm;
//    arch->GetStreamObject( &pstm.m_p );

//    std::string strdispName;
//    GetDisplayName(pstm, strdispName);
}

void ScanEngine::ScanCallBack::LeaveArchive( IRXArchive * arch, TRESULT trRebuild ) {

//    UTIL::com_ptr<IREStream> pstm;
//    arch->GetStreamObject( &pstm.m_p );

//    std::string strdispName;
//    GetDisplayName(pstm, strdispName);
}

TEFC ScanEngine::ScanCallBack::OnAlarm( IRXArchive * arch, SGID40 uSigId ) {
    LOG_GJCZ_DEBUG("OnAlarm start");
    if (mParent) {
        if (mParent->mStatus == ScanEngine::TERMINAL) {
            return TEFC_ABORT;
        }

        if (mParent->mObserver) {
            struct STRsVirusInfo stRsVirusInfo;
            std::string dispName,virusName;
            GetVirusName(arch, uSigId, dispName, virusName, &stRsVirusInfo);

            std::pair<std::string, std::string> package = std::make_pair(dispName, virusName);
            int err = mParent->mObserver->OnVirusEvent(EngineObserver::VIRUS_JUDGE, EngineObserver::JUDGE_DANGE, &package);
            return (enum TEFC)err;
        }
    }

    return TEFC_IGNORE;
}

TEFC ScanEngine::ScanCallBack::OnTreated( IRXArchive * arch, SGID40 uSigId, TRESULT trTreat ) {
    LOG_GJCZ_DEBUG("OnTreated start");

    if (mParent->mStatus == ScanEngine::TERMINAL) {
        return TEFC_ABORT;
    }

    if (TR_S_TREAT == trTreat) {
        struct STRsVirusInfo stRsVirusInfo;
        std::string dispName,virusName;
        GetVirusName(arch, uSigId, dispName, virusName, &stRsVirusInfo);
//        GetFilename(arch, dispName);
        if (!dispName.empty()) {
//            if (access(dispName.c_str(), W_OK) == 0)
            {
//                unlink(dispName.c_str());
                return TEFC_REMOVE;
            }
        }
    }
    else if (TR_E_FAIL == trTreat) {

    }

    return TEFC_IGNORE;
}

TEFC ScanEngine::ScanCallBack::OnAlarm( IRXArchive * arch, IExtDetectionResult * result ) {
	LOG_GJCZ_DEBUG("OnAlarm startX");

	UTIL::com_ptr<IREStream> stm;
	RFAILED_(arch->GetStreamObject(stm.pp()), TEFC_IGNORE);
	
	if(mParent) {
		if(mParent->mStatus == ScanEngine::TERMINAL) {
			return TEFC_ABORT;
		}
		
		if(mParent->mObserver) {
			ULONG       nFormatId = 0;
			SGID40      nSigId;
			LPCSTR      szFmt, szVirName;
			std::string strDispName, strVirusName;

			RFAILED_(GetDisplayName(stm, strDispName), TEFC_IGNORE);
			nSigId = result->MalwareId40();
			arch->GetFormatID(&nFormatId);
			szFmt  = fmtid2name(nFormatId);
			szVirName = result->MalwareName();
			if(szVirName) {
				/*if(szFmt) tcs::xprintf(strVirusName, "%s:%s", szFmt, szVirName);
				else*/      tcs::xprintf(strVirusName, "%s",    szVirName);
				if(!nSigId.AreaId) {
					std::string fix;
					tcs::xprintf(fix, "!%d", nSigId.Id32);
					strVirusName += fix;
				}
			}
			else {
				/*if(szFmt) tcs::xprintf(strVirusName, "%s:Malware.UDM.!%X.%X", szFmt, nSigId.AreaId, nSigId.Id32);
				else*/      tcs::xprintf(strVirusName, "Malware.UDM.!%X.%X",    nSigId.AreaId, nSigId.Id32);
			}

			std::pair<std::string, std::string> package = std::make_pair(strDispName, strVirusName);
            int err = mParent->mObserver->OnVirusEvent(EngineObserver::VIRUS_JUDGE, EngineObserver::JUDGE_DANGE, &package);
            return (enum TEFC)err;
		}
	}

	return TEFC_IGNORE;
}

TEFC ScanEngine::ScanCallBack::OnTreated( IRXArchive * arch, IExtDetectionResult * result, TRESULT trTreat ) {
	LOG_GJCZ_DEBUG("OnTreated startX");
	
	if (mParent->mStatus == ScanEngine::TERMINAL) {
		return TEFC_ABORT;
	}

	if(TR_S_TREAT == trTreat) {
		std::string strDispName;
		UTIL::com_ptr<IREStream> stm;

		RFAILED_(arch->GetStreamObject(stm.pp()), TEFC_IGNORE);
		RFAILED_(GetDisplayName(stm, strDispName), TEFC_IGNORE);
		if(!strDispName.empty()) {
			return TEFC_REMOVE;
		}
	}
	
	return TEFC_IGNORE;
}


HRESULT ScanEngine::ScanCallBack::OnComplexCallback( xv::Value& inVar, xv::Value& outVar ) {
    return E_NOTIMPL;
}

HRESULT ScanEngine::ScanCallBack::QueryLibFileVersion( LPCSTR aLibFileName, UINT32 uCurrent, UINT32 * lpLastVer ) {
    return E_FAIL;
}

HRESULT ScanEngine::ScanCallBack::OnTreatedEx( IRXArchive * arch, SGID40 uSigId, TEFC& rHowTreat, TRESULT trTreat, SIZE_T uTreatCount ) {
    return E_NOTIMPL;
}

DWORD ScanEngine::ScanCallBack::GetVirusName(
        IRXArchive *arch,
        SGID40 uSigId,
        std::string &strDispName,
        std::string &strVirusName,
        STRsVirusInfo *pstVirusInfo) {

    std::string oVirName;

    ULONG ulFormatId = 0;
    arch->GetFormatID( &ulFormatId );

    LPCSTR strFmt = fmtid2name(ulFormatId);
    mVirusInfo->GetVirusInfo(uSigId.Id32, pstVirusInfo);

    UTIL::com_ptr<IRXNameDB> oNameDB;
    if (SUCCEEDED( ez::RunTimeGet( oNameDB, mObjectTable, PID_ROT_NameDB ))) {
        CHAR vName[256] = {0};
        oNameDB->GetNameById40( uSigId, vName, sizeof(vName) );
        if( strFmt ) {
            tcs::xprintf( oVirName, "%s:%s", strFmt, vName );
        }
        else {
            tcs::xprintf( oVirName, "%s", vName );
        }
        if( !uSigId.AreaId ) {
            std::string suffix;
            tcs::xprintf( suffix, "!%d", uSigId.Id32 );
            oVirName.append( suffix );
        }
        strVirusName = vName;
    }
    else {
        if( strFmt ) {
            tcs::xprintf( oVirName, "%s:Malware.UDM!%X.%X", strFmt, uSigId.AreaId, uSigId.Id32 );
        }
        else {
            tcs::xprintf( oVirName, "Malware.UDM!%X.%X", uSigId.AreaId, uSigId.Id32 );
        }
    }
    UTIL::com_ptr<IRXStream> pstm;
    RASSERT(SUCCEEDED(arch->GetStreamObject((IREStream**)&pstm.m_p)), EFC_CONTINUE);
    RASSERT(GetDisplayName(pstm, strDispName), EFC_CONTINUE);

    return EFC_CONTINUE;
}

HRESULT ScanEngine::ScanCallBack::GetFilename(IRXArchive * arch, std::string& filename) {
    UTIL::com_ptr<IREStream> stm;
    RFAILED_( arch->GetStreamObject( stm.pp() ), TEFC_IGNORE );

    UTIL::com_ptr<IRXStream> pstm(stm);
    XSTRP strp;
    RFAILED( pstm->GetName( strp ) );
    tcs::xstr_t xstr;
    if( !xstr.set(strp) ) {
        return E_FAIL;
    }
    filename = xstr.StrMBS;

    return S_OK;
}

HRESULT ScanEngine::ScanCallBack::GetDisplayName(IREStream *lpStream, std::string& rFullName) {
    std::vector< std::string > names;
    UTIL::com_ptr<IRXStream> oCurrent = lpStream;
    while( oCurrent ) {
        XSTRP strp;
        RFAILED( oCurrent->GetName( strp ) );
        tcs::xstr_t xstr;
        if( !xstr.set(strp) ) {
            return E_FAIL;
        }
        names.push_back( xstr.StrMBS );

        UTIL::com_ptr<IRXStream> oParentStream;
        UTIL::com_ptr<IRXArchive> oParent;
        if( FAILED( oCurrent->GetParent(&oParent.m_p ) ) ) break;
        RFAILED( oParent->GetStreamObject((IREStream**)&oParentStream.m_p ) );
        oCurrent = oParentStream;
    }

    size_t k = names.size();
    for( size_t i = 0; i < k; ++ i ) {
        rFullName.append( names[k-1-i] );
        if( k - i > 1 ) {
            rFullName.append( "::" );
        }
    }

    return S_OK;
}

HRESULT ScanEngine::ScanCallBack::GetFullName(std::string& rFullName, IRXStream *lpStream)
{
    std::vector< std::string > names;
    UTIL::com_ptr<IRXStream> oCurrent = lpStream;
    while( oCurrent ) {
        XSTRP strp;
        RFAILED( oCurrent->GetName( strp ) );

        if( !strp.IsUnicode ) {
            names.push_back( strp.NameA );
        }
        else {
            tcs::xstr_t xstr;
            if( !xstr.set(strp) ) {
                std::string tmp;
                names.push_back( tmp );
            }
            else {
                names.push_back( xstr.StrMBS );
            }
        }

        UTIL::com_ptr<IREStream> oParentStream;
        UTIL::com_ptr<IRXArchive> oParent;
        if( FAILED( oCurrent->GetParent(&oParent.m_p ) ) ) {
            break;
        }
        RFAILED( oParent->GetStreamObject( &oParentStream.m_p ) );
        oCurrent = oParentStream;
    }

    if( names.empty() ) {
        return E_FAIL;
    }

    size_t k = names.size();
    for( size_t i = 0; i < k; ++ i ) {
        rFullName.append( names[k-1-i] );
        if( k - i > 1 ) {
            rFullName.append( "::" );
        }
    }

    return S_OK;
}

int ScanEngine::SetCurrentRunningPath(std::string& runningPath) {
    std::string tmp;
    tmp.resize(2048);
    ssize_t len = ::readlink("/proc/self/exe", &tmp[0], tmp.size() - 1);
    if(len <= 0 || len == tmp.size() - 1) {
        if(NULL == realpath("./", &tmp[0])) {
            return -1;
        }
        runningPath = tmp;
        return 0;
    }
    tmp[len] = 0;
    char * p = tcsrchr( tmp.c_str(), '/' );
    if( p ) {
        *p = '\x00';
    }
    runningPath = tmp;
    return 0;
}

HRESULT ScanEngine::UpdateVirusLib(const char* naviXml, const char *tempFolder, const char *rmdPath) {
    UTIL::com_ptr<mss::ILibFileUp> updateCallback;
//    RFAILED( ez::mss::CreateInstance( updateCallback, CLSID_MssLibUpdater ) );

    int err = ez::mss::CreateInstance( updateCallback, CLSID_MssLibUpdater ) ;
    char *virusName = NULL;

    VirusUploadCallback ucb(mObserver);

    HRESULT hr = updateCallback->Init(rmdPath, naviXml, &ucb, virusName);
    if(hr == 1) {
        return 12;
    }

    err = updateCallback->Update(tempFolder, &ucb);
    if(err) {
        LOG_GJCZ("update failed, error code -21, (%d,%s)", err, tempFolder);
        return -21;
    }
//    updateCallback->SetCachePath(GetCachePath().c_str());
    err = updateCallback->NotifyUpdated();
    if (err) {
        LOG_GJCZ("update notification failed, error code -22(%d)", err);
        return -22;
    }

    return S_OK;//0
}

HRESULT ScanEngine::VirusUploadCallback::PushWantFile(LPCSTR fileName) {
    mUpdateFiles.push_back(fileName);
    if (mObserver) {
        return mObserver->OnVirusEvent(EngineObserver::VIRUS_UPDATE, EngineObserver::RP_DOWNLOAD, (void*)fileName);
    }
    return S_OK;
}

HRESULT ScanEngine::VirusUploadCallback::PushWorkEvent(UPEVENT idErr, LPCSTR fileName) {
    if (mObserver) {
        return mObserver->OnVirusEvent(EngineObserver::VIRUS_UPDATE, EngineObserver::RP_COMBINE, (void*)fileName);
    }
    return S_OK;
}

int ScanEngine::GetFileSIG(const string& filepath, string& hash) {
    UTIL::com_ptr<IRXStream> stm;
    RFAILED( ez::XCreateStream( stm, mFileSystem, filepath.c_str(), FOMRM ) );

    SIG64EX xsig64;
    UTIL::com_ptr<IRXSig64List> oSigList;
    RFAILED( ez::GetSig64List( oSigList, stm, FALSE, TRUE ) );
    RFAILED( sig64FetchS( stm, oSigList ) );
    if(SUCCEEDED( oSigList->GetByClass( SIG64_PE_TJ11, &xsig64 ) )) {
        ez::HexDump( hash, &xsig64.Hash, sizeof( xsig64.Hash ) );
        return 0;
    }
    else if( SUCCEEDED( oSigList->GetByClass( SIG64_SFX_FILE_NAMES, &xsig64 ))) {
        ez::HexDump( hash, &xsig64.Hash, sizeof( xsig64.Hash ) );
        return 0;
    }
    else if( SUCCEEDED( oSigList->GetByClass( SIG64_ANY_FCRC, &xsig64 ) )) {
        ez::HexDump( hash, &xsig64.Hash, sizeof( xsig64.Hash ) );
        return 0;
    }

    return -1;
}
