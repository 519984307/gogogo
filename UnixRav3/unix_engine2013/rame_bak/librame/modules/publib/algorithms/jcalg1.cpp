//***************************************************************************
//
// Implementation of DecompressJCALG1 in C++ by Jeffrey Lim
// 
// JCALG1 is copyright (C) Jeremy Collake
//
// Note: This code should work on any little-endian system that supports
//		 mis-aligned reads. To support non-little endian, the functions
//		 CompressionSourceData::GetBit() and CompressionSourceData::GetBits(int)
//		 need to be modified.
//		 If the target system does not support mis-aligned reads, either
//		 the actual decompresssion data has to be made to align to 4-byte 
//		 boundaries [note that there's a 10-byte header on JCALG1], or else 
//		 the CompressionSourceData routines need to be modified.
//
//***************************************************************************

#include "algorithms.h"


//***************************************************************************


class CompressionSourceData
{
private:
	const unsigned char *Data;
	UINT32				BitBuffer;
	INT32				BitsRemaining;
	INT32				_size;
	INT32				_curcount;

public:
	CompressionSourceData(const unsigned char *iData);

	int GetBit();
	int GetBits(int Count);
	int GetInteger();
	void SetSourceSize(UINT32 size) { _size = size; }

	unsigned long GetProcessCount() {return _curcount;} 
};

CompressionSourceData::CompressionSourceData(const unsigned char *iData)
{
	Data			= iData;
	BitBuffer		= 0;
	BitsRemaining	= 0;
	_curcount		= 0;
}

int CompressionSourceData::GetBit()
{
	int ReturnValue;

	if(!BitsRemaining)
	{
		BitsRemaining = 32;
		if (_size-_curcount < 4) throw 1;
		_curcount += 4;
		BitBuffer = GETDWORD(Data);
		Data += 4;
	}

	ReturnValue = BitBuffer >> 31;
	BitBuffer <<= 1;
	BitsRemaining--;
	return ReturnValue;
}

int CompressionSourceData::GetBits(int Count)
{
	if(BitsRemaining >= Count)
	{
		int ReturnValue = BitBuffer >> (32-Count);
		BitBuffer <<= Count;
		BitsRemaining -= Count;
		return ReturnValue;
	}
	else
	{
		int Remainder = Count-BitsRemaining;

		int ReturnValue = BitBuffer >> (32-BitsRemaining) << Remainder;
		if (_size-_curcount < 4) throw 1;
		_curcount += 4;
		BitBuffer = GETDWORD(Data);
		Data += 4;

		ReturnValue |= BitBuffer >> (32-Remainder);
		BitsRemaining = 32-Remainder;
		BitBuffer <<= Remainder;

		return ReturnValue;
	}
}

int CompressionSourceData::GetInteger()
{
	int Value = 1;
	do
	{
		Value = (Value<<1) + GetBit();
	} while(GetBit());

	return Value;
}

//***************************************************************************

struct CompressionState
{
	int		LastIndex;
	int		IndexBase;
	int		LiteralBits;
	int		LiteralOffset;

	CompressionState() { LastIndex = 1; IndexBase = 8; }
};

//***************************************************************************

void TransferMatch(unsigned char* &Destination, int MatchOffset, int MatchLength)
{
	unsigned char* p = Destination;
	unsigned char* s = p-MatchOffset;

	do
	{
		*p++ = *s++;
	} while(--MatchLength);

	Destination = p;
}

//***************************************************************************

bool jcalg1_decode(unsigned char *pDestination, ULONG desclen, const unsigned char *pSource, ULONG srclen, ULONG* srcProcess, ULONG * destProcess)
{
	CompressionState		State;
	CompressionSourceData	Source(pSource);  //我故意不加10!! // +10 skips the header of JCALG. A more robust routine would check this
	unsigned char *Destination =  pDestination;

	if (desclen==0 || srclen==0) return true;
	Source.SetSourceSize(srclen);
	unsigned long desccount = 0;

	try
	{
	while(1)
	{
		if(Source.GetBit())
		{
			// Is Literal
			if (desclen-desccount<1) throw 1;
			desccount ++;
			*Destination++ = Source.GetBits(State.LiteralBits) + State.LiteralOffset;
		}
		else
		{
			// Isn't literal
			if(Source.GetBit())
			{
				// Normal phrase
				int HighIndex = Source.GetInteger();

				if(HighIndex == 2) // Use the last index
				{
					int PhraseLength = Source.GetInteger();
					if ((unsigned long)State.LastIndex > desccount) throw 1;
					if (desclen-desccount < (unsigned long)PhraseLength) throw 1;
					desccount += PhraseLength;
					TransferMatch(Destination, State.LastIndex, PhraseLength);
				}
				else
				{
					State.LastIndex = ((HighIndex-3) << State.IndexBase) + Source.GetBits(State.IndexBase);

					int PhraseLength = Source.GetInteger();

					if(State.LastIndex >= 0x10000)		PhraseLength += 3;
					else if(State.LastIndex >= 0x37FF)	PhraseLength += 2;
					else if(State.LastIndex >= 0x27F)	PhraseLength++;
					else if(State.LastIndex <= 127)		PhraseLength += 4;

					if ((unsigned long)State.LastIndex > desccount) throw 1;
					if (desclen-desccount < (unsigned long)PhraseLength) throw 1;
					desccount += PhraseLength;
					TransferMatch(Destination, State.LastIndex, PhraseLength);
				}
			}
			else
			{
				if(Source.GetBit())
				{
					// OneBytePhrase or literal size change
					int OneBytePhraseValue = Source.GetBits(4) - 1;
					if(OneBytePhraseValue == 0)
					{
						if (desclen-desccount < 1) throw 1;
						desccount ++;
						*Destination++ = 0;
					}
					else if(OneBytePhraseValue > 0)
					{
						if ((unsigned long)(OneBytePhraseValue) > desccount) throw 1;
						if (desclen-desccount < 1) throw 1;
						desccount ++;
						*Destination = *(Destination-OneBytePhraseValue);
						Destination++;
					}
					else
					{
						if(Source.GetBit())
						{
							// Next block
							do
							{
								if (desclen-desccount < 256) throw 1;
								desccount ++;
								for(int i = 0; i < 256; i++) *Destination++ = Source.GetBits(8);
							} while(Source.GetBit());
						}
						else
						{
							// New literal size
							State.LiteralBits = 7+Source.GetBit();
							State.LiteralOffset = 0;
							if(State.LiteralBits != 8)
							{
								State.LiteralOffset = Source.GetBits(8);
							}
						}
					}
				}
				else
				{
					// Short match
					int NewIndex = Source.GetBits(7);
					int MatchLength = 2+Source.GetBits(2);

					if(NewIndex == 0)
					{
						// Extended short
						if(MatchLength == 2) break;	// End of decompression

						State.IndexBase = Source.GetBits(MatchLength+1);
					}
					else
					{
						if ((unsigned long)State.LastIndex > desccount) throw 1;
						if (desclen-desccount < (unsigned long)MatchLength) throw 1;
						desccount += MatchLength;
						State.LastIndex = NewIndex;
						TransferMatch(Destination, State.LastIndex, MatchLength);
					}
				}
			}
		}
	}	// loop
	if (destProcess) *destProcess = desccount;
	if (srcProcess) * srcProcess = Source.GetProcessCount();

	return true;
	}	// try
	catch(...)
	{
		return false;
	}
}

//***************************************************************************
