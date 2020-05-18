#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>

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
#include "commands/cp/cp.h"
#include "commands/chmod/chmod.h"
#include "commands/touch/touch.h"
#include "commands/makedir/makedir.h"
#include "commands/stat/stat.h"
#include "commands/wc/wc.h"
#include "commands/grep/grep.h"
#include "commands/tail/tail.h"
#include "commands/head/head.h"
#include "commands/find/find.h"
#include "commands/parser/parser.h"
#include "commands/ln/ln.h"

#define BUFF_SIZE 512

struct thread
{
    pthread_t thread;
    struct parsed_part *parsed;
    char *path;
    char *temp;
    int ret;
};

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
        rm(argc, parse_command);
    else if (strcmp(parse_command[0], "mv") == 0)
        mv(argc, parse_command);
    else if (strcmp(parse_command[0], "echo") == 0)
        echo(argc, parse_command);
    else if (strcmp(parse_command[0], "cat") == 0)
        cat(argc, parse_command);
    else if (strcmp(parse_command[0], "cp") == 0)
        cp(argc, parse_command);
    else if (strcmp(parse_command[0], "chmod") == 0)
        chmod_command(argc, parse_command);
    else if (strcmp(parse_command[0], "touch") == 0)
        touch(argc, parse_command);
    else if (strcmp(parse_command[0], "makedir") == 0)
        makedir(argc, parse_command);
    else if (strcmp(parse_command[0], "stat") == 0)
        stat_(argc, parse_command);
    else if (strcmp(parse_command[0], "wc") == 0)
        wc(argc, parse_command);
    else if (strcmp(parse_command[0], "grep") == 0)
        grep(argc, parse_command);
    else if (strcmp(parse_command[0], "tail") == 0)
        tail(argc, parse_command);
    else if (strcmp(parse_command[0], "head") == 0)
        head(argc, parse_command);
    else if (strcmp(parse_command[0], "find") == 0)
        find(argc, parse_command);
    else if (strcmp(parse_command[0], "ln") == 0)
        ln(argc, parse_command);
    else
    {
        execvp(parse_command[0], parse_command);
        write(STDOUT_FILENO, "Invalid command\n", 16);
    }

    _exit(0);
}

int exec_parts(char *buf, char *path, char *temp)
{
    int output = 0;
    int argc = 0;
    int fd = -1;
    int status;

    struct parsed_part *parsed;
    char **parse_command = parse_redirections(buf, &output, &parsed, &argc, &fd);
    if (parse_command == NULL)
        return 0;
    if (parse_command[0] == NULL)
    {
        free_parsed_part(parsed);
        free_parse_command(parse_command);
        return 0;
    }

    if (strcmp(parse_command[0], "cd") == 0)
    {
        cd(argc, parse_command, &path);
        free_parsed_part(parsed);
        free_parse_command(parse_command);
        if (fd > 0)
            close(fd);
        return 0;
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

        return 0;
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
            {
                return -1;
            }
        }
    }
    return 0;
}

int work(void *arg)
{
    struct thread *t = (struct thread*) arg;
    char *path = t->path;
    char *temp = t->temp;
    char *buf = t->parsed->buf;

    struct parsed_part *pipes = parse_pipes(buf);
    int quit = 0;
    int num = pipes->parts;
    if (num < 2)
    {
        int err = exec_parts(pipes->buf, path, temp);
        if (err == -1)
        {
            t->ret = 1;
            quit = 1;
        }
        free_parsed_part(pipes);
        return quit;
    }
    struct parsed_part *tp = pipes;
    int fd_in = STDIN_FILENO;
    int save_out = dup(STDOUT_FILENO);
    int save_in = dup(STDIN_FILENO);

    int fd[2];
    pid_t pid;

    while (tp->buf != NULL)
    {
        if (pipe(fd) == -1)
            err(EXIT_FAILURE, "Error with pipe");
        pid = fork();
        if (pid == -1)
            err(EXIT_FAILURE, "Error with fork");
        if (pid == 0)
        {
            dup2(fd_in, STDIN_FILENO);
            if (tp->next->buf != NULL)
                dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            int err = exec_parts(tp->buf, path, temp);
            exit(err);
        }
        else
        {
            wait(NULL);
            close(fd[1]);
            fd_in = fd[0];
            tp = tp->next;
        }
    }
    dup2(save_out, STDOUT_FILENO);
    dup2(save_in, STDIN_FILENO);
    free_parsed_part(pipes);
    return quit;
}


