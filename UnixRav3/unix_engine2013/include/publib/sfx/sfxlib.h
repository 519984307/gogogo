#ifndef __SFXLIB_H__
#define __SFXLIB_H__

#if defined(_MSC_VER) && _MSC_VER > 1000
#pragma once
#endif


CCriticalSection& g_cs_sfxlib(); //extern


#include <memory>
#include <vector>
#include <algorithm>
#include "sfxlibfile.h"

#define ASSERT_OK(exp) { HRESULT _rt = (exp); if (_rt != S_OK ) return FALSE;}
#define SafeRead8(value, pos) \
	{	\
		if((pos)+1>dwFileLen) { _records.clear(); return FALSE; }	\
		(value) = *(BYTE *)(pFileData+(pos));		\
		(pos) += 1;	\
	}
#define SafeRead32(value, pos)  \
	{	\
		if((pos)+4>dwFileLen) { _records.clear(); return FALSE; }	\
		(value) = GETDWORD((pFileData+(pos)));	\
		(pos) += 4;	\
	}
#define SafeRead16(value, pos) \
	{	\
		if ((pos)+2>dwFileLen) { _records.clear(); return FALSE; } \
		(value) = GETWORD((pFileData+(pos)));	\
		(pos) += 2;	\
	}
#define SafeReadBytes(value, pos, len) \
	{	\
		if ((pos)+len>dwFileLen) { _records.clear(); return FALSE; } \
		(value) = RSNEW BYTE[(len)]; \
		if (NULL == (value)) return FALSE;  \
		memcpy((value), pFileData+(pos), (len));  \
		_binary_convert((value), (len));	\
		pos += (len); \
	}
#define SafeReadChars(value, pos, len) \
	{	\
		if ((pos)+len>dwFileLen) { _records.clear(); return FALSE; } \
		(value) = RSNEW CHAR[(len)]; \
		if (NULL == (value)) return FALSE;  \
		memcpy((value), pFileData+(pos), (len));  \
		_binary_convert((BYTE *)(value), (len));	\
		pos += (len); \
	}

/*******************************************************************************

							STArchiveSearchMethod

*******************************************************************************/
// 要求文件中存储的SearchMethod 同一个SearchMethod的不同pos， 必须按大小顺序。 因为这个不排序
// 同样. 如果count相同. 则每个count的pos必不同 (因为pos相同的话只是len不同的话,为了效率考虑.因该归为一个method)
typedef struct __tagArchiveSearchMethod
{
	DWORD SearchId;
	DWORD SearchCount;
	DWORD *SearchPos;
	DWORD *SearchLen;
	DWORD SearchDescLen;
	CHAR *SearchDesc;
	DWORD ReserveCount;
	__tagArchiveSearchMethod(){ memset(this, 0, sizeof(__tagArchiveSearchMethod)); }
	__tagArchiveSearchMethod(const __tagArchiveSearchMethod & method)
	{
		memset(this, 0, sizeof(__tagArchiveSearchMethod));
		*this = method;
	}
	const __tagArchiveSearchMethod & operator = (const __tagArchiveSearchMethod & method)
	{
		if (this==&method) return *this;
		Release();
		memcpy(this, &method, sizeof(__tagArchiveSearchMethod));
		if (SearchCount > 0)
		{
			SearchPos = RSNEW DWORD[SearchCount];
			memcpy(SearchPos, method.SearchPos, sizeof(DWORD) * SearchCount);
			SearchLen = RSNEW DWORD[SearchCount];
			memcpy(SearchLen, method.SearchLen, sizeof(DWORD) * SearchCount);
		}
		if (SearchDescLen > 0)
		{
			SearchDesc = RSNEW CHAR[SearchDescLen];
			memcpy(SearchDesc, method.SearchDesc, sizeof(CHAR) * SearchDescLen);
		}
		return *this;
	}
	~__tagArchiveSearchMethod() { Release(); }
	void Release() 
	{
		if (SearchCount>0)
		{
			if (SearchPos) delete []SearchPos;
			if (SearchLen) delete []SearchLen;
		}
		if (SearchDescLen>0)
		{
			if (SearchDesc) delete []SearchDesc;
		}
		memset(this, 0, sizeof(__tagArchiveSearchMethod));
	}
}STArchiveSearchMethod;

