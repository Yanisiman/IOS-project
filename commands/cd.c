#include <stdio.h> //printf
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <err.h>

#define BUFFERSIZE 512


int tellDescription(){
	int filedesc = open("description.txt", O_RDONLY);
	if(filedesc < 0)
	{
		//errx(EXIT_FAILURE, "Error: No corresponding file");
	}
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
int cd(int argc, char *argv[]) 
{    

	char s[BUFFERSIZE]; 

	// printf("%s\n", getcwd(s, BUFFERSIZE));     // printing current working directory 
	
	// for indicating if cd command was success
	int a;

	char path[BUFFERSIZE];
	path[0] = '\0';

	//going to root with no args
	if(argc < 2){
		chdir(getenv("HOME"));
		tellDescription();
		printf("%s\n", getcwd(s, BUFFERSIZE));
		return 1;	
	}

	strcat(path, argv[1]);
	char cwd[BUFFERSIZE];

	if(path[0] != '/'){

		// true for the dir in cwd
		getcwd(cwd,sizeof(cwd));
		strcat(cwd,"/");
		strcat(cwd,path);

		a = chdir(cwd);
		tellDescription();

		if(a < 0)
			errx(EXIT_FAILURE, "Error occured doing cd command");
	}
	else
	{

		a = chdir(path);
		tellDescription();

		if(a < 0)
			errx(EXIT_FAILURE, "Error occured doing cd command");
	}
	printf("%s\n", getcwd(s, BUFFERSIZE)); 

	return 0;
} 
