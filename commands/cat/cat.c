#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>

#define BUFFER_SIZE 512

void cat_(int fd_in, int fd_out)
{
    char buff[BUFFER_SIZE] = { 0 };
    ssize_t r = read(fd_in, buff, BUFFER_SIZE);
    int offset = 0;
    int w;
    while (r != 0)
    {
        if (r == -1)
            err(EXIT_FAILURE, "Error while reading");
        if (strstr(buff, "quit\n") != NULL)
            return;
        offset = 0;
        while (r > 0)
        {
            w = write(fd_out, buff + offset, r);
            if (w == -1)
                err(EXIT_FAILURE, "Error while writing");
            offset += w;
            r -= w;
        }
        r = read(fd_in, buff, BUFFER_SIZE);
    }
}

int cat(int argc, char **argv)
{
    if (argc < 1)
    {
        write(STDERR_FILENO, "An error occured\n", 17);
        return EXIT_FAILURE;
    }

    // Echo mode.
    // Copy the standard input to the standard output.
    if (argc == 1)
        cat_(STDIN_FILENO, STDOUT_FILENO);

    // Print mode.
    // If one parameter is passed,
    // print the contents of the file.
    else
    {
        for (int i = 1; i < argc; i++)
        {
            int fd = open(argv[i], O_RDONLY);
            if (fd == -1)
                write(STDOUT_FILENO, "Error: can't find the file\n", 23); 
            cat_(fd, STDOUT_FILENO);

            close(fd);
            write(STDOUT_FILENO, "\n", 1);
        }
    }

    return EXIT_SUCCESS;
}
