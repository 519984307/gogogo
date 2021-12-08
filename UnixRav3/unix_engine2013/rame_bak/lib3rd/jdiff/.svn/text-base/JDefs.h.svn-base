/*
 * JDefs.h
 *
 * JojoDiff global definitions
 *
 * Copyright (C) 2002-2011 Joris Heirbaut
 *
 * This file is part of JojoDiff.
 *
 * JojoDiff is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _JDEFS_H
#define _JDEFS_H

#include <stdio.h>
#include "JExport.h"

namespace JojoDiff
{

#ifdef _DEBUG
#define debug           0       /* Include debug code? */
#else
#define debug           0       /* Include debug code? */
#endif

/*
 * Default definitions (for GCC/Linux)
 */
//
//#ifdef JDIFF_FILE64
//
//	#define JDIFF_LARGEFILE
//	#define offset_t long long
//
//	#ifdef _WIN32
//		#define offset_t __int64
//		#define PRIzd "I64d"
//	#else
//		#define offset_t long long
//		#define PRIzd "lld"
//	#endif
//
//	//#define fopen   fopen64
//	//#define fclose  fclose
//	//#define fseek   fseeko64
//	//#define ftell   ftello64
//
//#else
//
//	#define offset_t	long
//	#define PRIzd		"ld"
//
//#endif

#ifdef JDIFF_LARGEFILE
	#if debug
		#define P8zd    "%10" PRIzd
	#else
		#define P8zd    "%12" PRIzd
	#endif
#else
	#define P8zd    "%8" PRIzd
#endif

/*
 * Global definitions
 */
#define JDIFF_VERSION   "0.8.1 (beta) December 2011"
#define JDIFF_COPYRIGHT "Copyright (C) 2002-2005,2009,2011 Joris Heirbaut"

#define uchar unsigned char
#define ulong unsigned long int         // unsigned long
#define null  NULL

#ifdef _LARGESAMPLE
typedef unsigned long long int  hkey ;  // 64-bit hash keys
#define PRIhkey "llx"                   // format to print a hkey
#else
typedef unsigned long int hkey ;        // 32-bit hash keys
#define PRIhkey "lx"                    // format to print a hkey
#endif /* _LARGESAMPLE */
const int SMPSZE = (int) sizeof(hkey) * 8 ;                                         // Number of bytes in a sample
const offset_t MAX_OFF_T = (((offset_t)-1) ^ (((offset_t) 1) << (sizeof(offset_t) * 8 - 1))) ;  // Largest positive offset



};

#endif /* _JDEFS_H */
