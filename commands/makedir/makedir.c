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



int makedir(int argc, char **argv){


       if (argc==1){
            write(STDOUT_FILENO, "Error: Insert the name of the directory\n", 40);           
    }else if(argc==2){

    int fd= mkdir(argv[1], 0777);
    

   if (fd ==-1) 
    { 
        write(STDOUT_FILENO, "Error: The directory already exists\n", 36);               
    }
   if (fd ==0) {
        write(STDOUT_FILENO, "Directory created successfully\n", 31);               
    }


}else{
            write(STDOUT_FILENO, "Error: Too many arguments\n", 26);               
}
return 0;
}
