#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "commands/pwd/pwd.h"
#include "commands/ls/ls.h"
#include "commands/cd/cd.h"
#include "commands/less/less.h"
#include "commands/help/help.h"
#include "commands/man/man.h"
#include "commands/rm/rm.h"
#include "commands/mv/mv.h"
#include "commands/echo/echo.h"
#include "commands/cat/cat.h"
#include "commands/parser.h"

#define BUFF_SIZE 512

void child_process(char **parse_command, int argc, char* temp)
{
    char arr[BUFF_SIZE] = { 0 };
    if (strcmp(parse_command[0], "quit") == 0
            || strcmp(parse_command[0], "exit") == 0)
    {
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
    else if (strcmp(parse_command[0], "mv") == 0)
        mv(argc, parse_command);
    else if (strcmp(parse_command[0], "echo") == 0)
        echo(argc, parse_command);
    else if (strcmp(parse_command[0], "cat") == 0)
        cat(argc, parse_command);
    else
    {
        execvp(parse_command[0], parse_command);
        write(STDOUT_FILENO, "Invalid command\n", 16);
    }

    _exit(0);
}


int main()
{
    char temp[BUFF_SIZE] = { 0 };
    char **parse_command; //= malloc(sizeof(char *));

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

        char *sep_redirection = ">";
        char *sep_args = " ";
        int argc = 0;

        struct parsed_part *parsed = parse_all_input(buf, sep_redirection);
        parse_command = parse_part_to_arg(parsed, sep_args, &argc);

        struct parsed_part *temp_parse = parsed;
        struct parsed_part *prev;
        while(temp_parse->buf)
        {
            prev = temp_parse;
            temp_parse = temp_parse->next;
        }

        int fd = -1;
        int output = dup(STDOUT_FILENO);
        if (parsed->next->buf)
        {
            char* redirect = prev->args->value;
            fd = open(redirect, O_WRONLY | O_TRUNC | O_CREAT,0644);
            if (fd < 0)
            {
                write(STDOUT_FILENO, "An error appeared\n", 18);
                free_parsed_part(parsed);
                free_parse_command(parse_command);
                continue;
            }
            else
                dup2(fd, STDOUT_FILENO);
        }

        if (strcmp(parse_command[0], "cd") == 0)
        {
            cd(argc, parse_command, &path);
            free_parsed_part(parsed);
            free_parse_command(parse_command);
            if (fd > 0)
                close(fd);
            continue;
        }

        pid_t process = fork();

        if (process == -1)
        {
            free_parsed_part(parsed);
            free_parse_command(parse_command);
            if (fd > 0)
                close(fd);
            dup2(output, STDOUT_FILENO);
            close(output);

            return -1;
        }
        else if (process == 0)
        {
            child_process(parse_command, argc, temp);
        }

        else 
        {
            wait(&status);
            free_parsed_part(parsed);
            free_parse_command(parse_command);
            if (fd > 0)
                close(fd);
            dup2(output, STDOUT_FILENO);
            close(output);
            if (WIFEXITED(status))
            {
                if (WEXITSTATUS(status) == EXIT_FAILURE)
                    return 1;
            }
        }
    }

    return 0;

}
