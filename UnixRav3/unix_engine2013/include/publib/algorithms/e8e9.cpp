#include "algorithms.h"

long inline_e8e9(unsigned char * start, long length)
{
	try
	{
		for(long i=0; i < length-4; )
		{
			if (*(start+i)==0xE8 || *(start+i)==0xE9)
			{
				unsigned long fix  =
					( (*(start+i+1)<<24) | (*(start+i+2)<<16) | (*(start+i+3)<<8) | (*(start+i+4)) ) - i -1;
				*(start+i+1) = (unsigned char)(fix & 0xff); 
				*(start+i+2) = (unsigned char)((fix>>8) & 0xff); 
				*(start+i+3) = (unsigned char)((fix>>16) & 0xff); 
				*(start+i+4) = (unsigned char)((fix>>24) & 0xff); 
				i += 5;
			}
			else
				++ i;
		}
		return 0;
	}
	catch(...)
	{
		throw;
	}
}

long e8e9(unsigned char * start, long length)
{
	try
	{
		return inline_e8e9(start, length);
	}
	catch(...)
	{
		return -1;
	}
}
