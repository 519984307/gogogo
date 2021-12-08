#ifndef __RX_SET_CONSOLE_COLOR__
#define __RX_SET_CONSOLE_COLOR__

namespace RSSetColor
{
	enum color_style
	{
		blue	= 0x0001,
		green	= 0x0002,
		red		= 0x0004,
		rg		= red | green,
		rb		= red | blue,
		gb		= green | blue,
		rgb		= blue | green | red,
	};


#ifdef _WIN32

	STATICINLINE void SetConsoleColor( int color )
	{
		WORD wAttributes = FOREGROUND_INTENSITY;
		if ( color & blue ) wAttributes |= FOREGROUND_BLUE;
		if ( color & red ) wAttributes |= FOREGROUND_RED;
		if ( color & green ) wAttributes |= FOREGROUND_GREEN;
		SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), wAttributes );
	}

#else

	#define NONE                 "\e[0m"
	#define BLACK                "\e[0;30m"
	#define L_BLACK              "\e[1;30m"
	#define RED                  "\e[0;31m"
	#define L_RED                "\e[1;31m"
	#define GREEN                "\e[0;32m"
	#define L_GREEN              "\e[1;32m"
	#define BROWN                "\e[0;33m"
	#define YELLOW               "\e[1;33m"
	#define BLUE                 "\e[0;34m"
	#define L_BLUE               "\e[1;34m"
	#define PURPLE               "\e[0;35m"
	#define L_PURPLE             "\e[1;35m"
	#define CYAN                 "\e[0;36m"
	#define L_CYAN               "\e[1;36m"
	#define GRAY                 "\e[0;37m"
	#define WHITE                "\e[1;37m"

	#define BOLD                 "\e[1m"
	#define UNDERLINE            "\e[4m"
	#define BLINK                "\e[5m"
	#define REVERSE              "\e[7m"
	#define HIDE                 "\e[8m"
	#define CLEAR                "\e[2J"
	#define CLRLINE              "\r\e[K" //or "\e[1K\r"

	STATICINLINE void SetConsoleColor( int color )
	{
		const char* lpAttributes = NULL;
		switch (color)
		{
		case red:	lpAttributes = RED; break; //"\033[22;31m";
		case green:	lpAttributes = GREEN; break; //"\033[22;32m";
		case blue:	lpAttributes = BLUE; break; //"\033[22;31m";
		case rgb:	lpAttributes = WHITE; break; //"\033[01;37m";			
		case rg:	lpAttributes = YELLOW; break;
		case rb:	lpAttributes = PURPLE; break;
		case gb:	lpAttributes = CYAN; break;
		default:	lpAttributes = NONE; break;
		}
		printf( lpAttributes );
	}

#endif

};

#endif //__RX_SET_CONSOLE_COLOR__