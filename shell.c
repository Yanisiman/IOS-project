#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>


int main()
{
	char* input;
	char pwd[256];
	pid_t child_pid;

	while(1)
	{
		char* path = getcwd(pwd, 256);
		input = readline(path);
		
		if (strcmp(input, "quit") == 0)
			break;

		child_pid = fork();
		if (child_pid == 0)
		{
			/*
			execvp();
			or 
			call to some function
			*/
		}
		else
		{
			waitpid(NULL);
		}
	}

	free(input);

	return 0;

}
