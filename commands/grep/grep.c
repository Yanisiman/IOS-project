#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <string.h>

#define BUFF 512

void read_from_in(char* pattern)
{
    char buffer[BUFF] = {0};
    ssize_t r = read(STDIN_FILENO, buffer, BUFF);
    int offset;
    int w;
    while (r != 0)
    {
        if (r == -1)
            err(EXIT_FAILURE, "Error while reading input of grep");
        if (strstr(buffer, "quit\n") != NULL)
            return;
        char *tmp = calloc(BUFF, sizeof(char));
        strcpy(tmp, buffer);
        if (strstr(tmp, pattern) != NULL)
        {
            offset = 0;
            while (r > 0)
            {
                w = write(STDOUT_FILENO, buffer + offset, r);
                if (w == -1)
                    err(EXIT_FAILURE, "Error while writing for grep");
                offset += w;
                r -= w;
            }
        }
        free(tmp);
        memset(buffer, 0, BUFF);
        r = read(STDIN_FILENO, buffer, BUFF);
    }
}

char *read_to_string(int fd)
{
    char buffer[BUFF] = {0};
    char *file = calloc(BUFF, sizeof(char));
    int alloc = BUFF;
    int i = 0;
    int r = read(fd, buffer, BUFF);
    while (r != 0)
    {
        if (r == -1)
            err(EXIT_FAILURE, "Error while reading");

        for (int j = 0; j < r; j++)
        {
            file[j+i] = buffer[j];
            if (j+i == alloc - 1)
            {
                int old = alloc;
                alloc *= 2;
                file = realloc(file, alloc * sizeof(char));
                if (file == NULL)
                    errx(EXIT_FAILURE, "Error trying to realloc for grep string");
                for (int l = old; l < alloc; l++)
                    file[l] = 0;
            }
        }
        i += r;
        r = read(fd, buffer, BUFF);
    }
    return file;
}

char **string_to_lines(char *file, int *count)
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
        results[i] = calloc(strlen(parsed) + 1, sizeof(char));
        strcpy(results[i], parsed);
        i++;
        parsed = strtok(NULL, sep);
    }

    *count = i;

    return results;
}


char **find_pattern(char **lines, char *pattern, int *count, int k)
{
    int alloc = 15;
    char **results = calloc(alloc, sizeof(char *));
    if (results == NULL)
        errx(EXIT_FAILURE, "Error trying to calloc for find_patteern");

    int i = 0;
    char *parsed;

    for (int j = 0; j < k; j++)
    {
        char *s = lines[j];
        char *save = calloc(strlen(lines[j]) + 1, sizeof(char));
        strcpy(save, lines[j]);
        parsed = strstr(s, pattern);

        if (parsed != NULL)
        {
            if (i == alloc - 1)
            {
                alloc *= 2;
                results = realloc(results, alloc * sizeof(char *));
                if (results == NULL)
                    err(EXIT_FAILURE, "Error trying to realloc");
            }
            results[i] = calloc(strlen(save) + 1, sizeof(char));
            strcpy(results[i], save);
            i++;
        }
        free(save);
    }

    *count = i;

    return results;
}


int grep(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Error: Arguments are missing\n");
        return EXIT_FAILURE;
    }

    char *pattern = argv[1];
    int fd;
    if (argc == 2)
    {
        read_from_in(pattern);
    }
    else
    {
        fd = open(argv[2], O_RDONLY);
        if (fd < 0)
        {
            printf("Error: can't find any file %s\n", argv[2]);
            return EXIT_FAILURE;
        }


        char *file = read_to_string(fd);
        int count = 0;
        int k = 0;

        char **lines = string_to_lines(file, &k);

        char **results = find_pattern(lines, pattern, &count, k);
        for (int i = 0; i < count; i++)
        {
            printf("%s\n", results[i]);
        }

        free(file);
        for (int i = 0; i < k; i++)
            free(lines[i]);
        free(lines);
        for (int i = 0; i < count; i++)
            free(results[i]);
        free(results);
    }

    return EXIT_SUCCESS;
}
