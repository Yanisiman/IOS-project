#include <stdio.h> //printf
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <err.h>

#define BUFFERSIZE 512


int tellDescription()
{
	int filedesc = open("description.txt", O_RDONLY);
	if(filedesc < 0)
	{
		//errx(EXIT_FAILURE, "Error: No corresponding file");
	}
	else
	{
		char message[BUFFERSIZE] = { 0 };
		int desc = read(filedesc, message, BUFFERSIZE);
		message[desc] = '\n'; 
		write(STDOUT_FILENO, message, desc);
		close(filedesc);
	}
	return 0;
}
int cd(int argc, char *argv[]) 
{    
	char s[BUFFERSIZE] = { 0 }; 
	int a;

	// printf("%s\n", getcwd(s, BUFFERSIZE));     

	if(argc < 2){
		chdir(getenv("HOME"));
		tellDescription();
		printf("\n%s\n", getcwd(s, BUFFERSIZE));
		return 1;	
	}

	a = chdir(argv[1]);

	if(a < 0)
	{
		write(STDOUT_FILENO, "Error occured doing cd command\n", 31);
		return -1;
	}

	tellDescription();
	printf("\n%s\n", getcwd(s, BUFFERSIZE)); 

	return 0;
} 
