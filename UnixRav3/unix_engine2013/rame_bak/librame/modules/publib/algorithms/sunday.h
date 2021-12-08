#ifndef __SUNDAY_H_H__
#define __SUNDAY_H_H__

static LPCBYTE sunday( LPCBYTE text, size_t textlen, LPCBYTE want, size_t wantlen )
{
	if( textlen < wantlen ) return NULL;

	size_t next[0x100] = {};

	for( size_t j = 0; j < 0x100; ++j )
		next[j] = wantlen + 1;

	for( size_t j = 0; j < wantlen; ++ j )
		next[ want[j] ] = wantlen - j;

	for( size_t pos = 0; pos < (textlen - wantlen + 1);  )
	{
		size_t i = pos; size_t j = 0;
		for( ; j < wantlen; ++j, ++i )
		{
			if( text[i] != want[j] )
			{
				if( pos + wantlen >= textlen )
					return NULL;
				pos += next[ text[pos + wantlen] ];
				break;
			}
		}
		if(j == wantlen )
			return text + pos;
	}
	return NULL;
}


#endif //__SUNDAY_H_H__
