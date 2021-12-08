#pragma once
#include <vector>
using namespace std;
typedef vector< WORD* > LSTDATA;
class CGetDiskInfo
{
public:
	CGetDiskInfo(void);
	~CGetDiskInfo(void);
	INT GetDriverNum();
	HRESULT GetDriverSerial( INT iIndex, BYTE* pBuf, BYTE &bBufSize);
private:
	BOOL ReadPhysicalDriveInNT();
	BOOL ReadIdeDriveAsScsiDriveInNT();
	BOOL ReadPhysicalDriveInNTUsingSmart();
	BOOL ReadDrivePortsInWin9X();
	BOOL AddIfNew(USHORT *pIdSector);
	HRESULT  ConvertToString(WORD dwDiskData[256], CHAR* pszString ,BYTE &dwBufSize,int nFirstIndex, int nLastIndex);
private:
	BOOL m_bIs9x;
	LSTDATA m_list;
};
