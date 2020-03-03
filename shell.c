#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>

#define BUFF_SIZE 512

char** parse_input(char *buf)
{
	char **parse = calloc(4, sizeof(char *));
	if (parse == NULL)
		errx(EXIT_FAILURE, "Error trying to allocate memory for the parse_input");

	int args = 0;
	char *separator = " ";
	char *parsed;

	parsed = strtok(buf, separator);
	while (parsed != NULL)
	{
		parse[args] = parsed;
		args++;
		parsed = strtok(NULL, separator);
	}
	parse[args] = NULL;
	return parse;

}

int main()
{
	//char* input;
	char pwd[256];
	pid_t child_pid;
	char buf[BUFF_SIZE];

	/*
	char* path = getcwd(pwd, 256);
	size_t len = strlen(path);
	if (len <= 256)
	{
		path[len] = ' ';
		path[len+1] = '\0';
		
	}*/
	char shell[1] = {'>'};

	//ssize_t w = write(STDOUT_FILENO, path, len+1);
	ssize_t w = write(STDOUT_FILENO, shell, 1);
	ssize_t r = read(STDIN_FILENO, buf, BUFF_SIZE);

	while(r != 0)
	{
		/*
		path = getcwd(pwd, 256);
		
		len = strlen(path);
		if (len <= 256)
		{
			path[len] = ' ';
			path[len+1] = '\0';
		}
		*/

		//w = write(STDOUT_FILENO, path, len+1);
		w = write(STDOUT_FILENO, shell, 1);
		r = read(STDIN_FILENO, buf, BUFF_SIZE);
	
		char ** parse_command = parse_input(buf);		
		if (strcmp(buf, "quit") == 0)
			break;

		/*
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
