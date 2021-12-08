#ifndef __SFXTYPE_H__
#define __SFXTYPE_H__

#include "../matching.h"
#include "../utility.h"

#include "sfxlib.h"
#include "sfxguid.h"
#include "crange.h"

#include <string>


class CSfxType
{
public:
	enum
	{
		RSENGINE_UNKNOWN = 0,
		RSENGINE_RAREXE = 1,
		RSENGINE_ZIPEXE = 2,
		RSENGINE_NSISEXE = 3,
		RSENGINE_CABEXE = 4,
		RSENGINE_LZHEXE = 5,
		RSENGINE_INNOEXE = 6,
		RSENGINE_7ZEXE = 7,
		RSENGINE_ARJEXE = 8,
	};
public:
	CSfxType(IRERunningObjectTable * prot, IREStream *stream) 
	{
		_prot=prot; 
		_file=stream; 
		if(FALSE==_file2pefile()) 
			_pefile=(IREPEArchive *)0; 
		_bInitSuccess=_init(); 
	}
	~CSfxType(){}
	BOOL GetSfxType(DWORD &dwArchiveType, DWORD &dwPos, BOOL &bCanDel, DWORD *pLens=0 OPTIONAL);

protected:
	BOOL _file2pefile()
	{
		UTIL::com_ptr<IUnknown> punk;
		UTIL::com_ptr<IREObjectLoader> pol;
		RASSERT(SUCCEEDED(_prot->GetObject(PID_ROT_ObjectLoader, &punk.m_p)) && (pol = punk), FALSE);
		RASSERT(SUCCEEDED(pol->CreateInstance(_prot, CLSID_CREPEArchive, NULL, re_uuidof(IREPEArchive), (void **)&_pefile)) && _pefile, FALSE);
		return SUCCEEDED(_pefile->Open(UTIL::com_ptr<IUnknown>(_file))) ? TRUE : FALSE;
	}
	BOOL _init();
	BOOL _InitSuccess();
	BOOL _GetFristInvalidRawDataRange(DWORD &dwLeft, DWORD &dwRight);
	BOOL _GetNextInvalidRawDataRange(DWORD &dwLeft, DWORD &dwRight);
	BOOL _GetFristMarkSection(DWORD &dwLeft, DWORD &dwRight);
	BOOL _GetNextMarkSection(DWORD &dwLeft, DWORD &dwRight);

	BOOL _GoSectionEnd(DWORD &pos);
	BOOL _SearchMarkFromFO(DWORD dwStart, DWORD dwEnd, 
										DWORD dwMarkCount, BYTE **pMark, DWORD *dwMarkLen,  
										DWORD &dwWhere, DWORD & dwIndex);
	void _GetOrgStream(IREStream **ppstm);

	UTIL::com_ptr<IRERunningObjectTable> _prot;
	UTIL::com_ptr<IREStream> _file;
	UTIL::com_ptr<IREPEArchive> _pefile;
	CRange<DWORD> _range;
	BOOL _bInitSuccess;
	DWORD _dwSectionEnd;

	struct STSectionInfo
	{
		std::string _Name;
		DWORD _dwBeginPos;
		DWORD _dwEndPos;
		STSectionInfo() {}
		STSectionInfo(const STSectionInfo & another)
		{
			_Name = another._Name;
			_dwBeginPos = another._dwBeginPos;
			_dwEndPos = another._dwEndPos;
		}
	};
	std::vector<STSectionInfo> _Sections;
	std::vector<STSectionInfo>::iterator _itCurSections;
	

	struct CMarkSectionName
	{
	public:
		static CMarkSectionName * GetInstance()
		{
			static CMarkSectionName marksectionname;
			return &marksectionname;
		}
		~CMarkSectionName() {}
		
		std::vector<std::string> * GetNames()
		{
			return & _strNames;
		}

	private:
		std::vector<std::string> _strNames;
		CMarkSectionName()
		{
			_strNames.insert(_strNames.end(), "_winzip_");
			// add your new section name here
		}
	};



	struct CMark	// each unmap range float search
	{
	public:
		BYTE **ppMark;
		DWORD *pMarkLen;
		DWORD dwMarkCount;