int main()
{
    char temp[BUFF_SIZE] = { 0 };
    char shell[1] = {'$'};

    ssize_t w;
    ssize_t r = 1;

    //int rval = 0;

    char* path = getcwd(temp, BUFF_SIZE);

    char *first_cd[2] = {"cd", "./Home"};
    if (cd(2, first_cd, &path) == -1)
        errx(EXIT_FAILURE, "Error trying to cd to Home folder");

    while(r != 0)
    {
        w = write(STDOUT_FILENO, shell, 1);
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

        struct parsed_part *parsed_instructions = parse_all_input(buf, ";");
        struct parsed_part *tmp = parsed_instructions;

        int exit = 0;
        while (tmp->buf)
        {

            struct parsed_part *parsed_and = parse_and(tmp->buf);
            struct parsed_part *temp_and = parsed_and;

            struct thread *threads = calloc(parsed_and->parts, sizeof(struct thread));
            int t = 0;
            while (temp_and->buf)
            {
                threads[t].parsed = temp_and;
                threads[t].path = path;
                threads[t].temp = temp;
                //pthread_create(&(threads[t].thread), NULL, work, (void *) &threads[t]);
                exit += work((void*) &threads[t]);
                temp_and = temp_and->next;
                t++;
            }

            /*
               temp_and = parsed_and;
               t = 0;
               while(temp_and->buf)
               {
               pthread_join(threads[t].thread, (void **) &rval);
               exit += threads[t].ret;
               temp_and = temp_and->next;
               t++;
               }
             */

            free_parsed_part(parsed_and);
            free(threads);

            if (exit)
                break;
            tmp = tmp->next;
        }
        free_parsed_part(parsed_instructions);
        if (exit)
            break;

    }

    return 0;

}

void worker(int fd)
{
    int save_out = dup(STDOUT_FILENO);
    int save_in = dup(STDIN_FILENO);
    int save_err = dup(STDERR_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDERR_FILENO);

    main();

    dup2(save_out, STDOUT_FILENO);
    dup2(save_in, STDIN_FILENO);
    dup2(save_err, STDERR_FILENO);

    close(save_out);
    close(save_in);
    close(save_err);
    close(fd);

}

int main_()
{
    struct addrinfo hints;
    struct addrinfo *results;
    int addrinfo_error;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    addrinfo_error = getaddrinfo("localhost", "2048", &hints, &results);

    if (addrinfo_error != 0)
        errx(EXIT_FAILURE, "Fail to get address for localhost on port 2048: %s", gai_strerror(addrinfo_error));

    struct addrinfo *rp;
    int cnx;

    for (rp = results; rp; rp = rp->ai_next)
    {
        cnx = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (cnx == -1)
            continue;
        int value = 1;
        if (setsockopt(cnx, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) == -1)
            err(EXIT_FAILURE, "Error caught by setsockopt");
        if (bind(cnx, rp->ai_addr, rp->ai_addrlen) == 0)
            break;
        close(cnx);
    }

    freeaddrinfo(results);

    if (!rp)
        err(EXIT_FAILURE, "Couldn't connect");
    if (listen(cnx, 5) == -1)
        err(EXIT_FAILURE, "Error listening");

    int fd;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    socklen_t addrlen = sizeof(struct sockaddr_in);

    write(STDOUT_FILENO, "Static Server\nListening to port 2048...\n", 40);
    while(1)
    {
        fd = accept(cnx, (struct sockaddr *) &addr, &addrlen);
        if (fd == -1)
            err(EXIT_FAILURE, "Error with accept");

        pid_t process = fork();
        if (process == -1)
            err(EXIT_FAILURE, "Error with the fork()");
        else if (process == 0)
        {
            close(cnx);
            printf("New connection (pid = %i)\n", getpid());
            fflush(stdout);
            worker(fd);

            signal(SIGCHLD, SIG_IGN);
            close(fd);
            printf("Close connection (pid = %i)\n", getpid());
            return 0;
        }
        else 
        {
            close(fd);
        }

    }
    close(cnx);
    return 0;
}
