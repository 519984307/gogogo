#ifndef _RS_STDIO_H_
#define _RS_STDIO_H_

#ifdef __cplusplus
extern "C" { ;
#endif

//
#include <stdio.h>

// stream ================================================================
#ifndef PLATFORM_TYPE_NATIVE
//
#define rs_fopen	fopen
#define rs_fclose	fclose
#define rs_fread	fread
#define rs_fwrite	fwrite
#define rs_fflush	fflush
#define rs_fgets	fgets
#define rs_ftell	ftell
#define rs_fseek	fseek
#define rs_feof		feof
#define rs_ferror	ferror
#define rs_rewind	rewind

#else
//
#include "../public/rsnative.h"
//
#define rs_fopen	bs_fopen
#define rs_fclose	bs_fclose
#define rs_fread	bs_fread
#define rs_fwrite	bs_fwrite
#define rs_fflush	bs_fflush
#define rs_fgets	bs_fgets
#define rs_ftell	bs_ftell
#define rs_fseek	bs_fseek
#define rs_feof		bs_feof
#define rs_ferror	bs_ferror
#define rs_rewind	bs_rewind

#endif


// varg ================================================================
#if defined(PLATFORM_TYPE_WIN32)
//
#define rs_vsnprintf	_vsnprintf
#define rs_snprintf		_snprintf

#elif defined(PLATFORM_TYPE_NATIVE)
//

#else
//
#define _vsnprintf 		vsnprintf
#define _snprintf 		snprintf

#endif



#ifdef __cplusplus
}
#endif

#endif 
