#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>

#define BUFF_SIZE 512

int main()
{
	//char* input;
	char pwd[256];
	pid_t child_pid;
	char buf[BUFF_SIZE] = {0};
	char r;



	char* path = getcwd(pwd, 256);
	ssize_t w = write(STDOUT_FILENO, path, strlen(path));

	while((r = read(0, buf, BUFF_SIZE) > 0))
	{
		w = write(STDOUT_FILENO, buf, r);

		/*
		if (strcmp(input, "quit") == 0)
			break;

		child_pid = fork();
		if (child_pid == 0)
		{
			
			execvp();
			or 
			call to some function
			
		}
		else
		{
			wait(NULL);
		}
		*/
	}

	//free(input);

	return 0;

}
