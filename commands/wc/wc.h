#ifndef WC_H
#define WC_H

#include <dirent.h>


struct count 
{
    int lines;
    int words;
    int letter;
};


void count_lines(int *count, char *buf, ssize_t r);

void count_words(int *count, char *buf, ssize_t r);

void printwc(int count);

int wc(int argc, char **argv);
struct count wcecho(int fd_in);
#endif
