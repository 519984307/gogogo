#include "algorithms.h"

void e8e9_1(char *buf, long len)
{
	long ebp_8 = -5;
	long ebp_4 = 0;
	long ebp_c;
	long flag1;
	long edx;
	char dl;

	if (len-5 < 0) return;

	for(long i=0; i<len-5; ++i)
	{
		if ( (*(buf+i) != (char)0xe8) && (*(buf+i) != (char)0xe9) ) continue;

		//if (i>0x4400)
		//	MessageBox(0,0,0,0);
		long tmp = i-ebp_8;
		ebp_8 = i;


		if (tmp<=5)
		{
			long tmpbak = tmp;
			do 
			{
				ebp_4 &= 0x77;
				ebp_4 <<= 1;
			} while(--tmp);
			tmp = tmpbak;
		}
		else
		{
			ebp_4 = 0;
		}


		dl = *(buf+i+4);
		tmp = ebp_c = ebp_4 >> 1;
		
		switch( (ebp_4>>1) & 7 )
		{
		case 0:
		case 1:
		case 2:
		case 4:
			flag1 = 1;
			break;
		case 3:
		case 5:
		case 6:
		case 7:
			flag1 = 0;
			break;
		default:
			break;
		}
		if (dl==(char)0 || dl ==(char)0xff)
		{
			if (flag1 != 0)
			{
				if ( (ebp_4&0xfffffffe) < 0x20)
				{
					edx = GETDWORD(buf+i+1);

					for(; ;)
					{
						edx -=(i+5);
						if (ebp_4 == 0) break;
						
						if (tmp>3) tmp = 3;
						
						long aaa = 0x18 - (tmp<<3);
						if ( (((edx>>aaa) & 0xff) != 0) && (((edx>>aaa) & 0xff) != 0xff) ) break;
						edx ^= ((1 << ((4 - tmp) << 3)) - 1);
						tmp = ebp_c;
					}


					if ((edx>>24) & 1)
					{
						*(buf+i+4) = (char)0xff;
					}
					else
					{
						*(buf+i+4) = 0;
					}
					*(buf+i+3) = ((char)(edx>>16)) & 0xff;
					*(buf+i+2) = ((char)(edx>>8)) & 0xff;
					*(buf+i+1) = ((char)(edx)) & 0xff;

					ebp_4 = 0;
					i+=4;
					continue;
				}
			}
		}


		ebp_4 |= 1;
		if (dl==(char)0 || dl==(char)0xff)
			ebp_4 |= 10;
	}

}
