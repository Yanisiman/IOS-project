#include <stdio.h> //printf
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> 
#include <err.h>

#define BUFFERSIZE 512

int less(int argc, char *argv[])
{
	if (argc <= 2)
		printf("Pick a different item to less");

	int filedesc = open(argv[1], O_RDONLY);
	if(filedesc < 0)
		printf("There is no %s here", argv[1]);
	else
	{
		char message[BUFFERSIZE];
		int desc = read(filedesc, message, BUFFERSIZE);
		message[desc] = '\0'; 
		write(1, message, desc );
		close(filedesc);
	}

	return 0;
}
