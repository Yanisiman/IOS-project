#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "commands/pwd/pwd.h"
#include "commands/ls/ls.h"
#include "commands/cd/cd.h"
#include "commands/less/less.h"
#include "commands/help/help.h"
#include "commands/man/man.h"
#include "commands/rm/rm.h"
#include "commands/echo/echo.h"
#include "commands/parser.h"

#define BUFF_SIZE 512

char** parse_input(char *buf)
{
    int alloc = 8;
    char **parse = calloc(alloc, sizeof(char *));
    if (parse == NULL)
        errx(EXIT_FAILURE, "Error trying to allocate memory for the parse_input");

    int args = 0;
    char *separator = " ";
    char *parsed;

    parsed = strtok(buf, separator);
    while (parsed != NULL)
    {
        if (args == alloc - 1)
        {
            alloc *= 2;
            parse = realloc(parse, alloc * sizeof(char *));
            if (parse == NULL)
                errx(EXIT_FAILURE, "Error trying to realloc memory for the parse_input");
        }
        parse[args] = parsed;
        args++;
        parsed = strtok(NULL, separator);
    }
    parse[args] = NULL;
    return parse;
}

int main()
{
    char arr[BUFF_SIZE] = { 0 };
    char temp[BUFF_SIZE] = { 0 };
    char **parse_command;

    char shell[1] = {'$'};

    ssize_t w;
    ssize_t r = 1;

    int status;

    char* path = getcwd(temp, BUFF_SIZE);

    char *first_cd[2] = {"cd", "./Home"};
    if (cd(2, first_cd, &path) == -1)
        errx(EXIT_FAILURE, "Error trying to cd to Home folder");

    while(r != 0)
    {
        //write(STDOUT_FILENO, "\033[0;41m",7);
        w = write(STDOUT_FILENO, shell, 1);
        //write(STDOUT_FILENO, "\033[0m", 4);
        write(STDOUT_FILENO, " ", 1);

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
            cd(argc, parse_command, &path);
            free(parse_command);
            continue;
        }

        pid_t process = fork();

        if (process == -1)
        {
            free(parse_command);
            return -1;
        }
        else if (process == 0)
        {
            //printf("%d\n", getpid());

            if (strcmp(parse_command[0], "quit") == 0
                    || strcmp(parse_command[0], "exit") == 0)
            {
                free(parse_command);
                _exit(EXIT_FAILURE);
            }

            else if (strcmp(parse_command[0], "ls") == 0)
                ls(argc, parse_command);
            else if (strcmp(parse_command[0], "pwd") == 0)
                pwd(arr,256);
            else if (strcmp(parse_command[0], "less") == 0)
                less(argc, parse_command);
            else if (strcmp(parse_command[0], "clear") == 0)
                write(STDOUT_FILENO, "\e[1;1H\e[2J", 11);
            else if (strcmp(parse_command[0], "help") == 0)
                help();
            else if (strcmp(parse_command[0], "man") == 0)
                man(argc, parse_command, temp);
            else if (strcmp(parse_command[0], "rm") == 0)
                rm(parse_command);
            else if (strcmp(parse_command[0], "echo") == 0)
                echo(argc, parse_command);
            else
            {
                execvp(parse_command[0], parse_command);
                write(STDOUT_FILENO, "Invalid command\n", 16);
            }

            //free(parse_command);

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

        free(parse_command);
    }

    free(parse_command);

    return 0;


}

