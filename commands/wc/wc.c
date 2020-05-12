#include <unistd.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void count_lines(int* count, char* buf, ssize_t r)
{
    for (ssize_t i = 0; i < r; i++){
        *count += (buf[i] == '\n');
    }       

}

void count_words(int* count, char* buf, ssize_t r)
{
    for (ssize_t i = 0; i < r; i++){

        *count += (buf[i] == ' ' && buf[i-1] != '\n' && buf[i-1] != ' ');
        *count += (buf[i] == '\n' && buf[i-1] !='\n' && buf[i-1] != ' ');

}
}


void printwc(int count)
{
    int digit = 1;
    int tmp = count;
    while ((tmp / 10) != 0)
    {
        digit++;
        tmp /= 10;
    }
    char* buf_tmp = malloc(digit * sizeof(char) + 1);
    tmp = count;
    buf_tmp[digit] = '\n';
    for (int i = digit - 1; i >= 0; i--)
    {
        buf_tmp[i] = '0' + tmp % 10;
        tmp /= 10;
    }
    int w = write(STDOUT_FILENO, buf_tmp, digit+1);
    if (w == -1)
        err(3, "Error while writing");
    free(buf_tmp);
}

void printvalor(int count)
{
    int digit = 1;
    int tmp = count;
    while ((tmp / 10) != 0)
    {
        digit++;
        tmp /= 10;
    }
    char* buf_tmp = malloc(digit * sizeof(char) + 1);
    tmp = count;
    buf_tmp[digit] = ' ';
    for (int i = digit - 1; i >= 0; i--)
    {
        buf_tmp[i] = '0' + tmp % 10;
        tmp /= 10;
    }
    int w = write(STDOUT_FILENO, buf_tmp, digit+1);
    if (w == -1)
        err(3, "Error while writing");
    free(buf_tmp);
}

int wc(int argc, char** argv)
{
//    if (argc < 2 || argc > 3 || (argc == 2 && argv[1][0] != '-'))
    if (argc < 2 || argc > 3 || (argc == 3 && argv[1][0] != '-'))
    {
        printf("Less or too many arguments given\n");
        return EXIT_FAILURE;
    }
//    int fd = argc == 2 ? STDIN_FILENO : open(argv[2], O_RDONLY);
    int fd;
    if (argc==3){
        fd =open(argv[2], O_RDONLY);
    }else{
        fd =open(argv[1], O_RDONLY);
    }

    if (fd == -1)
        err(EXIT_FAILURE, "Error with the input");

    char buf[255] = { 0 };
    int count = 0;
    int countw= 0;
    int countl=0;
    ssize_t r = read(fd, buf, 255);
    while(r != 0)
    {
        if (r == -1)
            err(3, "Error while reading");

        if (argc==3){
        if (strcmp(argv[1], "-m") == 0)
            count += r;
        if (strcmp(argv[1], "-w") == 0)
            count_words(&count, buf, r);
        if (strcmp(argv[1], "-l") == 0){
            count_lines(&count, buf, r);
        }
        }else{
            count += r;
            count_words(&countw, buf, r);
            count_lines(&countl, buf, r);    
        }

        r = read(fd, buf, 255);
    }
        if (argc ==3){
    //    count = strcmp(argv[1], "-m") == 0 ? count - 1 : count;
    //  count = strcmp(argv[1], "-l") == 0 ? count + 1 : count;
        printwc(count);
        }
        if (argc==2){
    //       count= count-1;
            printvalor(countl);
            printvalor(countw);
            printvalor(count);
            write(STDOUT_FILENO, argv[1], strlen(argv[1]));
            write(STDOUT_FILENO, "\n", 1);
        }
        

    //if (argc == 3 || argc == 2)
        close(fd);
    return 0;
}
