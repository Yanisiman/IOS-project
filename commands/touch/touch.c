#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>



int touch (int argc,char **argv)
{
    if(argc==2){

        mode_t mode= S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

        int file=open(argv[1],O_RDWR | O_CREAT ,mode);
        int time=utime(argv[1],NULL);
        
        if((file < 0)||(time < 0)){
            printf("Error occured doing touch command\n");
            return -1;
        }

        close(file);
        return 0;
    }
    printf("Error a valid number of parameters \n");
    return -1;
}