		static CMark & GetInstance()
		{
			static CMark mark;
			return mark;
		}
		~CMark() { Release(); }
		void Release()
		{
			if (dwMarkCount > 0 &&  NULL != pMarkLen && NULL != ppMark)
			{
				for (DWORD i=0; i<dwMarkCount; ++i)
					if (pMarkLen[i] > 0 && NULL != ppMark[i]) delete []ppMark[i];
				delete []pMarkLen;
				delete []ppMark;
				if (NULL != pArchiveType) delete []pArchiveType;
			}
		}

		DWORD GetArchiveType(DWORD dwIndex) const
		{
			if (dwIndex > dwMarkCount-1) return 0;
			if (NULL != pArchiveType) return pArchiveType[dwIndex];
			return 0;
		}

	private:
		enum { STEP = 100 };
		DWORD *pArchiveType;
		DWORD dwCapacity;

		CMark()
		{
#define MARK_RAR "\x52\x61\x72\x21\x1A\x07\x00"
#define MARK_RAR2 "\x52\x61\x70\x21\x1A\x07\x00"  //目前碰到很多这样的样本！ 居然用
#define MARK_ZIP "\x50\x4B\x03\x04"
#define MARK_NSIS "\xEF\xBE\xAD\xDE\x4E\x75\x6C\x6C\x73\x6F\x66\x74\x49\x6E\x73\x74"
#define MARK_CAB "\x4D\x53\x43\x46\x00\x00\x00\x00"
#define MARK_LZH "\x2D\x6C\x68"
#define MARK_7Z	"\x37\x7A\xBC\xAF\x27\x1C"
#define MARK_ARJ "\x60\xEA"
			ppMark=NULL; pMarkLen=NULL; pArchiveType=NULL; dwMarkCount=0; dwCapacity=0;
			AddMark((BYTE *)MARK_RAR, sizeof(MARK_RAR)-1, RSENGINE_RAREXE);
			AddMark((BYTE *)MARK_RAR2, sizeof(MARK_RAR2)-1, RSENGINE_RAREXE);
			AddMark((BYTE *)MARK_ZIP, sizeof(MARK_ZIP)-1, RSENGINE_ZIPEXE);
			AddMark((BYTE *)MARK_NSIS, sizeof(MARK_NSIS)-1, RSENGINE_NSISEXE);
			AddMark((BYTE *)MARK_CAB, sizeof(MARK_CAB)-1, RSENGINE_CABEXE);
			AddMark((BYTE *)MARK_LZH, sizeof(MARK_LZH)-1, RSENGINE_LZHEXE);
			AddMark((BYTE *)MARK_7Z, sizeof(MARK_7Z)-1, RSENGINE_7ZEXE);
			AddMark((BYTE *)MARK_ARJ, sizeof(MARK_ARJ)-1, RSENGINE_ARJEXE);
#undef MARK_ARJ
#undef MARK_7Z
#undef MARK_LZH
#undef MARK_CAB
#undef MARK_NSIS
#undef MARK_ZIP
#undef MARK_RAR2
#undef MARK_RAR
		}
		void AddMark(BYTE * pMark, DWORD dwMarkLen, DWORD dwArchiveType = 0)
		{
			if (dwMarkCount == dwCapacity)
			{
				BYTE **pp = RSNEW BYTE *[dwMarkCount+STEP];
				memset(pp, 0, sizeof(BYTE *) * (dwMarkCount+STEP));
				if (NULL != ppMark)
					memcpy(pp, ppMark, sizeof(BYTE *) * dwMarkCount);
				ppMark = pp;

				DWORD *p = RSNEW DWORD [dwMarkCount+STEP];
				memset(p, 0, sizeof(DWORD) * (dwMarkCount+STEP));
				if (NULL != pMarkLen)
					memcpy(p, pMarkLen, sizeof(DWORD) * dwMarkCount);
				pMarkLen = p;

				p = RSNEW DWORD [dwMarkCount+STEP];
				memset(p, 0, sizeof(DWORD) * (dwMarkCount+STEP));
				if (NULL != pArchiveType)
					memcpy(p, pArchiveType, sizeof(DWORD) * dwMarkCount);
				pArchiveType = p;

				dwCapacity += STEP;
			}

			ppMark[dwMarkCount]= RSNEW BYTE[dwMarkLen];
			memcpy(ppMark[dwMarkCount], pMark, dwMarkLen);
			pMarkLen[dwMarkCount] = dwMarkLen;
			pArchiveType[dwMarkCount]= dwArchiveType;

			++dwMarkCount;
		}
	};

