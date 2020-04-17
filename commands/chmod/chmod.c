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


int chmod_command(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Error: arguments missing\n");
        return EXIT_FAILURE;
    }

    mode_t mode;
    char *parsed;
    if (argv[1][0] >= 48 && argv[1][0] <= 57)
    {
        mode = strtol(argv[1], 0, 8);
        for (int i = 2; i < argc; i++)
        {
            char *file = argv[i];
            /*
               pthread_t thread;
               int e = ptread_creat(&thread, NULL, permissions, file, mode);
             */
            if (chmod(file, mode) < 0)
                printf("Error: can't change the permissions for the file\n");
        }

    }
    else
    {
        int k = 0;
        int j = 0;
        char *class = calloc(10, sizeof(char));
        char *perms = calloc(10, sizeof(char));
        char *sep;

        while (!k)
        {
            char *copy = calloc(strlen(argv[1]) + 5, sizeof(char));
            strcpy(copy, argv[1]);
            sep = j == 0 ? "+" : "-" ;
            parsed = strtok(copy, sep);
            while (parsed)
            {
                if (k == 0)
                    strcpy(class, parsed);
                else
                    strcpy(perms, parsed);
                k++;
                parsed = strtok(NULL, sep);
            }
            free(copy);
            k = k == 2 || j == 1;
            j++;
        }

        int n = strlen(class);
        int m = strlen(perms);

        struct stat stats;
        enum class_ c;
        enum permission p;

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
                for (int h = 0; h < m; h++)
                {
                    if (perms[h] == 'r')
                        p = PERMISSION_READ;
                    else if (perms[h] == 'w')
                        p = PERMISSION_WRITE;
                    else
                        p = PERMISSION_EXECUTE;

                    if (strcmp(sep,"+") == 0)
                        mode = mode_add(mode, c, p);
                    else
                        mode = mode_rm(mode, c, p);
                }
            }

            if (n == 0)
            {
                enum class_ r[3] = {CLASS_OWNER, CLASS_GROUP, CLASS_OTHER};
                for (int l = 0; l < 3; l++)
                {
                    c = r[l];
                    for (int h = 0; h < m; h++)
                    {
                        if (perms[h] == 'r')
                            p = PERMISSION_READ;
                        else if (perms[h] == 'w')
                            p = PERMISSION_WRITE;
                        else
                            p = PERMISSION_EXECUTE;

                        if (strcmp(sep,"+") == 0)
                            mode = mode_add(mode, c, p);
                        else
                            mode = mode_rm(mode, c, p);
                    }
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
