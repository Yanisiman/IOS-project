#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

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

int copy_file_to_directory(int src, char *source, char *dest)
{
    char path[BUFF_SIZE] = { 0 };
    sprintf(path, "%s/%s", dest, source);
    int dst = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    int ret = EXIT_SUCCESS;
    if (copy_file_to_file(src, dst) < 0)
        ret = EXIT_FAILURE;
    close(dst);
    return ret;
}

int copy_directories(char *source, char *dest, char *rec)
{
    DIR *dir;
    struct dirent *entry;
    if (!(dir = opendir(source)))
    {
        printf("Error: can't open the file or directory : %s\n", source);
        return EXIT_FAILURE;
    }
    char path[BUFF_SIZE] = { 0 };
    sprintf(path, "%s/%s", dest, rec);
    if (mkdir(path, 0755) < 0)
    {
        closedir(dir);
        return EXIT_FAILURE;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        char new_source[BUFF_SIZE] = { 0 };
        sprintf(new_source, "%s/%s", source, entry->d_name);

        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            copy_directories(new_source, path, entry->d_name);
        }
        else
        {
            int new_src = open(new_source, O_RDONLY);
            copy_file_to_directory(new_src, entry->d_name, path);
            close(new_src);
        }
    }
    closedir(dir);
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

    struct stat src_stats;
    struct stat dst_stats;

    int src = open(source, O_RDONLY);
    if (src < 0)
    {
        write(STDOUT_FILENO, "Error: with the source\n", 23);
        return EXIT_FAILURE;
    }

    int dst = open(dest, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (dst < 0 && stat(dest, &dst_stats) < 0)
    {
        close(src);
        write(STDOUT_FILENO, "Error: with the destination\n", 28);
        return EXIT_FAILURE;
    }

    if (fstat(src, &src_stats) < 0)
    {
        printf("Error with stats\n");
        close(src);
        close(dst);
        return EXIT_FAILURE;
    }

    int d_src = S_ISDIR(src_stats.st_mode);
    int d_dst = S_ISDIR(dst_stats.st_mode);

    int ret = EXIT_SUCCESS;

    if (!d_src && !d_dst)
    {
        if (copy_file_to_file(src, dst) < 0)
            ret = EXIT_FAILURE;
    }
    else if (!d_src && d_dst)
    {
        copy_file_to_directory(src, source, dest);
    }
    else if (d_src && d_dst)
    {
        copy_directories(source, dest, source);
    }
    else
    {
        printf("Error: can't cp directory to file\n");
        ret = EXIT_FAILURE;
    }

    close(src);
    close(dst);
    return ret;

}
