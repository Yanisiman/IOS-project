#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <string.h>

void listdir(char *pwd, int indent, int recursive, int info)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(pwd))){
        write(STDOUT_FILENO, "Error: can't open the file or directory\n", 40);
        return;
    }

    struct dirent dirs[512];
    int d=0;
    struct dirent files[512];
    int f=0;

    while((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                dirs[d]=*entry;
                d++;
            }
        }
        else
        {
            if (entry->d_name[0]!= '.' && strcmp(entry->d_name, "description.txt") !=0)
            {
                files[f]=*entry;
                f++;;
            }
        }
    }

    write(STDOUT_FILENO, "\033[0;32m", 7);
    write(STDOUT_FILENO, "Locations :\n", 12);
    write(STDOUT_FILENO, "\033[0m", 4);
    for(int j=0;j<d;j++)
    {
        char path[512];
        struct dirent *direst;
        direst=&dirs[j];

        struct stat file_stat;
        char abspath[512];
        sprintf(abspath,"%s/%s", pwd, direst->d_name);
        ssize_t length = strlen(direst->d_name);
        if (stat(abspath, &file_stat) < 0)
        {
            write(STDOUT_FILENO, "Error with stats of the file ", 29);
            write(STDOUT_FILENO, direst->d_name, length);
            write(STDOUT_FILENO, "\n", 1);
            return;
        }

        char perms[10];
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



        snprintf(path, sizeof(path), "%s/%s", pwd, direst->d_name);
        write(STDOUT_FILENO, "\033[0;31m", 7);
        printf("%*s%s[%s]\n", indent, "", info ? perms : "", direst->d_name);
        write(STDOUT_FILENO, "\033[0m", 4);
        if (recursive)
            listdir(path, indent + 2, recursive, info);
    }

    write(STDOUT_FILENO, "\033[0;32m", 7);
    write(STDOUT_FILENO, "\nItems :\n", 9);
    write(STDOUT_FILENO, "\033[0m", 4);
    for(int i=0;i<f;i++)
    {
        struct dirent *filest;
        filest=&files[i];

        struct stat file_stat;
        char abspath[512];
        sprintf(abspath,"%s/%s", pwd, filest->d_name);
        ssize_t length = strlen(filest->d_name);
        if (stat(abspath, &file_stat) < 0)
        {
            write(STDOUT_FILENO, "Error with stats of the file ", 29);
            write(STDOUT_FILENO, filest->d_name, length);
            write(STDOUT_FILENO, "\n", 1);
            return;
        }

        char perms[10];
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


        write(STDOUT_FILENO, "\033[0;34m", 7);
        printf("%*s- %s %s\n", indent, "", info ? perms : "", filest->d_name);
        write(STDOUT_FILENO, "\033[0m", 4);
    }

    closedir(dir);
    write(STDOUT_FILENO, "\n", 1);
}


void ls(int argc, char** argv)
{
    if (argc < 1)
        write(STDOUT_FILENO, "An error occured\n", 17);

    int recursive = 0, info = 0;
    char tmp[256];
    char *pwd = getcwd(tmp, sizeof(tmp));
    int i = 1;
    int k = argc - 1;

    if (argc > 1 && argv[1][0] == '-')
    {
        if (strcmp(argv[1],"-l") == 0)
            info = 1;
        if (strcmp(argv[1],"-R") == 0)
            recursive = 1;
        if (strcmp(argv[1],"-lR") == 0 || strcmp(argv[1],"-Rl") == 0)
        {
            recursive = 1;
            info = 1;
        }
        i++;
        k--;
    }


    if (k == 0)
    {
        listdir(pwd, 2, recursive, info);
    }
    else
    {
        for (int j; j < k; j++)
        {
            listdir(argv[i], 2, recursive, info);
            i++;
        }
    }
}

void simple_ls()
{
    char tmp[256];
    char *pwd = getcwd(tmp, sizeof(tmp));

    listdir(pwd, 2, 0, 0);

}
