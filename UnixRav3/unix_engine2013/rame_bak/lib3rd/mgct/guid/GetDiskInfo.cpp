#include "StdAfx.h"
#include <windows.h>
#include <winioctl.h>
#include <conio.h> 
#include ".\getdiskinfo.h"
#define  MAX_IDE_DRIVES  16
#pragma  pack(1)

#define  IDENTIFY_BUFFER_SIZE  512
#define MAX_WAIT_TIME	0x20

//  IOCTL commands
#define  DFP_GET_VERSION          0x00074080
#define  DFP_SEND_DRIVE_COMMAND   0x0007c084
#define  DFP_RECEIVE_DRIVE_DATA   0x0007c088

#define  FILE_DEVICE_SCSI              0x0000001b
#define  IOCTL_SCSI_MINIPORT_IDENTIFY  ((FILE_DEVICE_SCSI << 16) + 0x0501)
#define  IOCTL_SCSI_MINIPORT 0x0004D008  //  see NTDDSCSI.H for definition


#define SMART_GET_VERSION               CTL_CODE(IOCTL_DISK_BASE, 0x0020, METHOD_BUFFERED, FILE_READ_ACCESS)
#define SMART_SEND_DRIVE_COMMAND        CTL_CODE(IOCTL_DISK_BASE, 0x0021, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define SMART_RCV_DRIVE_DATA            CTL_CODE(IOCTL_DISK_BASE, 0x0022, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

