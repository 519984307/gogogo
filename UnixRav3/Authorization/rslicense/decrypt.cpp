#include "decrypt.h"
#include "RisingAccredit.h"


void Log(const char * fmt, ...)
{
	char    buf[1024] = {0};
	int     off = 0, len = 0;
	FILE  * flog;
	va_list args;

	flog = fopen("/tmp/test_decrypt.log", "ab");
	if(!flog) return;

	va_start(args, fmt);
	len = vsnprintf(buf + off, sizeof(buf)-1-off, fmt, args);
	va_end(args);

	fwrite(buf, 1, len, flog);
	fflush(flog);
	fclose(flog);
}

int decrypt_rs_sn(char* sn ,char * dir)
{
	CRisingAccredit test;
	DWORD dwVal = test.ParseAccredit(sn, dir, 1);
	LOG("decrypt(%s, %s) = %d\n", sn, dir, dwVal);
    return dwVal;
}
