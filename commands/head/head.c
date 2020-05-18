#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include "../grep/grep.h"

void print_first_lines(char *file, int count)
{
    int i = 0;
    char *parsed;
    char *p = file;
    char *sep = "\n";
    parsed = strtok(p, sep);

    while(parsed != NULL && i != count)
    {
        if (strcmp(parsed, " ") != 0)
        {
            write(STDOUT_FILENO, parsed, strlen(parsed));
            write(STDOUT_FILENO, "\n", 1);
            i++;
        }
        parsed = strtok(NULL, sep);
    }

}

int head(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Error: Arguments are missing\n");
        return EXIT_FAILURE;
    }

    int n = 10;
    int f = 1;
    if (strcmp(argv[1], "-n") == 0 && argc > 3)
    {
        n = atoi(argv[2]);
        if (n == 0)
        {
            printf("Error: Bad arguments given\n");
            return EXIT_FAILURE;
        }
        f = 3;
    }
    for (int i = f; i < argc; i++)
    {
        int fd = open( argv[i], O_RDONLY);
        if (fd < 0)
        {
            printf("Error: can't find any file %s\n", argv[i]);
            return EXIT_FAILURE;
        }
        printf("==> %s <==\n", argv[i]);
        char *file = read_to_string(fd);
        print_first_lines(file, n);
        printf("\n");
        free(file);
    }

    return EXIT_SUCCESS;
}
