#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>


int touch (int argc,char **argv)
{
    if(argc>0){

        mode_t mode= S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

        int file=open(argv[1],O_RDONLY | O_CREAT ,mode);

        if(file < 0){
            printf("Error occured doing touch command\n");
            return -1;
        }
        close(file);
        return 0;
    }
    return -1;
}