//  GETVERSIONOUTPARAMS contains the data returned from the 
//  Get Driver Version function.
typedef struct _GETVERSIONOUTPARAMS
{
	BYTE bVersion;      // Binary driver version.
	BYTE bRevision;     // Binary driver revision.
	BYTE bReserved;     // Not used.
	BYTE bIDEDeviceMap; // Bit map of IDE devices.
	DWORD fCapabilities; // Bit mask of driver capabilities.
	DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;


//  Bits returned in the fCapabilities member of GETVERSIONOUTPARAMS 
#define  CAP_IDE_ID_FUNCTION             1  // ATA ID command supported
#define  CAP_IDE_ATAPI_ID                2  // ATAPI ID command supported
#define  CAP_IDE_EXECUTE_SMART_FUNCTION  4  // SMART commannds supported
/*
typedef struct _GETVERSIONINPARAMS {
	UCHAR    bVersion;               // Binary driver version.
	UCHAR    bRevision;              // Binary driver revision.
	UCHAR    bReserved;              // Not used.
	UCHAR    bIDEDeviceMap;          // Bit map of IDE devices.
	ULONG   fCapabilities;          // Bit mask of driver capabilities.
	ULONG   dwReserved[4];          // For future use.
} GETVERSIONINPARAMS, *PGETVERSIONINPARAMS, *LPGETVERSIONINPARAMS;
*/
/*
//  IDE registers
typedef struct _IDEREGS
{
	BYTE bFeaturesReg;       // Used for specifying SMART "commands".
	BYTE bSectorCountReg;    // IDE sector count register
	BYTE bSectorNumberReg;   // IDE sector number register
	BYTE bCylLowReg;         // IDE low order cylinder value
	BYTE bCylHighReg;        // IDE high order cylinder value
	BYTE bDriveHeadReg;      // IDE drive/head register
	BYTE bCommandReg;        // Actual IDE command.
	BYTE bReserved;          // reserved for future use.  Must be zero.
} IDEREGS, *PIDEREGS, *LPIDEREGS;
*/
/*
//  SENDCMDINPARAMS contains the input parameters for the 
//  Send Command to Drive function.
typedef struct _SENDCMDINPARAMS
{
	DWORD     cBufferSize;   //  Buffer size in bytes
	IDEREGS   irDriveRegs;   //  Structure with drive register values.
	BYTE bDriveNumber;       //  Physical drive number to send 
	//  command to (0,1,2,3).
	BYTE bReserved[3];       //  Reserved for future expansion.
	DWORD     dwReserved[4]; //  For future use.
	BYTE      bBuffer[1];    //  Input buffer.
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;
*/

//  Valid values for the bCommandReg member of IDEREGS.
#define  IDE_ATAPI_IDENTIFY  0xA1  //  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY    0xEC  //  Returns ID sector for ATA.
#define	 ID_CMD          0xEC            // Returns ID sector for ATA

/*
// Status returned from driver
typedef struct _DRIVERSTATUS
{
	BYTE  bDriverError;  //  Error code from driver, or 0 if no error.
	BYTE  bIDEStatus;    //  Contents of IDE Error register.
	//  Only valid when bDriverError is SMART_IDE_ERROR.
	BYTE  bReserved[2];  //  Reserved for future expansion.
	DWORD  dwReserved[2];  //  Reserved for future expansion.
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;
*/
/*
// Structure returned by PhysicalDrive IOCTL for several commands
typedef struct _SENDCMDOUTPARAMS
{
	DWORD         cBufferSize;   //  Size of bBuffer in bytes
	DRIVERSTATUS  DriverStatus;  //  Driver status structure.
	BYTE          bBuffer[1];    //  Buffer of arbitrary length in which to store the data read from the                                                       // drive.
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;
*/

// The following struct defines the interesting part of the IDENTIFY
// buffer:
typedef struct _IDSECTOR
{
	USHORT  wGenConfig;
	USHORT  wNumCyls;
	USHORT  wReserved;
	USHORT  wNumHeads;
	USHORT  wBytesPerTrack;
	USHORT  wBytesPerSector;
	USHORT  wSectorsPerTrack;
	USHORT  wVendorUnique[3];
	CHAR    sSerialNumber[20];
	USHORT  wBufferType;
	USHORT  wBufferSize;
	USHORT  wECCSize;
	CHAR    sFirmwareRev[8];
	CHAR    sModelNumber[40];
	USHORT  wMoreVendorUnique;
	USHORT  wDoubleWordIO;
	USHORT  wCapabilities;
	USHORT  wReserved1;
	USHORT  wPIOTiming;
	USHORT  wDMATiming;
	USHORT  wBS;
	USHORT  wNumCurrentCyls;
	USHORT  wNumCurrentHeads;
	USHORT  wNumCurrentSectorsPerTrack;
	ULONG   ulCurrentSectorCapacity;
	USHORT  wMultSectorStuff;
	ULONG   ulTotalAddressableSectors;
	USHORT  wSingleWordDMA;
	USHORT  wMultiWordDMA;
	BYTE    bReserved[128];
} IDSECTOR, *PIDSECTOR;

//
// IDENTIFY data (from ATAPI driver source)
//
typedef struct _IDENTIFY_DATA {
	USHORT GeneralConfiguration;            // 00 00
	USHORT NumberOfCylinders;               // 02  1
	USHORT Reserved1;                       // 04  2
	USHORT NumberOfHeads;                   // 06  3
	USHORT UnformattedBytesPerTrack;        // 08  4
	USHORT UnformattedBytesPerSector;       // 0A  5
	USHORT SectorsPerTrack;                 // 0C  6
	USHORT VendorUnique1[3];                // 0E  7-9
	USHORT SerialNumber[10];                // 14  10-19
	USHORT BufferType;                      // 28  20
	USHORT BufferSectorSize;                // 2A  21
	USHORT NumberOfEccBytes;                // 2C  22
	USHORT FirmwareRevision[4];             // 2E  23-26
	USHORT ModelNumber[20];                 // 36  27-46
	UCHAR  MaximumBlockTransfer;            // 5E  47
	UCHAR  VendorUnique2;                   // 5F
	USHORT DoubleWordIo;                    // 60  48
	USHORT Capabilities;                    // 62  49
	USHORT Reserved2;                       // 64  50
	UCHAR  VendorUnique3;                   // 66  51
	UCHAR  PioCycleTimingMode;              // 67
	UCHAR  VendorUnique4;                   // 68  52
	UCHAR  DmaCycleTimingMode;              // 69
	USHORT TranslationFieldsValid:1;        // 6A  53
	USHORT Reserved3:15;
	USHORT NumberOfCurrentCylinders;        // 6C  54
	USHORT NumberOfCurrentHeads;            // 6E  55
	USHORT CurrentSectorsPerTrack;          // 70  56
	ULONG  CurrentSectorCapacity;           // 72  57-58
	USHORT CurrentMultiSectorSetting;       //     59
	ULONG  UserAddressableSectors;          //     60-61
	USHORT SingleWordDMASupport : 8;        //     62
	USHORT SingleWordDMAActive : 8;
	USHORT MultiWordDMASupport : 8;         //     63
	USHORT MultiWordDMAActive : 8;
	USHORT AdvancedPIOModes : 8;            //     64
	USHORT Reserved4 : 8;
	USHORT MinimumMWXferCycleTime;          //     65
	USHORT RecommendedMWXferCycleTime;      //     66
	USHORT MinimumPIOCycleTime;             //     67
	USHORT MinimumPIOCycleTimeIORDY;        //     68
	USHORT Reserved5[2];                    //     69-70
	USHORT ReleaseTimeOverlapped;           //     71
	USHORT ReleaseTimeServiceCommand;       //     72
	USHORT MajorRevision;                   //     73
	USHORT MinorRevision;                   //     74
	USHORT Reserved6[50];                   //     75-126
	USHORT SpecialFunctionsEnabled;         //     127
	USHORT Reserved7[128];                  //     128-255
} IDENTIFY_DATA, *PIDENTIFY_DATA;
#pragma pack()
typedef struct _SRB_IO_CONTROL
{
	ULONG HeaderLength;
	UCHAR Signature[8];
	ULONG Timeout;
	ULONG ControlCode;
	ULONG ReturnCode;
	ULONG Length;
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;
#define SMART_RCV_DRIVE_DATA            CTL_CODE(IOCTL_DISK_BASE, 0x0022, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define  SENDIDLENGTH  sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE


typedef struct _rt_IdeDInfo_
{
	BYTE IDEExists[4];
	BYTE DiskExists[8];
	WORD DisksRawInfo[8*256];
} rt_IdeDInfo, *pt_IdeDInfo;


// (* IdeDinfo "data fields" *)
typedef struct _rt_DiskInfo_
{
	BOOL DiskExists;
	BOOL ATAdevice;
	BOOL RemovableDevice;
	WORD TotLogCyl;
	WORD TotLogHeads;
	WORD TotLogSPT;
	char SerialNumber[20];
	char FirmwareRevision[8];
	char ModelNumber[40];
	WORD CurLogCyl;
	WORD CurLogHeads;
	WORD CurLogSPT;
} rt_DiskInfo;


#define  m_cVxDFunctionIdesDInfo  1



BOOL DoIdentify (HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
						   PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
						   PDWORD lpcbBytesReturned)
{
	// Set up data structures for IDENTIFY command.
	pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;
	pSCIP -> irDriveRegs.bFeaturesReg = 0;
	pSCIP -> irDriveRegs.bSectorCountReg = 1;
	pSCIP -> irDriveRegs.bSectorNumberReg = 1;
	pSCIP -> irDriveRegs.bCylLowReg = 0;
	pSCIP -> irDriveRegs.bCylHighReg = 0;

	// Compute the drive number.
	pSCIP -> irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4);

	// The command can either be IDE identify or ATAPI identify.
	pSCIP -> irDriveRegs.bCommandReg = bIDCmd;
	pSCIP -> bDriveNumber = bDriveNum;
	pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;

	return ( DeviceIoControl (hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
		(LPVOID) pSCIP,
		sizeof(SENDCMDINPARAMS) - 1,
		(LPVOID) pSCOP,
		sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
		lpcbBytesReturned, NULL) );
}

CGetDiskInfo::CGetDiskInfo(void)
{
	OSVERSIONINFO version;
	memset (&version, 0, sizeof (version));
	version.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	GetVersionEx (&version);
	m_bIs9x=TRUE;
	if (version.dwPlatformId==VER_PLATFORM_WIN32_NT) 
	{
		m_bIs9x=FALSE;
	}
}

CGetDiskInfo::~CGetDiskInfo(void)
{
	for(UINT i = 0; i< m_list.size(); i++)
		delete m_list[i];
	m_list.clear();
}
INT CGetDiskInfo::GetDriverNum()
{
	if(m_list.size())
		return (INT)m_list.size();
	if(m_bIs9x)
	{
		ReadDrivePortsInWin9X();
		
	}
	else
	{
		ReadPhysicalDriveInNT();
		ReadIdeDriveAsScsiDriveInNT();
		ReadPhysicalDriveInNTUsingSmart();

	}
	return (INT)m_list.size();
}

HRESULT CGetDiskInfo::GetDriverSerial( INT iIndex, BYTE* pBuf, BYTE &bBufSize)
{
	if(iIndex>=(INT)m_list.size())
		return E_FAIL;
	 return ConvertToString(m_list[iIndex],(CHAR*)pBuf,bBufSize,10,19);
}
//NT架构系统下，通过deviceiocontrol函数获取物理磁盘信息，需要有管理员权限
BOOL CGetDiskInfo::ReadPhysicalDriveInNT()
{
	int done = FALSE;
	int drive = 0;
	BYTE IdOutCmd [sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];
	for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
	{
		HANDLE hPhysicalDriveIOCTL = 0;
		
		CHAR driveName [256];

		sprintf (driveName, "\\\\.\\PhysicalDrive%d", drive);

			hPhysicalDriveIOCTL = CreateFileA (driveName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);

		if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE)
		{
			GETVERSIONOUTPARAMS VersionParams;
			DWORD               cbBytesReturned = 0;

			// Get the version, etc of PhysicalDrive IOCTL
			memset ((void*) &VersionParams, 0, sizeof(VersionParams));

			if (  DeviceIoControl (hPhysicalDriveIOCTL, DFP_GET_VERSION,
				NULL, 
				0,
				&VersionParams,
				sizeof(VersionParams),
				&cbBytesReturned, NULL) )
			{         


				// If there is a IDE device at number "i" issue commands
				// to the device
				if (VersionParams.bIDEDeviceMap > 0)
			 {
				 BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
				 SENDCMDINPARAMS  scip;

				 // Now, get the ID sector for all IDE devices in the system.
				 // If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
				 // otherwise use the IDE_ATA_IDENTIFY command
				 bIDCmd = (VersionParams.bIDEDeviceMap >> drive & 0x10) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;

				 memset (&scip, 0, sizeof(scip));
				 memset (IdOutCmd, 0, sizeof(IdOutCmd));

				 if ( DoIdentify (hPhysicalDriveIOCTL, 
					 &scip, 
					 (PSENDCMDOUTPARAMS)&IdOutCmd, 
					 (BYTE) bIDCmd,
					 (BYTE) drive,
					 &cbBytesReturned))
				 {


					 USHORT *pIdSector = (USHORT *)((PSENDCMDOUTPARAMS) IdOutCmd) -> bBuffer;
					 AddIfNew(pIdSector);	  
					 done = TRUE;
				 }
			 }
			}

			CloseHandle (hPhysicalDriveIOCTL);
		}
	}

	return done;

}
BOOL CGetDiskInfo::ReadIdeDriveAsScsiDriveInNT (void)
{
	int done = FALSE;
	int controller = 0;

	for (controller = 0; controller < 16; controller++)
	{
		HANDLE hScsiDriveIOCTL = 0;
		CHAR   driveName [256];

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		sprintf (driveName, "\\\\.\\Scsi%d:", controller);

		//  Windows NT, Windows 2000, any rights should do
		hScsiDriveIOCTL = CreateFileA (driveName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);


		if (hScsiDriveIOCTL != INVALID_HANDLE_VALUE)
		{
			int drive = 0;

			for (drive = 0; drive < 2; drive++)
			{
				char buffer [sizeof (SRB_IO_CONTROL) + SENDIDLENGTH];
				SRB_IO_CONTROL *p = (SRB_IO_CONTROL *) buffer;
				SENDCMDINPARAMS *pin =
					(SENDCMDINPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
				DWORD dummy;

				memset (buffer, 0, sizeof (buffer));
				p -> HeaderLength = sizeof (SRB_IO_CONTROL);
				p -> Timeout = 10000;
				p -> Length = SENDIDLENGTH;
				p -> ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
				strncpy ((char *) p -> Signature, "SCSIDISK", 8);

				pin -> irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
				pin -> bDriveNumber = drive;

				if (DeviceIoControl (hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, 
					buffer,
					sizeof (SRB_IO_CONTROL) +
					sizeof (SENDCMDINPARAMS) - 1,
					buffer,
					sizeof (SRB_IO_CONTROL) + SENDIDLENGTH,
					&dummy, NULL))
				{
					SENDCMDOUTPARAMS *pOut =
						(SENDCMDOUTPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
					IDSECTOR *pId = (IDSECTOR *) (pOut -> bBuffer);
					if (pId -> sModelNumber [0])
					{

						USHORT *pIdSector = (USHORT *) pId;
						AddIfNew(pIdSector);	
						//BOOL bAdd = TRUE;
						//  for(UINT i =0; i< m_list.size();i++)
						//  {
						//   if(memcmp(pIdSector,m_list[i],256 * sizeof(WORD)) == 0)
						//   {
						//	   bAdd = false;
						//	   break;
						//   }
						//  }
						//  if(bAdd)
						//  {
						//    WORD* diskdata = new WORD[256];
						//	 ::memcpy(diskdata,pIdSector,256*sizeof(WORD));
						//	 m_list.push_back(diskdata);
						//  }

						done = TRUE;
					}
				}
			}
			CloseHandle (hScsiDriveIOCTL);
		}
	}

	return done;
}

BOOL CGetDiskInfo::ReadPhysicalDriveInNTUsingSmart (void)
{
	int done = FALSE;
	int drive = 0;

	for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
	{
		HANDLE hPhysicalDriveIOCTL = 0;

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.

		CHAR driveName [256];
		sprintf  (driveName, "\\\\.\\PhysicalDrive%d", drive);

		//  Windows NT, Windows 2000, Windows Server 2003, Vista
		hPhysicalDriveIOCTL = CreateFileA (driveName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 
			NULL, OPEN_EXISTING, 0, NULL);

		if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		{

		}
		else
		{
			GETVERSIONINPARAMS GetVersionParams;
			DWORD cbBytesReturned = 0;

			// Get the version, etc of PhysicalDrive IOCTL
			memset ((void*) & GetVersionParams, 0, sizeof(GetVersionParams));

			if (  DeviceIoControl (hPhysicalDriveIOCTL, SMART_GET_VERSION,
				NULL, 
				0,
				&GetVersionParams, sizeof (GETVERSIONINPARAMS),
				&cbBytesReturned, NULL) )
			{         
				// Allocate the command buffer
				ULONG CommandSize = sizeof(SENDCMDINPARAMS) + IDENTIFY_BUFFER_SIZE;
				PSENDCMDINPARAMS Command = (PSENDCMDINPARAMS) malloc (CommandSize);
				// Retrieve the IDENTIFY data
				// Prepare the command

				Command -> irDriveRegs.bCommandReg = ID_CMD;
				DWORD BytesReturned = 0;
				if ( DeviceIoControl (hPhysicalDriveIOCTL, 
					SMART_RCV_DRIVE_DATA, Command, sizeof(SENDCMDINPARAMS),
					Command, CommandSize,
					&BytesReturned, NULL) )

				{
					// Print the IDENTIFY data
					USHORT *pIdSector = (USHORT *)
						(PIDENTIFY_DATA) ((PSENDCMDOUTPARAMS) Command) -> bBuffer;


					AddIfNew(pIdSector);

					done = TRUE;
				}
				// Done
				CloseHandle (hPhysicalDriveIOCTL);
				free (Command);
		 }
		}
	}

	return done;
}
BOOL CGetDiskInfo::ReadDrivePortsInWin9X (void)
{
	return FALSE;

	//vxd驱动程序代码
	/*
	WORD abData[256];
	WORD wOffset;
	INT i;
	
	// SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	SetPriorityClass (GetCurrentProcess (), REALTIME_PRIORITY_CLASS);
	for(i=0;i<MAX_WAIT_TIME;i++)
	{
		if(inp (0x1F7)== 0x50) //等待端口空闲
			break;
		Sleep(MAX_WAIT_TIME);
	}
	outp (0x1F6,0xA0);

	outp (0x1F7, 0xEC); 
	
	for(i=0;i<MAX_WAIT_TIME;i++)
	{
		if(inp (0x1F7) == 0x58)
			break;
		Sleep(MAX_WAIT_TIME);

	}
	if(i<MAX_WAIT_TIME)
	{
		for (wOffset = 0; wOffset != 256; wOffset++)
		abData [wOffset] = inpw (0x1F0); 
		AddIfNew(abData);
	}
	//  reset the thread priority back to normal
	SetPriorityClass (GetCurrentProcess (), NORMAL_PRIORITY_CLASS);
	return TRUE;*/

	
}
BOOL CGetDiskInfo::AddIfNew(USHORT *pIdSector)
{
	BOOL bAdd = TRUE;
	for(UINT i =0; i< m_list.size();i++)
	{
		if(memcmp(pIdSector,m_list[i],256 * sizeof(WORD)) == 0)
		{
			bAdd = false;
			break;
		}
	}
	if(bAdd)
	{
		WORD* diskdata = new WORD[256];
		::memcpy(diskdata,pIdSector,256*sizeof(WORD));
		m_list.push_back(diskdata);
	}
	return bAdd;
}
HRESULT  CGetDiskInfo::ConvertToString(WORD dwDiskData[256], CHAR* pszString ,BYTE& dwBufSize,int nFirstIndex, int nLastIndex)
{
	char szResBuf[256];
	char ss[256];
	int nIndex = 0;
	int nPosition = 0;
	for(nIndex = nFirstIndex; nIndex <= nLastIndex; nIndex++)
	{
		ss[nPosition] = (char)(dwDiskData[nIndex] / 256);
		nPosition++;

		// Get low BYTE for 2nd character
		ss[nPosition] = (char)(dwDiskData[nIndex] % 256);
		nPosition++;
	}

	// End the string
	ss[nPosition] = '\0';

	int i, index=0;
	for(i=0; i<nPosition; i++)
	{
		if(ss[i]==0 || ss[i]==32)	continue;
		szResBuf[index]=ss[i];
		index++;
	}
	szResBuf[index]=0;
	if((dwBufSize<=index)||(pszString==NULL))
	{
		dwBufSize=index+1;
		return E_OUTOFMEMORY;
	}
	strcpy(pszString,szResBuf);
	dwBufSize=index;
	return S_OK;
}