/*******************************************************************************

							STArchiveCorp

*******************************************************************************/
typedef struct __tagArchiveCorp
{
	DWORD	SearchId;
	DWORD	Type;		   	// for example: ARCHIVE_RAR
	DWORD	CorpLen;
	CHAR		*Corp;			// for example: "rarlib"
	DWORD	CorpWebLen;
	CHAR		*CorpWeb;		// for example: "www.rarlib.com"
	DWORD	VersionCount;	// 1
	DWORD	*Version;
	DWORD	*VersionStringLen;
	CHAR		**VersionString;

	__tagArchiveCorp() { memset(this, 0, sizeof(__tagArchiveCorp)); }
	__tagArchiveCorp(const __tagArchiveCorp & corp)
	{
		memset(this, 0, sizeof(__tagArchiveCorp));
		*this = corp;
	}
	const __tagArchiveCorp & operator = (const __tagArchiveCorp & corp)
	{
		if (this == &corp) return *this;
		Release();
		memcpy(this, &corp, sizeof(__tagArchiveCorp));
		if (CorpLen > 0)
		{
			Corp = RSNEW CHAR[CorpLen];
			memcpy(Corp, corp.Corp, sizeof(CHAR) * CorpLen);
		}
		if (CorpWebLen > 0)
		{
			CorpWeb = RSNEW CHAR[CorpWebLen];
			memcpy(CorpWeb, corp.CorpWeb, sizeof(CHAR) * CorpWebLen);
		}
		if (VersionCount > 0)
		{
			Version = RSNEW DWORD[VersionCount];
			memcpy(Version, corp.Version, sizeof(DWORD) * VersionCount);
			VersionStringLen = RSNEW DWORD[VersionCount];
			memcpy(VersionStringLen, corp.VersionStringLen, sizeof(DWORD) * VersionCount);
			VersionString = RSNEW CHAR *[VersionCount];
			memcpy(VersionString, corp.VersionString, sizeof(CHAR *) * VersionCount);
			for (DWORD i=0; i<VersionCount; ++i)
			{
				if (VersionStringLen[i] > 0)
				{
					VersionString[i] = RSNEW CHAR[VersionStringLen[i]];
					memcpy(VersionString[i], corp.VersionString[i], sizeof(CHAR) * VersionStringLen[i]);
				}
			}
		}
		return *this;
	}
	~__tagArchiveCorp() { Release(); }
	void Release()
	{
		if (Corp) delete[]Corp;
		if (CorpWeb) delete[]CorpWeb;
		if (VersionCount>0)
		{
			if (Version) delete[]Version;
			if (VersionStringLen) delete []VersionStringLen;
			if (VersionString)
			{
				for(DWORD i=0; i<VersionCount; ++i)
					if (VersionString[i]) delete []VersionString[i];
				delete []VersionString;
			}
		}
		memset(this, 0, sizeof(__tagArchiveCorp));
	}
}STArchiveCorp;


