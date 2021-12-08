/*
LzmaDecode.c
LZMA Decoder (optimized for Speed version)

LZMA SDK 4.22 Copyright (c) 1999-2005 Igor Pavlov (2005-06-10)
http://www.7-zip.org/

LZMA SDK is licensed under two licenses:
1) GNU Lesser General Public License (GNU LGPL)
2) Common Public License (CPL)
It means that you can select one of these two licenses and 
follow rules of that license.

SPECIAL EXCEPTION:
Igor Pavlov, as the author of this Code, expressly permits you to 
statically or dynamically link your Code (or bind by name) to the 
interfaces of this file without subjecting your linked Code to the 
terms of the CPL or GNU LGPL. Any modifications or additions 
to this file, however, are subject to the LGPL or CPL terms.
*/

#include "../algorithms.h"
#include "stdlib.h"


#ifndef Byte
#define Byte unsigned char
#endif

#ifndef UInt32
#define UInt32 UINT32
#endif

#ifndef UInt16
#define UInt16 unsigned short
#endif

#define kNumTopBits 24
#define kTopValue ((UInt32)1 << kNumTopBits)							//0x01000000

#define kNumBitModelTotalBits 11
#define kBitModelTotal (1 << kNumBitModelTotalBits)						//0x00000800
#define kNumMoveBits 5

#define RC_READ_BYTE (*Buffer++)

#define RC_INIT2 Code = 0; Range = 0xFFFFFFFF; \
  { int i; for(i = 0; i < 5; ++i) { RC_TEST; Code = (Code << 8) | RC_READ_BYTE; }}

#define RC_TEST { if (Buffer == BufferLim) throw 1; }

#define RC_INIT(buffer, bufferSize) Buffer = buffer; BufferLim = buffer + bufferSize; RC_INIT2


#define RC_NORMALIZE if (Range < kTopValue) { RC_TEST; Range <<= 8; Code = (Code << 8) | RC_READ_BYTE; }

#define IfBit0(p) RC_NORMALIZE; bound = (Range >> kNumBitModelTotalBits) * *(p); if (Code < bound)
#define UpdateBit0(p) Range = bound; *(p) += (kBitModelTotal - *(p)) >> kNumMoveBits;
#define UpdateBit1(p) Range -= bound; Code -= bound; *(p) -= (*(p)) >> kNumMoveBits;

#define RC_GET_BIT2(p, mi, A0, A1) IfBit0(p) \
  { UpdateBit0(p); mi <<= 1; A0; } else \
  { UpdateBit1(p); mi = (mi + mi) + 1; A1; } 

#define RC_GET_BIT(p, mi) RC_GET_BIT2(p, mi, ; , ;)               

#define RangeDecoderBitTreeDecode(probs, numLevels, res) \
  { int i = numLevels; res = 1; \
  do { CProb *p = probs + res; RC_GET_BIT(p, res) } while(--i != 0); \
  res -= (1 << numLevels); }


#define kNumPosBitsMax 4
#define kNumPosStatesMax (1 << kNumPosBitsMax)							//0x00000010

#define kLenNumLowBits 3
#define kLenNumLowSymbols (1 << kLenNumLowBits)							//0x00000008
#define kLenNumMidBits 3
#define kLenNumMidSymbols (1 << kLenNumMidBits)							//0x00000008
#define kLenNumHighBits 8
#define kLenNumHighSymbols (1 << kLenNumHighBits)						//0x00000100

#define LenChoice 0								
#define LenChoice2 (LenChoice + 1)										//0x00000001
#define LenLow (LenChoice2 + 1)											//0x00000002
#define LenMid (LenLow + (kNumPosStatesMax << kLenNumLowBits))			//0x00000082
#define LenHigh (LenMid + (kNumPosStatesMax << kLenNumMidBits))			//0x00000102
#define kNumLenProbs (LenHigh + kLenNumHighSymbols)						//0x00000202


#define kNumStates 12
#define kNumLitStates 7

#define kStartPosModelIndex 4
#define kEndPosModelIndex 14
#define kNumFullDistances (1 << (kEndPosModelIndex >> 1))				//0x00000080

#define kNumPosSlotBits 6
#define kNumLenToPosStates 4

#define kNumAlignBits 4
#define kAlignTableSize (1 << kNumAlignBits)							//0x00000010

#define kMatchMinLen 2

#define IsMatch 0
#define IsRep (IsMatch + (kNumStates << kNumPosBitsMax))				//0x000000c0
#define IsRepG0 (IsRep + kNumStates)									//0x000000cc
#define IsRepG1 (IsRepG0 + kNumStates)									//0x000000d8			
#define IsRepG2 (IsRepG1 + kNumStates)									//0x000000e4
#define IsRep0Long (IsRepG2 + kNumStates)								//0x000000f0
#define PosSlot (IsRep0Long + (kNumStates << kNumPosBitsMax))			//0x000001b0
#define SpecPos (PosSlot + (kNumLenToPosStates << kNumPosSlotBits))		//0x000002b0
#define Align (SpecPos + kNumFullDistances - kEndPosModelIndex)			//0x00000322
#define LenCoder (Align + kAlignTableSize)								//0x00000332
#define RepLenCoder (LenCoder + kNumLenProbs)							//0x00000534
#define Literal (RepLenCoder + kNumLenProbs)							//0x00000736


