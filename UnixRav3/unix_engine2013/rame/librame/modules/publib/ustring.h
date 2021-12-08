#ifndef USTRING_H_
#define USTRING_H_

class ustring
{
	enum type_t { kAny, kAnsi, kUnicode };
	struct _buffer
	{
		BYTE * _base;
		size_t _size;
		type_t _type;
		_buffer() : _base(NULL), _size(0), _type(kAny) { }
		~_buffer() { _free(); }
		void _free() { delete [] _base; _base = NULL; _size = 0; }
		void * _malloc(size_t cb) { return (_base = RSNEW BYTE [(_size = cb)]); }
	};
	struct _duplicate : _buffer
	{
		explicit _duplicate(const char * s) 
		{ 
			ASSERT(s);
			size_t cch = strlen(s), cb = (cch + 1) * sizeof(char);
			if (_malloc(cb)) { memcpy(_base, s, cb); }
			_type = kAnsi;
		}
		explicit _duplicate(const wchar_t * s) 
		{
			ASSERT(s);
			size_t cch = wcslen(s), cb = (cch + 1) * sizeof(wchar_t);
			if (_malloc(cb)) { memcpy(_base, s, cb); }
			_type = kUnicode;
		}
		explicit _duplicate(const _buffer & s)
		{
			ASSERT(s._base);
			size_t cb = s._size;
			if (_malloc(cb)) { memcpy(_base, s._base, cb); }
			_type = s._type;
		}
	};
	struct _convert : _buffer
	{
		explicit _convert(const char * s)
		{
			size_t cch = mbstowcs(NULL, s, 0);
			if (size_t(-1) != cch)
			{
				size_t cb = (cch + 1) * sizeof(wchar_t);
				if (_malloc(cb)) { mbstowcs((wchar_t *)_base, s, cch+1); }
			}
			_type = kUnicode;
		}
		explicit _convert(const wchar_t * s)
		{
			size_t cch = wcstombs(NULL, s, 0);
			if (size_t(-1) != cch)
			{
				size_t cb = (cch + 1) * sizeof(wchar_t);
				if (_malloc(cb)) { wcstombs((char *)_base, s, cb); }
			}
			_type = kAnsi;
		}
	};
protected:
	_buffer * m_data;
public:
	ustring() 
		: m_data(NULL) { }
	ustring(const char * s) 
		: m_data(s ? RSNEW _duplicate(s) : NULL) { } 
	ustring(const wchar_t * s) 
		: m_data(s ? RSNEW _duplicate(s) : NULL) { }
	ustring(const ustring & s)
		: m_data(s.m_data ? RSNEW _duplicate(*s.m_data) : NULL) { }
	~ustring() 
	{ delete m_data; }
	ustring & operator = (const char * s) 
	{ delete m_data; m_data = s ? RSNEW _duplicate(s) : NULL; return * this; }
	ustring & operator = (const wchar_t * s)
	{ delete m_data; m_data = s ? RSNEW _duplicate(s) : NULL; return * this; }
	ustring & operator = (const ustring & s)
	{ delete m_data; m_data = s.m_data ? RSNEW _duplicate(*s.m_data) : NULL; return * this; }
	operator const char * ()
	{
		if (m_data) 
		{
			if (kUnicode == m_data->_type) 
			{ 
				_buffer * newdata = RSNEW _convert((wchar_t *)m_data->_base);
				delete m_data; m_data = newdata;
			}
		}
		if (m_data) { return (char *)m_data->_base; }
		return NULL;
	}
	operator char * () { return (char *)(const char *)(*this); }
	operator const wchar_t * ()
	{
		if (m_data)
		{
			if (kUnicode != m_data->_type)
			{
				_buffer * newdata = RSNEW _convert((char *)m_data->_base);
				delete m_data; m_data = newdata;
			}
		}
		if (m_data) { return (wchar_t *)m_data->_base; }
		return NULL;
	}
	operator wchar_t * () { return (wchar_t *)(const wchar_t *)(*this); }
public:
	bool operator ! () { return !(m_data && m_data->_base); }
	bool is_unicode() { return (m_data && (kUnicode == m_data->_type)); }
	bool is_ansi() { return (m_data && (kAnsi == m_data->_type)); }
	//
	bool empty() { return !(*this); }
	void clear() { delete m_data; m_data = NULL; }
};


#endif
