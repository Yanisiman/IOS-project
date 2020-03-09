#include <stdio.h> //printf
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> 

#define BUFFERSIZE 512

int main(int argc, char *argv[])
{
    int filedesc = open(argv[1], O_RDONLY);
    if(filedesc < 0){
		printf("%s\n", "an error occured reading file"); 
	}else{
		
		char message[BUFFERSIZE];
		char n;
		int desc = read(filedesc, message, BUFFERSIZE);
		message[desc] = '\0'; 
		write(1, message, desc );
		close(filedesc);
	}
	return 0;
}
