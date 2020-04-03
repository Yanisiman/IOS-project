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
    if(filedesc > 0)
    {
        while(1)
        {
            char message[BUFFERSIZE] = { 0 };
            int desc = read(filedesc, message, BUFFERSIZE);
            if (desc <= 0)
                break;
            write(STDOUT_FILENO, message, desc);
        }
        close(filedesc);
    }
    return 0;
}

int cd(int argc, char *argv[], char** last_cd)
{
    if (argc < 1)
    {
        write(STDERR_FILENO, "An error occured\n", 17);
        return -1;
    }
    if (argc > 2)
    {
        write(STDERR_FILENO, "Error: cd: Too much arguments\n", 29);
        return -1;
    }

    char s[BUFFERSIZE] = { 0 };
    getcwd(s, BUFFERSIZE);
    int a;

    if (argc < 2)
        a = chdir(getenv("HOME"));
    else
    {
        if (argv[1][0] == '-')
            a = chdir(*last_cd);
        else
            a = chdir(argv[1]);
    }

    if (a < 0)
    {
        ssize_t k = strlen(argv[1]);
        write(STDERR_FILENO, "Error: can't find any folder named: ", 37);
        write(STDERR_FILENO, argv[1], k);
        write(STDERR_FILENO, "\n", 1);
        return -1;
    }
    strcpy(*last_cd, s);
    tellDescription();

    return 0;
}