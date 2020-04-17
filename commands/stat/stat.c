#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

char *format_date(char* str, time_t val)
{
    strftime(str, 36, "%d.%m.%Y %H:%M:%S", localtime(&val));
    return str;
}

int stat_(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Wrong arguments\n");
        return EXIT_FAILURE;
    }

    struct stat info;
    int stats = stat(argv[1], &info);
    if (stats == -1)
    {
        printf("The file %s can't be found\n", argv[1]);
        return EXIT_FAILURE;
    }

    char *mode = calloc(10, sizeof(char));;
    sprintf(mode, "%03o", info.st_mode);
    int l = strlen(mode) - 4;
    mode += l;

    printf("\tID of device containing file: %ld\n", info.st_dev);
    printf("\tInode number: %ld\n", info.st_ino);
    printf("\tFile type and mode: %s\n", mode);
    printf("\tNumber of hard links: %ld\n", info.st_nlink);
    printf("\tUser ID of owner: %d\n", info.st_uid);
    printf("\tGroup ID of owner: %d\n", info.st_gid);
    printf("\tDevice Id: %ld\n\n", info.st_rdev);
    printf("\tTotal size in bytes: %ld\n", info.st_size);
    printf("\tBlock size for filesystem I/O: %ld\n", info.st_blksize);
    printf("\tNumber of 512B blocks allocated: %ld\n\n", info.st_blocks);

    char date[36];
    printf("\tTime of last access: %s\n", format_date(date, info.st_atim.tv_sec));
    printf("\tTime of last modification: %s\n", format_date(date, info.st_mtim.tv_sec));
    printf("\tTime of last status change: %s\n", format_date(date, info.st_ctim.tv_sec));

    mode -= l;
    free(mode);
    return 0;
}
