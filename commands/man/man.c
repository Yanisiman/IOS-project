#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void man_command(char* command, int n)
{
    char path[512] = { 0 };
    sprintf(path, "commands/%s/man", command);
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
        write(STDOUT_FILENO, "Error: can't open find the command: ", 36);
        write(STDOUT_FILENO, command, n);
        write(STDOUT_FILENO, "\n", 1);
    }

    return;
}

void man(int argc, char** argv)
{
    if (argc < 1)
    {
        write(STDOUT_FILENO, "Error", 5);
        return;
    }

    int k = strlen(argv[1]);
    man_command(argv[1], k);

}
