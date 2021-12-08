#ifndef _PUBERROR_H_
#define _PUBERROR_H_

//
// Success codes
//
#define S_OK								((HRESULT)0x00000000L)
#define S_FALSE								((HRESULT)0x00000001L)


#define ERROR_FILE_NOT_FOUND             	2L
#define ERROR_PATH_NOT_FOUND             	3L
#define ERROR_ACCESS_DENIED              	5L
#define ERROR_INVALID_HANDLE             	6L
#define ERROR_BAD_FORMAT                 	11L
#define ERROR_SEEK                       	25L
#define ERROR_ALREADY_EXISTS             	183L
#define ERROR_NOACCESS                   	998L
#define ERROR_INVALID_USER_BUFFER        	1784L
#define ERROR_NOT_FOUND					 	1168L

#define _HRESULT_TYPEDEF_(_sc) ((HRESULT)_sc)

#define E_UNEXPECTED                     _HRESULT_TYPEDEF_(0x8000FFFFL)
#define E_NOTIMPL                        _HRESULT_TYPEDEF_(0x80004001L)
#define E_OUTOFMEMORY                    _HRESULT_TYPEDEF_(0x8007000EL)
#define E_INVALIDARG                     _HRESULT_TYPEDEF_(0x80070057L)
#define E_NOINTERFACE                    _HRESULT_TYPEDEF_(0x80004002L)
#define E_POINTER                        _HRESULT_TYPEDEF_(0x80004003L)
#define E_HANDLE                         _HRESULT_TYPEDEF_(0x80070006L)
#define E_ABORT                          _HRESULT_TYPEDEF_(0x80004004L)
#define E_FAIL                           _HRESULT_TYPEDEF_(0x80004005L)
#define E_ACCESSDENIED                   _HRESULT_TYPEDEF_(0x80070005L)

#define ENUM_E_LAST         0x800401BFL


#define STG_E_UNIMPLEMENTEDFUNCTION      _HRESULT_TYPEDEF_(0x800300FEL)

#define MK_S_MONIKERALREADYREGISTERED    _HRESULT_TYPEDEF_(0x000401E7L)

//
// Generic test for success on any status value (non-negative numbers
// indicate success).
//
#ifndef SUCCEEDED
#define SUCCEEDED(hr) ((HRESULT)(hr) >= 0)
#endif
#ifndef FAILED
#define FAILED(hr) ((HRESULT)(hr) < 0)
#endif

//
// Map a WIN32 error value into a HRESULT
// Note: This assumes that WIN32 errors fall in the range -32k to 32k.
//
// Define bits here so macros are guaranteed to work

#define FACILITY_WIN32                   7

// #if (defined(_MSC_VER) && (_MSC_VER > 1300))
#if (TARGET_OS != OS_WINDOWS )

	#define __HRESULT_FROM_WIN32(x) ((HRESULT)(x) <= 0 ? ((HRESULT)(x)) : ((HRESULT) (((x) & 0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000)))

	#define HRESULT_FROM_WIN32(x) __HRESULT_FROM_WIN32(x)

#endif

#if (TARGET_OS==OS_WINDOWS)
#	define HRESULT_FROM_SYSCALL(x) (x) ? S_OK : HRESULT_FROM_WIN32(::GetLastError())
#endif


#endif // _PUBERROR_H_