#define kLzmaStreamWasFinishedId (-1)

#define LZMA_RESULT_OK 0
#define LZMA_RESULT_DATA_ERROR 1


#define LZMA_BASE_SIZE 1846												//0x736
#define LZMA_LIT_SIZE 768												//0x300

#define LzmaGetNumProbs(Properties) (LZMA_BASE_SIZE + (LZMA_LIT_SIZE << ((Properties)->lc + (Properties)->lp)))

#ifdef _LZMA_PROB32
#define CProb UINT32
#else
#define CProb UINT16
#endif


int LzmaDecodeProperties(CLzmaProperties *props,  unsigned char prop)
{
	if (prop >= (9 * 5 * 5)) return LZMA_RESULT_DATA_ERROR;

    for (props->pb = 0; prop >= (9 * 5); props->pb++, prop -= (9 * 5));
    for (props->lp = 0; prop >= 9; props->lp++, prop -= 9);
    props->lc = prop;
	
	return LZMA_RESULT_OK;
}

inline int inline_LzmaDecode
	(CLzmaProperties *probs, CProb *prob,
	const unsigned char *inStream, SizeT inSize, SizeT *inSizeProcessed,
	unsigned char *outStream, SizeT outSize, SizeT *outSizeProcessed)
{
	const Byte *Buffer;
	const Byte *BufferLim;
	SizeT nowPos = 0;

	try{
	CProb *p = prob;
	
	Byte previousByte = 0;
	UInt32 posStateMask = (1 << (probs->pb)) - 1;
	UInt32 literalPosMask = (1 << (probs->lp)) - 1;
	int lc = probs->lc;


	int state = 0;
	UInt32 rep0 = 1, rep1 = 1, rep2 = 1, rep3 = 1;
	int len = 0;

	UInt32 Range;
	UInt32 Code;

	if (inSizeProcessed) *inSizeProcessed = 0;
	if (outSizeProcessed) *outSizeProcessed = 0;

	{
		UInt32 i;
		UInt32 numProbs;
		numProbs = Literal + ((UInt32)LZMA_LIT_SIZE << (lc + probs->lp));
		for (i = 0; i < numProbs; i++)
			p[i] = kBitModelTotal >> 1;
	}


	RC_INIT(inStream, inSize);


	while(nowPos < outSize)
	{
		CProb *prob;
		UInt32 bound;
		int posState = (int)(nowPos & posStateMask);

		prob = p + IsMatch + (state << kNumPosBitsMax) + posState;
		IfBit0(prob)
		{
			int symbol = 1;
			UpdateBit0(prob)
				prob = p + Literal + (LZMA_LIT_SIZE * 
				((( nowPos & literalPosMask) << lc) + (previousByte >> (8 - lc))));

			if (state >= kNumLitStates)
			{
				int matchByte;
				matchByte = outStream[nowPos - rep0];
				do
				{
					int bit;
					CProb *probLit;
					matchByte <<= 1;
					bit = (matchByte & 0x100);
					probLit = prob + 0x100 + bit + symbol;
					RC_GET_BIT2(probLit, symbol, if (bit != 0) break, if (bit == 0) break)
				}
				while (symbol < 0x100);
			}
			while (symbol < 0x100)
			{
				CProb *probLit = prob + symbol;
				RC_GET_BIT(probLit, symbol)
			}
			previousByte = (Byte)symbol;

			outStream[nowPos++] = previousByte;
			if (state < 4) state = 0;
			else if (state < 10) state -= 3;
			else state -= 6;
		}
	else             
	{
		UpdateBit1(prob);
		prob = p + IsRep + state;
		IfBit0(prob)
		{
			UpdateBit0(prob);
			rep3 = rep2;
			rep2 = rep1;
			rep1 = rep0;
			state = state < kNumLitStates ? 0 : 3;
			prob = p + LenCoder;
		}
	else
	{
		UpdateBit1(prob);
		prob = p + IsRepG0 + state;
		IfBit0(prob)
		{
			UpdateBit0(prob);
			prob = p + IsRep0Long + (state << kNumPosBitsMax) + posState;
			IfBit0(prob)
			{
				UpdateBit0(prob);

				if (nowPos == 0)
					//return LZMA_RESULT_DATA_ERROR;
					throw 1;

				state = state < kNumLitStates ? 9 : 11;
				previousByte = outStream[nowPos - rep0];
				outStream[nowPos++] = previousByte;

				continue;
			}
else
{
	UpdateBit1(prob);
}
		}
else
{
	UInt32 distance;
	UpdateBit1(prob);
	prob = p + IsRepG1 + state;
	IfBit0(prob)
	{
		UpdateBit0(prob);
		distance = rep1;
	}
		  else 
		  {
			  UpdateBit1(prob);
			  prob = p + IsRepG2 + state;
			  IfBit0(prob)
			  {
				  UpdateBit0(prob);
				  distance = rep2;
			  }
			else
			{
				UpdateBit1(prob);
				distance = rep3;
				rep3 = rep2;
			}
			rep2 = rep1;
		  }
		  rep1 = rep0;
		  rep0 = distance;
}
state = state < kNumLitStates ? 8 : 11;
prob = p + RepLenCoder;
	}
	{
		int numBits, offset;
		CProb *probLen = prob + LenChoice;
		IfBit0(probLen)
		{
			UpdateBit0(probLen);
			probLen = prob + LenLow + (posState << kLenNumLowBits);
			offset = 0;
			numBits = kLenNumLowBits;
		}
else
{
	UpdateBit1(probLen);
	probLen = prob + LenChoice2;
	IfBit0(probLen)
	{
		UpdateBit0(probLen);
		probLen = prob + LenMid + (posState << kLenNumMidBits);
		offset = kLenNumLowSymbols;
		numBits = kLenNumMidBits;
	}
		  else
		  {
			  UpdateBit1(probLen);
			  probLen = prob + LenHigh;
			  offset = kLenNumLowSymbols + kLenNumMidSymbols;
			  numBits = kLenNumHighBits;
		  }
}
RangeDecoderBitTreeDecode(probLen, numBits, len);
len += offset;
	}

	if (state < 4)
	{
		int posSlot;
		state += kNumLitStates;
		prob = p + PosSlot +
			((len < kNumLenToPosStates ? len : kNumLenToPosStates - 1) << 
			kNumPosSlotBits);
		RangeDecoderBitTreeDecode(prob, kNumPosSlotBits, posSlot);
		if (posSlot >= kStartPosModelIndex)
		{
			int numDirectBits = ((posSlot >> 1) - 1);
			rep0 = (2 | ((UInt32)posSlot & 1));
			if (posSlot < kEndPosModelIndex)
			{
				rep0 <<= numDirectBits;
				prob = p + SpecPos + rep0 - posSlot - 1;
			}
			else
			{
				numDirectBits -= kNumAlignBits;
				do
				{
					RC_NORMALIZE
						Range >>= 1;
					rep0 <<= 1;
					if (Code >= Range)
					{
						Code -= Range;
						rep0 |= 1;
					}
				}
				while (--numDirectBits != 0);
				prob = p + Align;
				rep0 <<= kNumAlignBits;
				numDirectBits = kNumAlignBits;
			}
			{
				int i = 1;
				int mi = 1;
				do
				{
					CProb *prob3 = prob + mi;
					RC_GET_BIT2(prob3, mi, ; , rep0 |= i);
					i <<= 1;
				}
				while(--numDirectBits != 0);
			}
		}
		else
			rep0 = posSlot;
		if (++rep0 == (UInt32)(0))
		{
			/* it's for stream version */
			len = kLzmaStreamWasFinishedId;
			break;
		}
	}

	len += kMatchMinLen;
	if (rep0 > nowPos)
		throw 1;

	do
	{
		previousByte = outStream[nowPos - rep0];
		len--;
		outStream[nowPos++] = previousByte;
	}
	while(len != 0 && nowPos < outSize);
	}
	}
	RC_NORMALIZE;

	if (inSizeProcessed) *inSizeProcessed = (SizeT)(Buffer - inStream);
	if (outSizeProcessed) *outSizeProcessed = nowPos;
	return LZMA_RESULT_OK;
	}
	catch(...)
	{
		if (inSizeProcessed) *inSizeProcessed = (SizeT)(Buffer - inStream);
		if (outSizeProcessed) *outSizeProcessed = nowPos;
		return LZMA_RESULT_DATA_ERROR;
	}
}
int lzma_decode(CLzmaProperties *probs,
			   const unsigned char *inStream, SizeT inSize, SizeT *inSizeProcessed,
			   unsigned char *outStream, SizeT outSize, SizeT *outSizeProcessed)
{
	SizeT prob_size = LzmaGetNumProbs(probs) * sizeof(CProb);
	if (prob_size<=0) return LZMA_RESULT_DATA_ERROR;
	CProb *prob = (CProb *)malloc(prob_size);
	if (NULL==prob) return LZMA_RESULT_DATA_ERROR;
	
	try
	{
		int ret  = inline_LzmaDecode(probs, prob, inStream, inSize, inSizeProcessed, outStream, outSize, outSizeProcessed);
		free(prob);
		return ret;
	}
	catch(...)
	{
		free(prob);
		return LZMA_RESULT_DATA_ERROR;
	}
}
