#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(int argc, const char *argv[])
{
	if(argc	!= 2)
	{
		printf("parameter number error\n");
		return 1;
	}

	setuid(geteuid());
	setgid(getegid());
	system(argv[argc-1]);

	return 0;


}
