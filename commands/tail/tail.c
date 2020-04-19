#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include "../grep/grep.h"

char **string_to_last_lines(char *file, int *count)
{
    int alloc = 15;
    char **results = calloc(alloc, sizeof(char *));
    if (results == NULL)
        errx(EXIT_FAILURE, "Error trying to calloc for find_patteern");

    int i = 0;
    char *parsed;
    char *p = file;
    char *sep = "\n";
    parsed = strtok(p, sep);

    while(parsed != NULL)
    {
        if (i == alloc - 1)
        {
            alloc *= 2;
            results = realloc(results, alloc * sizeof(char *));
            if (results == NULL)
                err(EXIT_FAILURE, "Error trying to realloc");
        }
        if (strcmp(parsed, " ") != 0)
        {
            results[i] = calloc(strlen(parsed) + 1, sizeof(char));
            strcpy(results[i], parsed);
            i++;
        }
        parsed = strtok(NULL, sep);
    }

    *count = i;

    return results;
}

int tail(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Error: Arguments are missing\n");
        return EXIT_FAILURE;
    }

    int n = 10;
    char *f = argv[1];

    if (strcmp(argv[1], "-n") == 0 && argc > 3)
    {
        n = atoi(argv[2]);
        if (n == 0)
        {
            printf("Error\n");
            return EXIT_FAILURE;
        }
        f  = argv[3];
    }
    int fd = open(f, O_RDONLY);
    if (fd < 0)
    {
        printf("Error: can't find any file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    char *file = read_to_string(fd);
    int count = 0;
    char **lines = string_to_last_lines(file, &count);

    for (int i = count - n; i < count; i++)
    {
        printf("%s\n", lines[i]);
    }

    free(file);
    for (int i = 0; i < count; i++)
        free(lines[i]);
    free(lines);

    return EXIT_SUCCESS;
}
