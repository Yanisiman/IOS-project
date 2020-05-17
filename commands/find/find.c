#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define BUFF 256

void find_file(char *path, int recursive, int folder, char *to_find)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(path)))
        return;

    while ((entry = readdir(dir)) != NULL)
    {
        char new_path[512] = {0};
        struct stat f_stat;

        sprintf(new_path, "%s/%s", path, entry->d_name);
        if (stat(new_path, &f_stat) < 0)
        {
            printf("Error: stat on %s", new_path);
            return;
        }

        if (entry->d_type == DT_DIR && (folder || recursive))
        {
            char p[512] = {0};
            if (strcmp(entry->d_name, ".") != 0 || strcmp(entry->d_name, "..") != 0)
            {
                snprintf(p, sizeof(p), "%s/%s", path, entry->d_name);
                find_file(p, recursive, folder, to_find);
            }
        }
        else
        {
            if (folder)
                printf("%s\n", new_path);
            else if (strcmp(to_find, entry->d_name) == 0)
                printf("%s\n", new_path);
        }
    }
    closedir(dir);
}


int find(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Error : Arguments missing\n");
        return EXIT_FAILURE;
    }

    int recursive = 0;
    int i = 1;
    if (strcmp(argv[1], "-name") == 0)
    {
        recursive = 1;
        i++;
    }

    for (; i < argc; i++)
    {
        char *f = argv[i];
        struct stat stats;
        int err = stat(f, &stats);
        if (err < 0 && !recursive)
        {
            printf("Something is wrong with the argument given\n");
            return EXIT_FAILURE;
        }

        int folder = 0;
        char path[BUFF] = {0};
        if (!S_ISDIR(stats.st_mode))
            getcwd(path, sizeof(path));
        else
        {
            strcpy(path, f);
            folder++;
        }

        find_file(path, recursive, folder, f);
    }

    return EXIT_SUCCESS;
}
