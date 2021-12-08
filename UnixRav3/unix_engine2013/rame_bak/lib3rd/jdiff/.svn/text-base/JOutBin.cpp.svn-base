/*
 * JOutBin.cpp
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

#include "JOutBin.h"

namespace JojoDiff {

JOutBin::JOutBin( JFileBOut& rob ) : rOutB(rob), miOprCur(ESC), mzEqlCnt(0), mbOutEsc(false) {
}

JOutBin::~JOutBin() {
}

/*******************************************************************************
* Output functions
*
* The output has following format
*   <esc> <opcode> [<length>|<data>]
* where
*   <esc>    =   ESC
*   <opcode> =   MOD | INS | DEL | EQL | BKT
*   <data>   :   A series of data bytes.
*        The series is ended with a new "<esc> <opcode>" sequence.
*        If an "<esc> <opcode>" sequence occurs within the data, it is
*        prefixed with an additional <esc>.
*        E.g.: <ESC><MOD>This data contains an <ESC><ESC><EQL> sequence.
*   <length> :   1 to 5 bytes for specifying a 32-bit unsigned number.
*              1 <= x < 252        1 byte:   0-251
*            252 <= x < 508        2 bytes:  252, x-252
*            508 <= x < 0x10000    3 bytes:  253, xx
*        0x10000 <= x < 0x100000000        5 bytes:  254, xxxx
*                          9 bytes:  255, xxxxxxxx
*
*******************************************************************************/

/* ---------------------------------------------------------------
 * ufPutLen outputs a length as follows
 * byte1  following      formula              if number is
 * -----  ---------      -------------------- --------------------
 * 0-251                 1-252                between 1 and 252
 * 252    x              253 + x              between 253 and 508
 * 253    xx             253 + 256 + xx       a 16-bit number
 * 254    xxxx           253 + 256 + xxxx     a 32-bit number
 * 255    xxxxxxxx       253 + 256 + xxxxxxxx a 64-bit number
 * ---------------------------------------------------------------*/
void JOutBin::ufPutLen ( offset_t azLen  )
{ if (azLen <= 252) {
    putch(azLen - 1) ;
    gzOutBytCtl += 1;
  } else if (azLen <= 508) {
    putch(252);
    putch((azLen - 253)) ;
    gzOutBytCtl += 2;
  } else if (azLen <= 0xffff) {
    putch(253);
    putch((azLen >>  8)       ) ;
    putch((azLen      ) & 0xff) ;
    gzOutBytCtl += 3;
#ifdef JDIFF_LARGEFILE
  } else if (azLen <= 0xffffffff) {
#else
  } else {
#endif
    putch(254);
    putch((azLen >> 24)       );
    putch((azLen >> 16) & 0xff) ;
    putch((azLen >>  8) & 0xff) ;
    putch((azLen      ) & 0xff) ;
    gzOutBytCtl += 5;
  }
#ifdef JDIFF_LARGEFILE
  else {
    putch(255);
    putch((azLen >> 56)       ) ;
    putch((azLen >> 48) & 0xff) ;
    putch((azLen >> 40) & 0xff) ;
    putch((azLen >> 32) & 0xff) ;
    putch((azLen >> 24) & 0xff) ;
    putch((azLen >> 16) & 0xff) ;
    putch((azLen >>  8) & 0xff) ;
    putch((azLen      ) & 0xff) ;
    gzOutBytCtl += 9;
  }
#endif
} /* ufPutLen */

/* ---------------------------------------------------------------
 * ufPutOpr outputs a new opcode and closes the previous
 * data stream.
 * ---------------------------------------------------------------*/
void JOutBin::ufPutOpr ( int aiOpr )
{ if (mbOutEsc) {
    putch(ESC) ;
    putch(ESC) ;
    mbOutEsc = false ;
    gzOutBytEsc++ ;
    gzOutBytDta++ ;
  }

  if ( aiOpr != ESC ) {
    putch(ESC);
    putch(aiOpr);
    gzOutBytCtl+=2;
  }
}

/* ---------------------------------------------------------------
 * ufPutByt outputs a byte, prefixing a data sequence <esc> <opcode>
 * with an addition <esc> byte.
 * ---------------------------------------------------------------*/
void JOutBin::ufPutByt ( int aiByt )
{
  if (mbOutEsc) {
    mbOutEsc = false;
    if (aiByt >= BKT && aiByt <= ESC) {
      /* output an additional <esc> byte */
      putch(ESC) ;
      gzOutBytEsc++ ;
    }
    putch(ESC) ;
    gzOutBytDta++;
  }
  if (aiByt == ESC) {
    mbOutEsc = true ;
  } else {
    putch(aiByt) ;
    gzOutBytDta++;
  }
}

bool JOutBin::putch( int uch )
{
	return rOutB.putch( uch );
}

/* ---------------------------------------------------------------
 * ufOutBytBin: binary output function for generating patch files
 * ---------------------------------------------------------------*/
bool JOutBin::put (
  int   aiOpr,
  offset_t azLen,
  int   aiOrg,
  int   aiNew,
  offset_t azPosOrg,
  offset_t azPosNew
)
{ /* Output a pending EQL operand (if more than 4 equal bytes) */
  if (aiOpr != EQL && mzEqlCnt > 0) {
    if (mzEqlCnt > 4 || (miOprCur != MOD && aiOpr != MOD)) {
      // more than 4 equal bytes => output as EQL
      miOprCur = EQL;
      ufPutOpr(EQL) ;
      ufPutLen(mzEqlCnt);

      gzOutBytEql+=mzEqlCnt;
    } else {
      // less than 4 equal bytes => output as MOD
      if (miOprCur != MOD) {
        miOprCur = MOD ;
        ufPutOpr(MOD) ;
      }
      for (int liCnt=0; liCnt < mzEqlCnt; liCnt++)
        ufPutByt(miEqlBuf[liCnt]) ;
    }
    mzEqlCnt=0;
  }

  /* Handle current operand */
  switch (aiOpr) {
    case ESC : /* before closing the output */
      ufPutOpr(ESC);
      miOprCur = ESC ;
      break;

    case MOD :
    case INS :
      if (miOprCur != aiOpr) {
        miOprCur = aiOpr ;
        ufPutOpr(aiOpr) ;
      }
      ufPutByt(aiNew) ;
      break;

    case DEL :
      ufPutOpr(DEL) ;
      ufPutLen(azLen);

      miOprCur=DEL;
      gzOutBytDel+=azLen;
      break;

    case BKT :
      ufPutOpr(BKT) ;
      ufPutLen(azLen);

      miOprCur=BKT;
      gzOutBytBkt+=azLen;
      break;

    case EQL :
      if (mzEqlCnt < 4) {
          miEqlBuf[mzEqlCnt++] = aiOrg ;
          return (mzEqlCnt >= 4) ;
      } else {
          mzEqlCnt+=azLen ;
          return true ;
      }
      break;
  }

  return false ;
}
}
