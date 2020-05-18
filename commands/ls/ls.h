#ifndef LS_H
#define LS_H

#include <dirent.h>

struct flags 
{
    int recursive;
    int hidden;
    int info;
};

struct other
{
    int indent;
    int multiple;
};

void listdir(char *pwd, struct other others, struct flags flag);

void ls(int argc, char** argv);

void simple_ls(char *file);

void display_directories(char *pwd, struct other others, struct flags flag, int d, struct dirent *dirs);

void display_files(char *pwd, struct other others, struct flags flag, int f, struct dirent *files);

char *get_stats(char *pwd, char* name);

#endif
