#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include "chmod.h"

mode_t perm(enum class_ c, enum permission p)
{
    int temp = ((3-p) + (2-c)*3 - 1);

    return (mode_t) (1 << temp);
}

mode_t mode_contains(mode_t mode, enum class_ c, enum permission p)
{
    return mode & perm(c, p);
}

mode_t mode_add(mode_t mode, enum class_ c, enum permission p)
{
    return (mode_t) (mode | perm(c, p));
}

mode_t mode_rm(mode_t mode, enum class_ c, enum permission p)
{
    return (mode_t) (mode & ~perm(c, p));
}


void mode_update(mode_t *mode, char *sep, int m, char* perms, enum class_ c)
{
    enum permission p;
    for (int h = 0; h < m; h++)
    {
        if (perms[h] == 'r')
            p = PERMISSION_READ;
        else if (perms[h] == 'w')
            p = PERMISSION_WRITE;
        else
            p = PERMISSION_EXECUTE;

        if (strcmp(sep,"+") == 0)
            *mode = mode_add(*mode, c, p);
        else
            *mode = mode_rm(*mode, c, p);
    }
}


int chmod_command(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Error: arguments missing\n");
        return EXIT_FAILURE;
    }

    mode_t mode;
    if (argv[1][0] >= 48 && argv[1][0] <= 57)
    {
        mode = strtol(argv[1], 0, 8);
        for (int i = 2; i < argc; i++)
        {
            char *file = argv[i];
            if (chmod(file, mode) < 0)
                printf("Error: can't change the permissions for the file\n");
        }

    }
    else
    {
        int operator = 0;
        char *class = calloc(10, sizeof(char));
        char *perms = calloc(10, sizeof(char));
        char *sep;

        char *option = argv[1];
        int k = strlen(option);
        for (int l = 0; l < k; l++)
            operator = option[l] == '+' ? operator + 1 :
                option[l] == '-' ? operator - 1 : operator;
        if (operator == 0)
        {
            printf("Error");
            return EXIT_FAILURE;
        }
        if (operator == 1)
            sep = "+";
        else
            sep = "-";

        char *copy = calloc(k + 5, sizeof(char));
        strcpy(copy, option);
        char *one = strtok(copy, sep);
        char *two = strtok(NULL, sep);


        if (two == NULL)
            strcpy(perms, one);
        else
        {
            strcpy(class, one);
            strcpy(perms, two);
        }

        int n = strlen(class);
        int m = strlen(perms);

        struct stat stats;
        enum class_ c;
        for (int f = 2; f < argc; f++)
        {
            if (stat(argv[f], &stats) < 0)
            {
                printf("Error: can't find the stats for the file\n");
                continue;
            }

            mode = stats.st_mode;

            for (int i = 0; i < n; i++)
            {
                if (class[i] == 'u')
                    c = CLASS_OWNER;
                else if (class[i] == 'g')
                    c = CLASS_GROUP;
                else
                    c = CLASS_OTHER;
                mode_update(&mode, sep, m, perms, c);
            }

            if (n == 0)
            {
                enum class_ r[3] = {CLASS_OWNER, CLASS_GROUP, CLASS_OTHER};
                for (int l = 0; l < 3; l++)
                {
                    c = r[l];
                    mode_update(&mode, sep, m, perms, c);
                }
            }

            if (chmod(argv[f], mode) < 0)
                printf("Error: chmod for %s with %04o", argv[f], mode);
        }

        free(class);
        free(perms);

    }

    return EXIT_SUCCESS;
}
