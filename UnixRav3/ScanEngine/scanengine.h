#ifndef SCANENGINE_H
#define SCANENGINE_H

#include <string>
#include "scanengine_global.h"

#include <public/rame.h>

using namespace _13;
using namespace mss;
using namespace std;

class ScanConfig;
class EngineObserver;
class ScanEngine
{
public:
    static ScanEngine *NEW(EngineObserver *observer,
                           const std::string& rmdPath,
                           const std::string& libpath,
                           ScanConfig *config=NULL);
    ~ScanEngine();

    void SetConfig(const ScanConfig& config);

    void SetObserver(EngineObserver *observer) {
        mObserver = observer;
    }
    enum ScanEngineType {
        VIRUS_ENGINE = 0x1,
        CLOUD_ENGINE
    };
    enum EnginePriority {
        PRIORITY_MASTER = 0,
        PRIORITY_SLAVE,
        PRIORITY_ABREAST
    };

    /** scan one file */
    HRESULT ScanOneFile(const char *fileName);
    /** update the virus engine */
    HRESULT UpdateVirusLib(const char* naviXmlUrl, const char *tempFolder, const char *rmdPath);
    /** set the engine priority, if you set one, the other auto been set*/
    void SetEnginePriority(enum ScanEngineType type, enum EnginePriority level);
    /** get the file crc */
    int GetFileSIG(const string& filepath, string& hash);

    void Terminal() {
        mStatus = TERMINAL;
    }

private:
    ScanEngine(EngineObserver *observer);
    int Construct(const std::string& rmdPath, const std::string& libpath, ScanConfig *config);

    HRESULT LoadScanEngine(const std::string& rmdPath, const std::string& libpath);

    int SetCurrentRunningPath(std::string& runningPath);
    std::string GetCachePath();

private:
    class ScanCallBack : public IRXCallbackX, public CUnknownImp {
    public:
        static ScanCallBack* NEW(IRXObjectTable *ot, IRXParameters *lp);
        virtual ~ScanCallBack(){}

        void SetParent(ScanEngine *engine) {
            mParent = engine;
        }

    private:
        ScanCallBack(){
            mParent = NULL;
        }
        int Construct(IRXObjectTable *ot, IRXParameters *lp);

        DWORD   GetVirusName(IRXArchive * arch, SGID40 uSigId, std::string &strdispName, std::string &strVirusName, STRsVirusInfo *pstVirusInfo);
        HRESULT GetDisplayName(IREStream *lpStream, std::string& rFullName);
        HRESULT GetFullName(std::string& rFullName, IRXStream *lpStream);
        HRESULT GetFilename(IRXArchive * arch, std::string& filename);

    public:
        UNKNOWN_IMP2_(IRXCallback,IRXCallbackX);
        STDMETHOD_(PROCOPT, GetProcOptions )();
        STDMETHOD_(EFC, EnterFile)(IRXStream *p, SIZE_T uNest);
        STDMETHOD_(VOID,LeaveFile)( IRXStream * pstm, SIZE_T uNest, LEAVECASE uCase );
        STDMETHOD_(VOID,EnterArchive)( IRXArchive * arch );
        STDMETHOD_(VOID,LeaveArchive)( IRXArchive * arch, TRESULT trRebuild );
        STDMETHOD_(TEFC, OnAlarm)( IRXArchive * arch, SGID40 uSigId );
        STDMETHOD_(TEFC, OnTreated)( IRXArchive * arch, SGID40 uSigId, TRESULT trTreat );
        STDMETHOD(OnComplexCallback)( xv::Value& inVar, xv::Value& outVar );
        STDMETHOD(QueryLibFileVersion)( LPCSTR aLibFileName, UINT32 uCurrent, UINT32 * lpLastVer );
        STDMETHOD(OnTreatedEx)( IRXArchive * arch, SGID40 uSigId, TEFC& rHowTreat, TRESULT trTreat, SIZE_T uTreatCount );

        // IRXCallbackX
        STDMETHOD_(TEFC, OnAlarm)( IRXArchive * arch, IExtDetectionResult * result );
        STDMETHOD_(TEFC, OnTreated)( IRXArchive * arch, IExtDetectionResult * result, TRESULT trTreat );

    private:
        PROCOPT		_Opt;

        UTIL::com_ptr<IRXParameters>    mEngineParams;
        UTIL::com_ptr<IRXObjectTable>   mObjectTable;
        UTIL::com_ptr<IREVirusInfoLib>  mVirusInfo;

        ScanEngine *mParent;
    };

    class VirusUploadCallback : public mss::ILibFileUpCallback {
    private:
        std::vector< std::string > mUpdateFiles;
        EngineObserver *mObserver;

    public:
        VirusUploadCallback(EngineObserver *observer) {
            mObserver = observer;
        }

        STDMETHOD(PushWantFile)(LPCSTR fileName);
        STDMETHOD(PushWorkEvent)(UPEVENT idErr, LPCSTR wFileName);
        STDMETHOD_(LPCSTR, GetObNamePrefix)() {
            return NULL;
        }
    };

private:
    UTIL::com_ptr<IRxFrontEnd>      mEngine;
    UTIL::com_ptr<IRXParameters>    mEngineParams;
    UTIL::com_ptr<IRXFileSystem>    mFileSystem;
    UTIL::com_ptr<IRXObjectTable>   mObjectTable;

    UTIL::com_ptr<ScanCallBack>     mCallback;

private:
    ScanConfig      *mScanConfig;
    EngineObserver  *mObserver;

    std::string mRmdPath;
    std::string mLibPath;

    enum {
        INITIALIZE = 0,
        TERMINAL = 2
    }mStatus;
};

#endif // SCANENGINE_H
