#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void man_command(char* commands_path, char* command, int n)
{
    char path[512] = { 0 };
    sprintf(path, "%s/commands/%s/man", commands_path, command);
    int fd = open(path, O_RDONLY);
    if (fd > 0)
    {
        while(1)
        {
            char buffer[512] = { 0 };
            int r = read(fd, buffer, 512);
            if (r <= 0)
                break;
            write(STDOUT_FILENO, buffer, r);
        }
        close(fd);
    }
    else
    {
        write(STDERR_FILENO, "Error: can't open man for the command: ", 39);
        write(STDERR_FILENO, command, n);
        write(STDERR_FILENO, "\n", 1);
    }

    return;
}

void man(int argc, char** argv, char* path)
{
    if (argc < 1)
    {
        write(STDERR_FILENO, "Error with the man command", 26);
        return;
    }

    for (int i = 1; i < argc; i++)
    {
        int k = strlen(argv[i]);
        man_command(path, argv[i], k);
    }

}
