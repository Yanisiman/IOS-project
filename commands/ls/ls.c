#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include "ls.h"

void listdir(char *pwd, struct other others, struct flags flag)
{
    DIR *dir;
    struct dirent *entry;

    ssize_t k_pwd = strlen(pwd);

    if (!(dir = opendir(pwd))){
        //write(STDERR_FILENO, "Error: can't open the file or directory: ", 41);
        //write(STDERR_FILENO, pwd, k_pwd);
        //write(STDERR_FILENO, "\n", 1);
        simple_ls(pwd);
        return;
    }

    struct dirent dirs[512];
    int d=0;
    struct dirent files[512];
    int f=0;

    if (others.multiple)
    {
        write(STDOUT_FILENO, pwd, k_pwd);
        write(STDOUT_FILENO, "/\n", 1);
    }

    while((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            if (flag.hidden || (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0))
            {
                dirs[d]=*entry;
                d++;
            }
        }
        else
        {
            if ((flag.hidden || entry->d_name[0]!= '.') && strcmp(entry->d_name, "description.txt") !=0)
            {
                files[f]=*entry;
                f++;;
            }
        }
    }
    display_files(pwd, others, flag, f, files);
    display_directories(pwd, others, flag, d, dirs);
    closedir(dir);
    write(STDOUT_FILENO, "\n", 1);
}


void ls(int argc, char** argv)
{
    if (argc < 1)
    {
        write(STDERR_FILENO, "An error occured\n", 17);
        return;
    }

    struct flags flag = {0, 0, 0};
    struct other others = {2, 0};

    char tmp[256];
    char *pwd = getcwd(tmp, sizeof(tmp));
    int i = 1;
    int k = argc - 1;

    if (argc > 1 && argv[1][0] == '-')
    {
        int j = 1;
        int len = strlen(argv[1]);
        for (; j < len; j++)
        {
            if (argv[1][j] == 'l')
                flag.info = 1;
            else if (argv[1][j] == 'R')
                flag.recursive = 1;
            else if (argv[1][j] == 'a')
                flag.hidden = 1;
            else
            {
                char opt[1] = {argv[1][j]};
                write(STDERR_FILENO, "Error: option ", 14);
                write(STDERR_FILENO, opt, 1);
                write(STDERR_FILENO, " doesn't exit\n", 14);
                return;
            }
        }
        i++;
        k--;
    }


    if (k == 0)
    {
        listdir(pwd, others, flag);
    }
    else
    {
        others.multiple = k > 1;
        for (int j = 0; j < k; j++)
        {
            listdir(argv[i], others, flag);
            i++;
        }
    }
}

void simple_ls(char *file)
{
    char *perms = get_stats(".", file);

    write(STDOUT_FILENO, "\033[0;34m", 7);
    write(STDOUT_FILENO, perms, 10);
    printf(" %s\n", file);
    write(STDOUT_FILENO, "\033[0m", 4);
    free(perms);

    return;
}

void display_directories(char *pwd, struct other others, struct flags flag, int d, struct dirent *dirs)
{
    write(STDOUT_FILENO, "\033[0;32m", 7);
    write(STDOUT_FILENO, "Locations :\n", 12);
    write(STDOUT_FILENO, "\033[0m", 4);
    for(int j=0;j<d;j++)
    {
        char path[512];
        struct dirent *direst;
        direst=&dirs[j];

        char *perms = get_stats(pwd, direst->d_name);

        snprintf(path, sizeof(path), "%s/%s", pwd, direst->d_name);

        write(STDOUT_FILENO, "\033[0;31m", 7);
        for (int i = 0; i < others.indent; i++)
            write(STDOUT_FILENO, " ", 1);
        if (flag.info)
            write(STDOUT_FILENO, perms, 10);
        printf(" [%s]\n", direst->d_name);
        fflush(stdout);
        write(STDOUT_FILENO, "\033[0m", 4);
        free(perms);

        if (flag.recursive && strcmp(direst->d_name, ".") != 0 && strcmp(direst->d_name, "..") != 0)
        {
            others.indent += 2;
            listdir(path, others, flag);
        }
    }

}

void display_files(char *pwd, struct other others, struct flags flag, int f, struct dirent *files)
{
    write(STDOUT_FILENO, "\033[0;32m", 7);
    write(STDOUT_FILENO, "\nItems :\n", 9);
    write(STDOUT_FILENO, "\033[0m", 4);
    for(int i=0;i<f;i++)
    {
        struct dirent *filest;
        filest=&files[i];

        char *perms = get_stats(pwd, filest->d_name);

        write(STDOUT_FILENO, "\033[0;34m", 7);
        for (int i = 0; i < others.indent; i++)
            write(STDOUT_FILENO, " ", 1);
        if (flag.info)
            write(STDOUT_FILENO, perms, 10);
        printf(" %s\n", filest->d_name);
        fflush(stdout);
        write(STDOUT_FILENO, "\033[0m", 4);
        free(perms);
    }

}

char *get_stats(char *pwd, char *name)
{
    struct stat file_stat;
    char abspath[512];
    sprintf(abspath,"%s/%s", pwd, name);
    ssize_t length = strlen(name);
    if (stat(abspath, &file_stat) < 0)
    {
        write(STDOUT_FILENO, "Error with stats of the file ", 29);
        write(STDOUT_FILENO, name, length);
        write(STDOUT_FILENO, "\n", 1);
        return "";
    }

    char *perms = calloc(10, sizeof(char));
    perms[0] = (S_ISDIR(file_stat.st_mode)) ? 'd' : '-';
    perms[1] = (file_stat.st_mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (file_stat.st_mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (file_stat.st_mode & S_IXUSR) ? 'x' : '-';
    perms[4] = (file_stat.st_mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (file_stat.st_mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (file_stat.st_mode & S_IXGRP) ? 'x' : '-';
    perms[7] = (file_stat.st_mode & S_IROTH) ? 'r' : '-';
    perms[8] = (file_stat.st_mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (file_stat.st_mode & S_IXOTH) ? 'x' : '-';

    return perms;

}
