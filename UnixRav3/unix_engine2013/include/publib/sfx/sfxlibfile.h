#ifndef __SFXLIBFILE_H__
#define __SFXLIBFILE_H__

class CSfxLibFile : public IREStream
{
	CSfxLibFile(const CSfxLibFile &);
	CSfxLibFile & operator = (const CSfxLibFile &);
public:
	// constructor
	CSfxLibFile() :_file_len(sizeof(cbLibFile)){ _pos = 0; }

	// destructor
	virtual ~CSfxLibFile() {}

	STDMETHOD(QueryInterface)(REFGUID , void **) { return E_UNEXPECTED; }
	STDMETHOD_(ULONG, AddRef)() { return 2; }
	STDMETHOD_(ULONG, Release)() { return 1; }
	STDMETHOD(Read)(LPVOID lpBuffer,  DWORD cbBytesToRead, OUT LPDWORD lpBytesRead OPTIONAL)
	{
		return __Read(lpBuffer, cbBytesToRead, *lpBytesRead);
	}
	STDMETHOD(Write)(LPCVOID , DWORD , OUT LPDWORD  OPTIONAL)
	{
		return E_UNEXPECTED;
	}
	STDMETHOD(Flush)()
	{
		return E_UNEXPECTED;
	}
	STDMETHOD(Seek)(LONG lDistanceToMove, PLONG  OPTIONAL, DWORD dwMoveMethod)
	{
		return __Seek(lDistanceToMove, dwMoveMethod) ;
	}
	STDMETHOD(Tell)(OUT LPDWORD lpOffsetLow, OUT LPDWORD  OPTIONAL)
	{
		return __Tell(*lpOffsetLow);
	}
	STDMETHOD(GetSize)(OUT LPDWORD lpSizeLow, OUT LPDWORD  OPTIONAL)
	{
		return __GetSize(*lpSizeLow);
	}
	STDMETHOD(SetSize)(DWORD , LPDWORD  OPTIONAL)
	{
		return E_UNEXPECTED;
	}
	STDMETHOD_(DWORD, GetStatus)()
	{
		return 0;
	}

private:
	// interface from IRSFileObject
	HRESULT __Read(void *pBuf, DWORD dwBufSize, DWORD &dwRead) 
	{ 
		if (_pos < 0 || _pos >= _file_len) return E_UNEXPECTED;
		dwRead = _pos+dwBufSize>_file_len ? _file_len-_pos : dwBufSize;
		memcpy(pBuf, cbLibFile+_pos, dwRead);
		_pos += dwRead;
		return S_OK; 
	}
	HRESULT __GetSize(DWORD &fileSize) 
	{
		fileSize =  _file_len;
		return S_OK; 
	}
	
	// 其实Seek都没用上， 只要Read和GetSize就可以
	HRESULT __Seek(LONG lOff, UINT nFrom = SPBegin) 
	{ 
		if (nFrom == SPBegin)
			_pos = lOff;
		else if (nFrom == SPCurrent)
			_pos += lOff;
		else
			_pos = _file_len+lOff;

		if (_pos < 0) _pos=0;
		return S_OK; 
	}

	HRESULT __Tell(DWORD &dwOff) 
	{ 
		dwOff = _pos;
		return S_OK; 
	}

private:
	DWORD _pos;
	const DWORD _file_len;
	static unsigned char cbLibFile[9397];
};


#endif //__SFXLIBFILE_H__