/*******************************************************************************

							STArchiveMark

*******************************************************************************/
// 同一个method对应的全部mark中, count必然相同(都为method.SearchCount), 但len可以为0表示匹配的时候这个不搜
typedef struct __tagArchiveMark
{
	DWORD	PEStringCount;				//1
	DWORD 	*PEStringLen;				//22
	BYTE		**PEString;					//A159004200C1E002A35D004200575133C0BFB8144200
	DWORD	ArchiveStringLen;
	BYTE 	*ArchiveString;				//526172211a0700
	DWORD	SFXPos;						//e000
	DWORD	IPos;						//31d3
	DWORD	ILen;						// 54
	BYTE		*IString;					// 52 '?' 00 22 61 '?' 00 05 72 '?' 00 05 21 '?' 00 05 1a '?' 00 05 07 '?' 00 05 00
	__tagArchiveMark() { memset(this, 0, sizeof(__tagArchiveMark)); }
	__tagArchiveMark(const __tagArchiveMark & mark)
	{
		 memset(this, 0, sizeof(__tagArchiveMark));
		*this = mark;
	}
	const __tagArchiveMark & operator = (const __tagArchiveMark & mark)
	{
		if (this==&mark) return *this;
		Release();
		memcpy(this, &mark, sizeof(__tagArchiveMark));
		if (PEStringCount > 0)
		{
			PEStringLen = RSNEW DWORD[PEStringCount];
			memcpy(PEStringLen, mark.PEStringLen, sizeof(DWORD) * PEStringCount);
			PEString = RSNEW BYTE *[PEStringCount];
			memcpy(PEString, mark.PEString, sizeof(BYTE *) * PEStringCount);
			for (DWORD i=0; i<PEStringCount; ++i)
			{
				if (PEStringLen[i] > 0)
				{
					PEString[i] = RSNEW BYTE[PEStringLen[i]];
					memcpy(PEString[i], mark.PEString[i], sizeof(BYTE) * PEStringLen[i]);
				}
			}
		}
		if (ArchiveStringLen > 0)
		{
			ArchiveString = RSNEW BYTE[ArchiveStringLen];
			memcpy(ArchiveString, mark.ArchiveString, sizeof(BYTE) * ArchiveStringLen);
		}
		if (ILen > 0)
		{
			IString = RSNEW BYTE[ILen];
			memcpy(IString, mark.IString, sizeof(BYTE) * ILen);
		}
		return *this;
	}

	~__tagArchiveMark() { Release(); }
	void Release()
	{
		if (PEStringCount>0)
		{
			if (PEStringLen) delete []PEStringLen;
			if (PEString)
			{
				for (DWORD i=0; i<PEStringCount; ++i)
					if (PEString[i]) delete []PEString[i];
				delete []PEString;
			}
		}
		if (ArchiveString) delete []ArchiveString;
		if (IString) delete []IString;
		memset(this, 0, sizeof(__tagArchiveMark));
	}
}STArchiveMark;
inline BOOL operator < (const STArchiveMark &mark1, const STArchiveMark &mark2)
{
	if (mark1.PEStringCount != mark2.PEStringCount) return mark1.PEStringCount < mark2.PEStringCount;
	for (DWORD i=0; i<mark1.PEStringCount; ++i)
	{
		if (0 == mark1.PEStringLen[i] || 0 == mark2.PEStringLen[i]) continue;
		int iret = memcmp(mark1.PEString[i], mark2.PEString[i], MIN(mark1.PEStringLen[i], mark2.PEStringLen[i]));
		if (0 != iret) return iret<0;
		if (mark1.PEStringLen[i] != mark2.PEStringLen[i]) return mark1.PEStringLen[i] < mark2.PEStringLen[i];
	}
	return FALSE;
}


/*******************************************************************************

							STArchiveInfo

*******************************************************************************/
typedef struct __tagArchiveInfo
{
	STArchiveCorp corp;
	STArchiveMark mark;
	void Release() { corp.Release(); mark.Release(); }
}STArchiveInfo;
inline BOOL operator < (const STArchiveInfo &info1, const STArchiveInfo & info2)
{
	return info1.mark < info2.mark;
}
inline BOOL operator < (const STArchiveInfo &info, const std::vector< std::vector<BYTE> > &vecFileRead)
{
	if (info.mark.PEStringCount != vecFileRead.size()) return info.mark.PEStringCount < vecFileRead.size();
	for (DWORD i=0; i<info.mark.PEStringCount; ++i)
	{
		if (0 == info.mark.PEStringLen[i] || 0 == vecFileRead[i].size()) continue;
		int iret = memcmp(info.mark.PEString[i], &vecFileRead[i][0], MIN(info.mark.PEStringLen[i], (DWORD)(vecFileRead[i].size())));
		if ( 0!= iret) return iret<0;
		//if (info.mark.PEStringLen[i] != vecFileRead[i].size()) return info.mark.PEStringLen[i] < vecFileRead[i].size();
	}
	return FALSE;
}
inline BOOL operator < (const std::vector< std::vector<BYTE> > &vecFileRead, const STArchiveInfo &info)
{
	if (vecFileRead.size() != info.mark.PEStringCount) return vecFileRead.size() < info.mark.PEStringCount;
	for (DWORD i=0; i<info.mark.PEStringCount; ++i)
	{
		if (0 == info.mark.PEStringLen[i] || 0 == vecFileRead[i].size()) continue;
		int iret = memcmp( &vecFileRead[i][0], info.mark.PEString[i], MIN(info.mark.PEStringLen[i], (DWORD)(vecFileRead[i].size())));
		if (0 != iret) return iret<0;
		//if (vecFileRead[i].size() != info.mark.PEStringLen[i]) return vecFileRead[i].size() < info.mark.PEStringLen[i];
	}
	return FALSE;
}