	struct CMark2	// fix pos float search
	{
		struct STMark
		{
			STMark() {mark=NULL;}
			~STMark() {if (NULL != mark) delete []mark;}
			DWORD fix_pos;
			BYTE *mark;
			DWORD mark_size;
			DWORD sfx_type;
		};

		std::vector<STMark *> _marks;
		static CMark2 & GetInstance()
		{
			static CMark2 mark;
			return mark;
		} 
		

	private:
		CMark2()
		{
#define MARK_LZH "\x2D\x6C\x68"
#define MARK_7Z	"\x37\x7A\xBC\xAF\x27\x1C"
			AddMark(0x664, (BYTE *)MARK_LZH, sizeof(MARK_LZH)-1, RSENGINE_LZHEXE);
			AddMark(0x49400, (BYTE *)MARK_7Z, sizeof(MARK_7Z)-1, RSENGINE_7ZEXE);
#undef MARK_7Z
#undef  MARK_LZH
		}
		~CMark2()
		{
			for (size_t i=0; i<_marks.size(); ++i)
			{
				if (NULL != _marks[i]) delete _marks[i];
			}
		}

		void AddMark(DWORD pos, BYTE *mark, size_t mark_size, DWORD sfx_type)
		{
			STMark * pmark = RSNEW STMark;
			pmark->fix_pos = pos;
			pmark->mark = RSNEW BYTE[mark_size];
			memcpy(pmark->mark, mark, mark_size);
			pmark->mark_size = (DWORD)mark_size;
			pmark->sfx_type = sfx_type;
			_marks.insert(_marks.end(), pmark);
		}
	};
};


