/*
 * JOut.h
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

#ifndef JOUT_H_
#define JOUT_H_
#include <stdio.h>
#include "JDefs.h"
#include "JFile.h"

namespace JojoDiff {

/**
 * Abstract JDiff output class.
 */
class JOut
{
	public:
		/**
		 * Abstract output routine for JDiff, called to output one byte.
		 *
		 * @param aiOpr     operand: ESC, INS, DEL, EQL, BKT or MOD
		 * @param azLen     length of operand for DEL and BKT
		 * @param aiOrg     character from original file
		 * @param aiNew     character from new file
		 * @param azPosOrg  position within original file
		 * @param azPosNew  position within new file
		 * @return  false = continue sending byte by byte, true = permission to send length (faster)
		 */
		virtual bool put(int aiOpr, offset_t azLen, int aiOrg, int aiNew,
			offset_t azPosOrg, offset_t azPosNew) = 0;

		/*
		 * Statistics about operations
		 */
		offset_t gzOutBytDta; /* Number of data    bytes written                   */
		offset_t gzOutBytCtl; /* Number of control bytes written (overhead)        */
		offset_t gzOutBytDel; /* Number of data    bytes deleted                   */
		offset_t gzOutBytBkt; /* Number of data    bytes backtracked               */
		offset_t gzOutBytEsc; /* Number of escape  bytes written (overhead)        */
		offset_t gzOutBytEql; /* Number of data    bytes not written (gain)        */

	protected:
		JOut() :
			gzOutBytDta(0), gzOutBytCtl(0), gzOutBytDel(0), gzOutBytBkt(0),
			gzOutBytEsc(0), gzOutBytEql(0)
		{
		}
		;
	}; // class JOut

}

#endif /* JOUT_H_ */