/*******************************************************************************

							STSFXRecord

*******************************************************************************/
typedef struct __tagSFXRecord
{
	STArchiveSearchMethod method;
	std::vector< STArchiveInfo > info;
	void Release() 
	{ 
		method.Release(); 
		for (std::vector< STArchiveInfo >::iterator it=info.begin(); it!=info.end(); ++it)
		{
			it->Release();

		}
	}
}STSFXRecord;
inline BOOL operator < (const STSFXRecord &record1, const STSFXRecord &record2)
{
	return record1.method.SearchId < record2.method.SearchId;
}
inline BOOL operator < (const STSFXRecord &record, const DWORD searchid)
{
	return  record.method.SearchId < searchid;
}
inline BOOL operator < (const DWORD searchid, const STSFXRecord &record)
{
	return searchid < record.method.SearchId;
}

//#include 
#include "../synchronization.h"

class CSfxLib
{
public:
	// singleton
	static CSfxLib &GetInstance() {CCSLock lock(g_cs_sfxlib());  static CSfxLib lib; return lib;  }	

	// constructor
	BOOL Init(IREStream &file) 
	{
		static BOOL bInit  = FALSE;
		static BOOL bRet = FALSE;
		if (bInit) return bRet;
		bInit = TRUE;

		bRet = ForceInit(file);

		return bRet;
	}
	BOOL ForceInit(IREStream &file)
	{
		file.Seek(0, 0, SPBegin);
		_records.clear();

		// read file
		DWORD dwFileLen, dwRead, dwRecordCount;
		ASSERT_OK(file.GetSize(&dwFileLen, 0));
		BYTE * pFileData = RSNEW BYTE[(dwFileLen+3)/4*4];
		std::auto_ptr<BYTE> for_free (pFileData);
		memset(pFileData, 0,  (dwFileLen+3)/4*4 * sizeof(BYTE) );
		ASSERT_OK(file.Read(pFileData, dwFileLen, &dwRead));
		if (dwFileLen != dwRead) return FALSE;
		if (dwFileLen < 16 ) return FALSE;

		// mark
		DWORD pos=0;
		if (0 != memcmp(pFileData+pos, "RSEXT\00", 6))  return FALSE;
		pos += 6;
		
		// file version
		SHORT file_version;
		SafeRead16(file_version, pos);

		// file time
		time_t file_time;
		SafeRead32(file_time, pos);
		//char * ppppp = ctime(&file_time);
		//tm *_tm = gmtime(&file_time);

		// crc check
		DWORD dwCRC = 0;
		for (DWORD i = 0; i<dwFileLen; i+=4)
		{
			dwCRC ^= GETDWORD((pFileData+i));
		}
		if (0 != dwCRC) return FALSE;

		// parse data
		pos = 16;
		for (; ;)
		{
			STArchiveSearchMethod method;
			STArchiveCorp corp;
			STArchiveMark mark;
			STArchiveInfo info;
			STSFXRecord record;

			DWORD pos_bak = pos;

			BYTE block_type; 
			DWORD block_len, i;
			SafeRead8(block_type, pos);
			SafeRead32(block_len, pos);

			switch(block_type)
			{
			case 0x10:	// head
				SafeRead32(dwRecordCount, pos);
				if (dwRecordCount > 0) _records.reserve(dwRecordCount);
				break;
			case 0x11:	// search
				SafeRead32(method.SearchId, pos);
				SafeRead32(method.SearchCount, pos);
				if (method.SearchCount>0)
				{
					method.SearchPos = RSNEW DWORD[method.SearchCount];
					method.SearchLen = RSNEW DWORD[method.SearchCount];
					for (i = 0; i<method.SearchCount; ++i)
					{
						SafeRead32(method.SearchPos[i], pos);
						SafeRead32(method.SearchLen[i], pos);
					}
				}
				else
				{
					method.SearchPos = NULL;
					method.SearchLen = NULL;
				}
				SafeRead32(method.SearchDescLen, pos);
				if (method.SearchDescLen > 0)
				{
					SafeReadChars(method.SearchDesc, pos, method.SearchDescLen);
				}
				else
				{
					method.SearchDesc = NULL;
				}
				SafeRead32(method.ReserveCount, pos);
				record.method = method;

				// insert record
				{
					std::pair<std::vector<STSFXRecord>::iterator, std::vector<STSFXRecord>::iterator> p 
						= std::equal_range(_records.begin(), _records.end(), record);
					if (p.first == p.second)
						//_records.insert(p.first, record);
						_records.insert(p.first, record)->info.reserve(method.ReserveCount);
				}
				break;
			case 0x12:  // info
				// SearchId && ArchiveType
				SafeRead32(corp.SearchId, pos);
				SafeRead32(corp.Type, pos);

				// Corp && Web
				SafeRead32(corp.CorpLen, pos);
				if (corp.CorpLen > 0)
				{
					SafeReadChars(corp.Corp, pos, corp.CorpLen);
				}
				else
				{
					corp.Corp = NULL;
				}
				SafeRead32(corp.CorpWebLen, pos);
				if (corp.CorpWebLen > 0)
				{
					SafeReadChars(corp.CorpWeb, pos, corp.CorpWebLen);
				}
				else
				{
					corp.CorpWeb = NULL;
				}

				// Version
				SafeRead32(corp.VersionCount, pos);
				if (corp.VersionCount > 0)
				{
					
					corp.Version = RSNEW DWORD[corp.VersionCount];
					corp.VersionStringLen = RSNEW DWORD[corp.VersionCount];
					corp.VersionString = RSNEW CHAR *[corp.VersionCount];
					for (i=0; i<corp.VersionCount; ++i)
					{
						SafeRead32(corp.Version[i], pos);
						SafeRead32(corp.VersionStringLen[i], pos);
						if (corp.VersionStringLen[i] > 0)
						{
							SafeReadChars(corp.VersionString[i], pos, corp.VersionStringLen[i]);
						}
						else
						{
							corp.VersionString[i] = NULL;
						}
					}
					
				}
				else
				{
					corp.Version = NULL;
					corp.VersionStringLen = NULL;
					corp.VersionString = NULL;
				}

				// PEString
				SafeRead32(mark.PEStringCount, pos);
				if (mark.PEStringCount > 0)
				{
					mark.PEStringLen = RSNEW DWORD[mark.PEStringCount];
					mark.PEString  = RSNEW BYTE *[mark.PEStringCount];
					for (i=0; i<mark.PEStringCount; ++i)
					{
						SafeRead32(mark.PEStringLen[i], pos);
						if (mark.PEStringLen[i] > 0)
						{
							SafeReadBytes(mark.PEString[i], pos, mark.PEStringLen[i]);
						}
						else
						{
							mark.PEString[i] = NULL;
						}
					}
				}
				else
				{	
					mark.PEStringLen = NULL;
					mark.PEString = NULL;
				}
				
				// ArchiveString
				SafeRead32(mark.ArchiveStringLen, pos);
				if (mark.ArchiveStringLen > 0)
				{
					SafeReadBytes(mark.ArchiveString, pos, mark.ArchiveStringLen);
				}
				else
				{
					mark.ArchiveString = NULL;
				}

				// SFXPos, IPos
				SafeRead32(mark.SFXPos, pos);
				SafeRead32(mark.IPos, pos);

				// IString
				SafeRead32(mark.ILen, pos);
				if (mark.ILen > 0)
				{
					SafeReadBytes(mark.IString, pos, mark.ILen);
				}
				else
				{
					mark.IString = NULL;
				}
				info.corp = corp;
				info.mark = mark;
				
				// insert into info
				{
					std::pair<std::vector<STSFXRecord>::iterator, std::vector<STSFXRecord>::iterator> p 
						= std::equal_range(_records.begin(), _records.end(), info.corp.SearchId);
					if (p.first != p.second)
						p.first->info.insert
						(p.first->info.end(), info);
				}
				break;
			default:
				break;
			}
			pos = pos_bak + block_len;
			if (pos == dwFileLen) break;
		}

		// sort Record, for each record(sort info)
		for (std::vector< STSFXRecord >::iterator it=_records.begin(); it!=_records.end(); ++it)
		{
			std::sort(it->info.begin(), it->info.end());
		}
		
		return TRUE;
	}

