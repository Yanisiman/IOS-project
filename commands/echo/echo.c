#include <stdio.h>
#include <unistd.h>
#include <string.h>


void echo(int argc, char **argv)
{
    if (argc < 1)
    {
        write(STDOUT_FILENO, "An error occured", 16);
        return;
    }
    for (int i = 1; i < argc; i++)
    {
        write(STDOUT_FILENO, argv[i], strlen(argv[i]));
        write(STDOUT_FILENO, " ", 1);
    }
    write(STDOUT_FILENO, "\n", 1);
}
