#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFF_SIZE 4096

int copy_file_to_file(int src, int dst)
{
    char buffer[BUFF_SIZE] = { 0 };
    int w, offset;
    int r = read(src, buffer, BUFF_SIZE);
    while (r != 0)
    {
        if (r == -1)
        {
            return -1;
        }
        offset = 0;
        while (r > 0)
        {
            w = write(dst, buffer + offset, r);
            if (w == -1)
            {
                return -1;
            }
            offset += w;
            r -= w;
        }
        r = read(src, buffer, BUFF_SIZE);
    }

    return EXIT_SUCCESS;
}

int copy_file_to_directory()
{

    return EXIT_SUCCESS;
}

int copy_directory()
{

    return EXIT_SUCCESS;
}

int cp(int argc, char **argv)
{
    if (argc < 3)
    {
        write(STDOUT_FILENO, "An operand is missing\n", 22);
        return EXIT_FAILURE;
    }

    char *source;
    char *dest;
    int i = 1;
    for (; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            source = argv[i];
            break;
        }
    }
    dest = argv[++i];

    int src = open(source, O_RDONLY);
    if (src < 0)
    {
        write(STDOUT_FILENO, "Error with the source\n", 22);
        return EXIT_FAILURE;
    }
    int dst = open(dest, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (dst < 0)
    {
        close(src);
        write(STDOUT_FILENO, "Error with the source\n", 22);
        return EXIT_FAILURE;
    }

    struct stat src_stats;
    struct stat dst_stats;
    if (fstat(src, &src_stats) < 0 || fstat(dst, &dst_stats) < 0)
    {
        printf("Error with stats\n");
        return EXIT_FAILURE;
    }

    if (copy_file_to_file(src, dst) < 0)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;

}
