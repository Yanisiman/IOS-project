#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "commands/pwd.h"
#include "commands/ls.h"
#include "commands/cd.h"
#include "commands/less.h"
#include "commands/help.h"


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
	char **parse_command;

	char shell[2] = {'$', ' '};

	ssize_t w;
	ssize_t r = 1;

	int status;

	while(r != 0)
	{
		w = write(STDOUT_FILENO, shell, 2);
		if (w == -1)
			errx(EXIT_FAILURE, "Error while writing");

		char buf[BUFF_SIZE] = { 0 };
		r = read(STDIN_FILENO, buf, BUFF_SIZE);
		if (r == -1)
			errx(EXIT_FAILURE, "Error while reading");

		int i = 0;
		for (; buf[i] != '\0'; i++)
			continue;
		buf[i-1] = ' ';	
		parse_command = parse_input(buf);

		int argc = 0;
		for (; parse_command[argc]; argc++)
			continue;

		if (strcmp(parse_command[0], "cd") == 0)
		{
			if (cd(argc, parse_command) == -1)
				errx(EXIT_FAILURE, "Error with cd command");
			continue;
		}

		pid_t process = fork();

		if (process == -1)
			return -1;

		if (process == 0)
		{
			//printf("%d\n", getpid());

			if (strcmp(parse_command[0], "quit") == 0
					|| strcmp(parse_command[0], "exit") == 0)
				_exit(EXIT_FAILURE);


			else if (strcmp(parse_command[0], "ls") == 0)
			{
				simple_ls();
			}
			else if (strcmp(parse_command[0], "pwd") == 0)
			{
				pwd(arr,256);
			}
			else if (strcmp(parse_command[0], "less") == 0)
			{
				less(argc, parse_command);
			}

			else if (strcmp(parse_command[0], "clear") == 0)
			{
				write(STDOUT_FILENO, "\e[1;1H\e[2J", 12);
			}
			else
			{
				execvp(parse_command[0], parse_command);
			}

            if (strcmp(parse_command[0], "help") == 0)
			{
				help();
			}

			free(parse_command);

			_exit(0);
		}

		else 
		{
			//printf("%d\n", getpid());
			wait(&status);
			if (WIFEXITED(status))
			{
				if (WEXITSTATUS(status) == EXIT_FAILURE)
					return 1;
			}
		}
	}

	free(parse_command);

	return 0;

}
