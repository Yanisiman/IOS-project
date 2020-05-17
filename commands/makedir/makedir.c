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


    if (argc==1)
    {
        write(STDOUT_FILENO, "Error: Insert the name of the directory\n", 40);
    }
    else
    {   
        int j=1;
        int leer=0;
        mode_t mode=0777;
        if(argv[1][0] == '-'){
            j=2;
            if (strcmp(argv[1], "-v") == 0){
                    leer=1;
                }
            if (strcmp(argv[1], "-m") == 0){
                mode = strtol(argv[2], 0, 8);
                mkdir(argv[3], mode);
                return 0;
                }
        }    


        for(int i=j;i<argc;i++){
            int fd= mkdir(argv[i], 0777);
            if (fd ==-1) 
            {    
                write(STDOUT_FILENO, "Error: The directory already exists: ", 37);
                write(STDOUT_FILENO, argv[i], strlen(argv[i]));
                write(STDOUT_FILENO, "\n", 1);
            }
           if (leer==1 && fd ==0) {
                write(STDOUT_FILENO, "Directory created successfully: ", 32);
                write(STDOUT_FILENO, argv[i], strlen(argv[i]));
                write(STDOUT_FILENO, "\n", 1);
        }
    
        }


    }
    return 0;
}