inline
BOOL CSfxType::GetSfxType(DWORD &dwArchiveType, DWORD &dwPos, BOOL &bCanDel, DWORD *pLens)
{	
	RASSERT(_InitSuccess(), RS_FALSE);

	BYTE cbMark[255], cbIMark[255], cbTmp[255];
	BYTE *pcbMark = cbMark, *pcbIMark = cbIMark;
	DWORD dwMarkLen=0, dwIMarkLen=0, dwSFXPos=0, dwRead, dwIndex=(DWORD)-1;
	size_t i;
	const CMark &mark = CMark::GetInstance();
	const CMark2 &mark2 = CMark2::GetInstance();

	
	DWORD dwLeft, dwRight, dwSize=0, dwEnd=0;
	UTIL::com_ptr<IREPEResourceTable> prestbl(_pefile);

	// the origin stream property.
	UTIL::com_ptr<IREStream> orgstm;
	_GetOrgStream(&orgstm.m_p);
	UTIL::com_ptr<IREProperty> pprop(orgstm);
	RASSERT(pprop, FALSE);

	const PROPVARIANT *pvar;
	RASSERT(FAILED(pprop->GetProperty(PID_SFX_SearchFound, &pvar)), FALSE);//曾经找到过， 不要在找了.

	//如果下层认为可能存在imark， 则只搜索imark
	if (SUCCEEDED(pprop->GetProperty(PID_SFX_Signature, &pvar))) 
	{
		RASSERT(pvar && (pvar->vt==(VT_VECTOR|VT_UI1)) && pvar->caub.pElems && pvar->caub.cElems, FALSE);
		std::vector<BYTE> __search(pvar->caub.pElems, pvar->caub.pElems+pvar->caub.cElems);
		pcbMark = &__search[0], dwMarkLen = (DWORD)__search.size();
		// search all_archive_range  from each_unmap_Sections	
		if (_GetFristInvalidRawDataRange(dwLeft, dwRight))
		{
			do
			{
				dwEnd = dwRight;
				if (dwRight-dwLeft >= 0x200)
					if ( _SearchMarkFromFO( dwLeft, dwLeft+MIN(dwRight-dwLeft, 1024*5), 1, &pcbMark, &dwMarkLen, dwPos, dwIndex ))
					{
						dwArchiveType = mark.GetArchiveType(dwIndex);
						goto lblFound;
					}
			}while (_GetNextInvalidRawDataRange(dwLeft, dwRight));
		}
		return FALSE;
	}

	
	// ......................(0)
	// 一些特殊情况
	#pragma pack(push, 1)
		struct
		{
			long ID;
			long OffsetTableOffset, NotOffsetTableOffset;
		}inno_exe_header;
	#pragma pack(pop)

	if (SUCCEEDED(_file->Seek(0x30, 0, FILE_BEGIN)) &&
		SUCCEEDED(_file->Read(&inno_exe_header, sizeof(inno_exe_header), 0)))
	{
		inno_exe_header.ID = RS_SWAPDWORD(inno_exe_header.ID);
		inno_exe_header.OffsetTableOffset = RS_SWAPDWORD(inno_exe_header.OffsetTableOffset);
		inno_exe_header.NotOffsetTableOffset = RS_SWAPDWORD(inno_exe_header.NotOffsetTableOffset);
		if (inno_exe_header.ID == 0x6F6E6E49 && 
			inno_exe_header.OffsetTableOffset == ~inno_exe_header.NotOffsetTableOffset)
		{
			dwArchiveType = RSENGINE_INNOEXE;
			goto lblFound;
		}
	}


	// ......................(?)
	//1 对于资源111111的需要增加格式
	if (prestbl && prestbl->FindResource(dwPos, dwSize, 11111, 10/*RT_RCDATA*/))
	{
		dwArchiveType = RSENGINE_INNOEXE;
		goto lblFound;
	}


	// ......................(1)
	// search mark from LIB		
	if (CSfxLib::GetInstance().Search(_file, dwArchiveType, dwSFXPos,		//目前这个库只取了部分rar的版本。
							cbMark, dwMarkLen, cbIMark, dwIMarkLen))		//这里成功了， 只表示在那个库中记录匹配了。也就是找到了位置。但是需要对位置进行mark(返回值)的判断。
	{
		// set stream property,  here is PE match.
		{
			PROPVARIANT prop = {VT_EMPTY | VT_BYREF};
			pprop->SetProperty(PID_SFX_MatchLib, &prop);
			pprop->SetProperty(PID_SFX_MatchLibType, &(prop.vt=VT_UI4, prop.ulVal=dwArchiveType, prop));
		}
		
		RASSERT(_GoSectionEnd(dwPos), FALSE);
		dwEnd = dwPos;
		if (S_OK == _file->Seek(dwPos, 0, SPBegin) &&
			S_OK == _file->Read(cbTmp, dwMarkLen, &dwRead) && dwMarkLen == dwRead)
		{
			if (dwMarkLen > 0)  { if (0 == memcmp(cbTmp, cbMark, dwMarkLen)) 	goto lblFound; }
			if (dwIMarkLen > 0) { if (0 == memcmp(cbTmp, cbIMark, MIN(dwMarkLen, dwIMarkLen))) goto lblFound; }
		}

		if (dwSFXPos> 0 && S_OK == _file->Seek(dwSFXPos, 0, SPBegin) &&
			S_OK == _file->Read(cbTmp, dwMarkLen, &dwRead) && dwMarkLen == dwRead)
		{
			dwPos = dwSFXPos;
			if (dwMarkLen >0) { if (0 == memcmp(cbTmp, cbMark, dwMarkLen))	goto lblFound; }
			if (dwIMarkLen >0) { if (0 == memcmp(cbTmp, cbIMark, MIN(dwMarkLen, dwIMarkLen))) goto lblFound; }
		}

		// float search
		if (dwMarkLen != 0 || dwIMarkLen != 0) 
		{
			if (dwIMarkLen > 0)
			{
				if (_SearchMarkFromFO(0, CFileAccessor(_file).GetSize(), 1,(BYTE **)&pcbIMark, &dwIMarkLen, dwPos, dwIndex))	goto lblFound;
			}
			else
			{
				if (_SearchMarkFromFO(0, CFileAccessor(_file).GetSize(), 1, (BYTE **)&pcbMark, &dwMarkLen, dwPos, dwIndex)) goto lblFound;
			}
		}

		// 对于我很明显的根据pe提到了， 这是一个sfx， 那么我又在自己找不到， 可能自己是被脱壳后的，
		// 于是， 需要把（原来的文件， 上层的非pepack对象）重新设置sfx， 并且设置格式，为了上面能很方便的找到。
		if (dwIMarkLen != 0)
		{
			PROPVARIANT prop = {VT_UI1 | VT_VECTOR};
			prop.caub.cElems = dwIMarkLen;
			prop.caub.pElems = pcbIMark;
			pprop->SetProperty(PID_SFX_Signature, &prop);
			return FALSE;
		}
	}


	// ......................(2)
	// find sections by name	
	if (_GetFristMarkSection(dwLeft, dwRight))
	{
		do
		{
			dwEnd = dwRight;
			if (dwRight-dwLeft >= 0x200)
				if ( _SearchMarkFromFO( dwLeft, dwLeft+MIN(dwRight-dwLeft, 1024*5), mark.dwMarkCount, mark.ppMark, mark.pMarkLen, dwPos, dwIndex ))
				{
					dwArchiveType = mark.GetArchiveType(dwIndex);
					goto lblFound;
				}
		}while (_GetNextMarkSection(dwLeft, dwRight));
	}

	// ......................(3)
	// find sections by resource	
	if (prestbl && prestbl->FindResource(dwPos, dwSize, "CABINET", 10/*RT_RCDATA*/))
	{
		dwEnd = dwPos + dwSize;
		if ( _SearchMarkFromFO( dwPos, dwPos+MIN(dwSize, 1024*5), mark.dwMarkCount, mark.ppMark, mark.pMarkLen, dwPos, dwIndex ))
		{
			if (RSENGINE_CABEXE == (dwArchiveType=mark.GetArchiveType(dwIndex)))
				goto lblFound;
		}
	}

	// ......................(4)
	// search all_archive_range  from each_unmap_Sections	
	if (_GetFristInvalidRawDataRange(dwLeft, dwRight))
	{
		do
		{
			dwEnd = dwRight;
			if (dwRight-dwLeft >= 0x200)
				if ( _SearchMarkFromFO( dwLeft, dwLeft+MIN(dwRight-dwLeft, 1024*5), mark.dwMarkCount, mark.ppMark, mark.pMarkLen, dwPos, dwIndex ))
				{
					dwArchiveType = mark.GetArchiveType(dwIndex);
					goto lblFound;
				}
		}while (_GetNextInvalidRawDataRange(dwLeft, dwRight));
	}

	//......................(5)
	// fix pos float searh in mark2
	for (i=0; i<mark2._marks.size(); ++i)
	{
		CMark2::STMark *pmark = mark2._marks[i];
		dwLeft = pmark->fix_pos;
		dwRight = CFileAccessor(_file).GetSize();
		dwEnd = dwRight;
		if (_SearchMarkFromFO(dwLeft,  dwLeft+MIN(dwRight-dwLeft, 1024*5), 1, &pmark->mark, &pmark->mark_size, dwPos, dwIndex))
		{
			dwArchiveType = pmark->sfx_type;
			goto lblFound;
		}
	}

	//......................(6)
	// use last invalidRawDataRange for pointer. // reason 1:  used for InstallShield AFW[OverLay](peid)  cabexe
	DWORD pos;
	if (4==CFileAccessor(_file).ReadPos(&pos, dwLeft, 4))
	{
		dwLeft += (4+RS_SWAPDWORD(pos));
		dwRight =  CFileAccessor(_file).GetSize();
		dwEnd = dwRight;
		if (dwLeft<dwRight && _SearchMarkFromFO( dwLeft, dwLeft+MIN(dwRight-dwLeft, 1024*5), mark.dwMarkCount, mark.ppMark, mark.pMarkLen, dwPos, dwIndex ))
		{
			dwArchiveType = mark.GetArchiveType(dwIndex);
			goto lblFound;
		}
	}





	return FALSE;

lblFound:
	if (pLens)
	{
		if (0 != dwEnd)
			*pLens = dwEnd - dwPos;
		else
			*pLens = CFileAccessor(_file).GetSize() - dwPos;
	}

	switch(dwArchiveType)
	{
	case RSENGINE_RAREXE:
		bCanDel = TRUE;
		break;
	case RSENGINE_ZIPEXE:
		bCanDel = TRUE;
		break;
	case RSENGINE_NSISEXE:
		bCanDel = FALSE;
		break;
	case RSENGINE_CABEXE:
		bCanDel = TRUE;
		break;
	case RSENGINE_LZHEXE:
		bCanDel = TRUE;
		break;
	case RSENGINE_INNOEXE:
		bCanDel = FALSE;
		break;
	case RSENGINE_7ZEXE:
		bCanDel = TRUE;
		break;
	case RSENGINE_ARJEXE:
		bCanDel = TRUE;
		break;
	default:
		ASSERT0;
		break;
	}

	//RASSERT(SUCCEEDED(pprop->SetProperty(PID_SFX_SearchFound, &var)), FALSE); //找到了，以后脱壳进来不找了.
	//改到open成功后去设这个property

	return TRUE;
}




