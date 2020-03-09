#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>
#include "commands/pwd.h"
#include "commands/ls.h"
#include "commands/cd.h"

/*
#include "commands/less.h"
*/

#define BUFF_SIZE 512

char** parse_input(char *buf)
{
	char **parse = calloc(8, sizeof(char *));
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
	char arr[256];
	char buf[BUFF_SIZE] = { 0 };
	char **parse_command;

	
	char shell[2] = {'$', ' '};

	ssize_t w;
	ssize_t r = 1;

	while(r != 0)
	{
		w = write(STDOUT_FILENO, shell, 2);
		if (w == -1)
			errx(EXIT_FAILURE, "Error while writing");

		r = read(STDIN_FILENO, buf, BUFF_SIZE);
		if (r == -1)
			errx(EXIT_FAILURE, "Error while reading");

		int i = 0;
		for (; buf[i] != '\0'; i++)
			continue;
		buf[i-1] = ' ';	
		parse_command = parse_input(buf);		
		if (strcmp(parse_command[0], "quit") == 0)
			break;
		if (strcmp(parse_command[0], "cd") == 0)
		{
			// cd command
			if (cd(2,parse_command) == -1)
				errx(EXIT_FAILURE, "Error with cd command");
		}
		if (strcmp(parse_command[0], "ls") == 0)
	 	{
			// ls command
			simple_ls();
		}
		if (strcmp(parse_command[0], "pwd") == 0)
		{
			pwd(arr,256);
		}

		free(parse_command);
	}

	free(parse_command);

	return 0;

}