	// destructor
	~CSfxLib()
	{
	}

	// method -- search
	BOOL Search( IREStream *file, DWORD &archiveType, DWORD &startPos, BYTE * pMark, DWORD &dwMarkLen, BYTE *pIMark, DWORD &dwIMarkLen) 
	{
		DWORD i, dwRead;
		std::vector<STSFXRecord>::iterator it;
		std::vector< std::vector<BYTE> > vecFileRead;

		for(it=_records.begin(); it!=_records.end(); ++it)
		{
			vecFileRead.clear();
			vecFileRead.resize(it->method.SearchCount);

			for (i=0; i<it->method.SearchCount; ++i)
			{
				vecFileRead[i].resize(it->method.SearchLen[i]);
				if (!(S_OK == file->Seek(it->method.SearchPos[i], 0, SPBegin) && 
						S_OK == file->Read(&vecFileRead[i][0], it->method.SearchLen[i], &dwRead)))
					break;
			}
			if (i<it->method.SearchCount) continue;  //can't read file in this pos

			std::pair< std::vector<STArchiveInfo>::iterator, std::vector<STArchiveInfo>::iterator > p 
				= std::equal_range(it->info.begin(), it->info.end(), vecFileRead);
			if (p.first != p.second) // found!!!
			{
				archiveType = p.first->corp.Type;
				startPos = p.first->mark.SFXPos;
				if (pMark)
				{
					memcpy(pMark, p.first->mark.ArchiveString, p.first->mark.ArchiveStringLen);
					pMark[p.first->mark.ArchiveStringLen] = 0;
					dwMarkLen = p.first->mark.ArchiveStringLen;
				}
				if (pIMark)
				{
					pIMark[0] = 0;
					dwIMarkLen = 0;
					DWORD pos1=0, pos2=0,  dwICnt=0, dwMaxICnt=0;
					const STArchiveMark & mark = p.first->mark;

					if (mark.ILen > 0)
					{
						// get read instruction len
						i=0;
						for (BOOL question=FALSE; ;)
						{
							if (question)
							{
								if (mark.IString[i++] == 0)
								{
									if (i >= mark.ILen) break;
									// ? 00 05 表示跳过5bytes //当有符号数加， 这样可以前跳！
									// 注意， 不能跳到第一个读的位置之前， 库里面必须保证.
									// 认为第一次读的始终是开始， 即便第一次匹配不在开始， 那么构造一个 “下跳” 来实现！
									dwICnt += (signed char)mark.IString[i++];	 //有符号加!!
									if (dwMaxICnt < dwICnt) dwMaxICnt = dwICnt;
								}
								else
								{
									++dwICnt;						// ? ? 表示?
									if (dwMaxICnt < dwICnt) dwMaxICnt = dwICnt;
								}
								question = FALSE;
							}
							else
							{
								if (mark.IString[i++] == '?')
									question = TRUE;
								else
								{
									++dwICnt;
									if (dwMaxICnt < dwICnt) dwMaxICnt = dwICnt;
								}
							}
							if (i >= mark.ILen) break;
						}
						dwICnt = dwMaxICnt;

						// get instruction mark
						std::vector<BYTE> vec(dwICnt);
						if (dwICnt>0 && S_OK == file->Seek(mark.IPos, 0, SPBegin) && 
										S_OK == file->Read(&vec[0], dwICnt, &dwRead) && 
										dwICnt == dwRead)
						{
							i = 0;
							for (BOOL question=FALSE; ;)
							{
								if (question)
								{
									if (mark.IString[i++] == 0)
									{
										if (i>= mark.ILen) break;
										pos2 += (signed char)mark.IString[i++]; //有符号数！， 可以前跳!
									}
									else
									{
										pIMark[pos1++] = vec[pos2++];
									}
									question = FALSE;
								}
								else
								{
									if (mark.IString[i++] == '?')
									{
										question = TRUE;
									}
									else
										pIMark[pos1++] = vec[pos2++];
								}
								if (i >= mark.ILen) break;
							}
							pIMark[pos1] = 0;
							dwIMarkLen = pos1;
						}	// get instruction mark
					} // mark.ilen >0
				} // pIMark != NULL
				return TRUE;
			}	// found
		}	// each method
		
		return FALSE;

	}

private:
	CSfxLib() {  Init(_libfile); }

private:
	CSfxLibFile _libfile;
	std::vector< STSFXRecord> _records;
	
private:
	// algorithms

	void _binary_convert(BYTE * bytes, DWORD dwLen)
	{
		BYTE key[]="rsextlib";
		DWORD key_len=sizeof(key)-1;
		DWORD i,j=0;
		for (i=0; i<dwLen; ++i)
		{
			if (j>key_len-1) j=0;
			bytes[i] ^= key[j];
			++j;
		}
	}

};


#endif //__SFXLIB_H__