inline
BOOL CSfxType::_SearchMarkFromFO(DWORD dwStart, DWORD dwEnd, 
										DWORD dwMarkCount, BYTE **pMark, DWORD *dwMarkLen,  
										DWORD &dwWhere, DWORD & dwIndex)
{
	const DWORD BUFF_COUNT  = 1024*10;	// 10k buffer
	if (dwStart<0 || dwEnd<=dwStart || dwMarkCount<=0 || NULL==dwMarkLen || *dwMarkLen<=0 || NULL==pMark || NULL==dwMarkLen) return FALSE;
	if (S_OK != _file->Seek(dwStart, 0, SPBegin)) return FALSE;
	
	DWORD dwSearchLen = dwEnd - dwStart;
	DWORD dwRead;
	const BYTE *pcbRet=0;
	DWORD dwMaxMarkLen=0;
	DWORD i;
	
	for (i=0; i<dwMarkCount; ++i)
		if (dwMaxMarkLen < dwMarkLen[i]) dwMaxMarkLen = dwMarkLen[i];

	UTIL::sentry<BYTE *, UTIL::default_array_sentry> pBuf (RSNEW BYTE[BUFF_COUNT + dwMaxMarkLen-1]);
	DWORD dwCount = dwSearchLen / BUFF_COUNT;
	for (i=0; i<dwCount; ++i)
	{
		memmove(pBuf, pBuf+BUFF_COUNT, dwMaxMarkLen-1);
		if (!(S_OK == _file->Read(pBuf+dwMaxMarkLen-1, BUFF_COUNT, &dwRead) && dwRead == BUFF_COUNT)) return FALSE;
		for (DWORD j=0; j<dwMarkCount; ++j)
		{
			if (NULL != pMark[j] && dwMarkLen[j] > 0)
			{
				// StmStm 效率要比std::search高
				pcbRet = StmStm((BYTE *)pBuf, pBuf+BUFF_COUNT+dwMaxMarkLen-1, pMark[j], pMark[j]+dwMarkLen[j]);
				//pcbRet = std::search(pBuf, pBuf+BUFF_COUNT+dwMaxMarkLen-1, pMark[j], pMark[j]+dwMarkLen[j]);
				if (pcbRet != pBuf+BUFF_COUNT+dwMaxMarkLen-1)
				{
					dwWhere = i*BUFF_COUNT + (pcbRet-pBuf) - (dwMaxMarkLen-1) + dwStart;	
					dwIndex = j;
					return TRUE;
				}
			}
		}
	}

	DWORD dwRemain = dwSearchLen % BUFF_COUNT;
	if (dwRemain > 0)
	{
		memmove(pBuf, pBuf+BUFF_COUNT, dwMaxMarkLen-1);
		if (!(S_OK == _file->Read(pBuf+dwMaxMarkLen-1, dwRemain, &dwRead) && dwRead == dwRemain)) return FALSE;
		for (DWORD j=0; j<dwMarkCount; ++j)
		{
			if (NULL != pMark[j] && dwMarkLen[j] >0)
			{
				pcbRet = StmStm((BYTE *)pBuf, pBuf+dwRemain+dwMaxMarkLen-1, pMark[j], pMark[j]+dwMarkLen[j]);
				//pcbRet = std::search(pBuf, pBuf+dwRemain+dwMaxMarkLen-1, pMark[j], pMark[j]+dwMarkLen[j]);
				if (pcbRet != pBuf+dwRemain+dwMaxMarkLen-1)
				{
					dwWhere = i*BUFF_COUNT + (pcbRet-pBuf) - (dwMaxMarkLen-1) + dwStart;
					dwIndex = j;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}



inline
BOOL CSfxType::
_init()
{
	RASSERT(_file && _pefile, RS_FALSE);
	DWORD dwFileLen;
	RASSERT(SUCCEEDED(_file->GetSize(&dwFileLen, 0)) && dwFileLen, FALSE);

	_dwSectionEnd = 0;
	_range.Init(0,dwFileLen);

	IREPERecordTable::IMAGE_SECTION_HEADER  *pSH = NULL;
	IREPERecordTable::IMAGE_OPTIONAL_HEADERS_32 *pOH = NULL;
	UTIL::com_ptr<IREPERecordTable>  pRecTbl(_pefile);
	RASSERT(pRecTbl, FALSE);
	DWORD dwSectionCount = pRecTbl->GetRecordCount( IREPERecordTable::TABLE_SECTION_HEADER );
	for (DWORD i=0; i<dwSectionCount; ++i)
	{
		pSH = (IREPERecordTable::IMAGE_SECTION_HEADER  *) pRecTbl->GetRecord( IREPERecordTable::TABLE_SECTION_HEADER, i );
		if (NULL == pSH) return FALSE;
		_range.AddRange(pSH->PointerToRawData, pSH->PointerToRawData + pSH->SizeOfRawData);

		STSectionInfo si;
		si._dwBeginPos = pSH->PointerToRawData;
		si._dwEndPos = pSH->PointerToRawData + pSH->SizeOfRawData;
		//si._Name = (char *)pSH->Name;
		si._Name.assign((char *)pSH->Name, sizeof(pSH->Name));
		_Sections.insert(_Sections.end(), si);

		if (_dwSectionEnd < pSH->PointerToRawData + pSH->SizeOfRawData) _dwSectionEnd = pSH->PointerToRawData + pSH->SizeOfRawData;
	}
	pOH = (IREPERecordTable::IMAGE_OPTIONAL_HEADERS_32  *) pRecTbl->GetRecord( IREPERecordTable::TABLE_OPTIONAL_HEADER, 0 );
	if (NULL == pOH) return FALSE;
	_range.AddRange(0, pOH->SizeOfHeaders);

	return TRUE;
}

inline
BOOL CSfxType::
_InitSuccess()
{
	return _bInitSuccess;
}

inline
BOOL CSfxType::
_GetFristInvalidRawDataRange(DWORD &dwLeft, DWORD &dwRight)
{
	return _range.GetFristInvalidRange(dwLeft, dwRight);
}

inline
BOOL CSfxType::
_GetNextInvalidRawDataRange(DWORD &dwLeft, DWORD &dwRight)
{
	return _range.GetNextInvalidRange(dwLeft, dwRight);
}

inline
BOOL CSfxType::
_GetFristMarkSection(DWORD &dwLeft, DWORD &dwRight)
{
	typedef std::vector<std::string>::iterator CITString;
	typedef std::vector<STSectionInfo>::iterator CITInfo;


	CITString itBegin = CMarkSectionName::GetInstance()->GetNames()->begin();
	CITString itEnd  = CMarkSectionName::GetInstance()->GetNames()->end();

	for (CITInfo itinfo = _Sections.begin(); itinfo != _Sections.end(); ++itinfo)
	{
		CITString it  = std::find(itBegin, itEnd, itinfo->_Name);
		if (it !=itEnd)
		{
			dwLeft = itinfo->_dwBeginPos;
			dwRight = itinfo->_dwEndPos;
			_itCurSections = ++itinfo;
			return TRUE;
		}
	}
	return FALSE;
}

inline
BOOL CSfxType::
_GetNextMarkSection(DWORD &dwLeft, DWORD &dwRight)
{
	typedef std::vector<std::string>::iterator CITString;
	typedef std::vector<STSectionInfo>::iterator CITInfo;

	CITString itBegin = CMarkSectionName::GetInstance()->GetNames()->begin();
	CITString itEnd  = CMarkSectionName::GetInstance()->GetNames()->end();

	for (CITInfo itinfo = _itCurSections; itinfo != _Sections.end(); ++itinfo)
	{
		CITString it  = std::find(itBegin, itEnd, itinfo->_Name);
		if (it !=itEnd)
		{
			dwLeft = itinfo->_dwBeginPos;
			dwRight = itinfo->_dwEndPos;
			_itCurSections = ++itinfo;
			return TRUE;
		}
	}
	return FALSE;
}

inline
BOOL CSfxType::
_GoSectionEnd(DWORD &pos)
{
	pos = _dwSectionEnd;
	return TRUE;
}

inline 
void CSfxType::
_GetOrgStream(IREStream **ppstm)
{
	UTIL::com_ptr<IREStream> pstm(_file);
	UTIL::com_ptr<IREArchive> parcParent;

	for( ; ; )
	{
		UTIL::com_ptr<IREProperty> pprop(pstm);

		if( FAILED( CFileAccessor(pstm).GetParent(&(parcParent=(IUnknown*)0).m_p)) ||
			REFFR::FILE_PEPACKER != CFileAccessor::GetCurrentType(parcParent) )
		{
			break;
		}
		parcParent->GetStreamObject(&((pstm=(IUnknown*)0).m_p));
	}
	*ppstm = pstm.detach();
}


#endif //__SFXTYPE_H__

