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
			write(STDOUT_FILENO, "\n", 1);
        }
        close(filedesc);
    }
    return 0;
}

int cd(int argc, char *argv[], char** last_cd)
{
   
    if (argc > 2)
    {
        write(STDERR_FILENO, "Too many arguments are given.\n", 30);
        return EXIT_FAILURE;
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
        write(STDERR_FILENO, "There is no room that you can enter called ", 43);
        write(STDERR_FILENO, argv[1], k);
        write(STDERR_FILENO, ".\n", 2);
        return EXIT_FAILURE;
    }
    strcpy(*last_cd, s);
    tellDescription();

    return 0;
}
