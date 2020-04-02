#include <stdio.h>
#include <unistd.h>
#include <string.h> 


int mv (int argc,char **argv)
{   
    if(argc<3){
        printf("Enter a valid number of parameters\n");
        return -1;
    }else{
        //int j=rename(argv[1], argv[2]);
        //if(j==0){
          //  printf("the %s is moved to %s\n", argv[1], argv[2]);
          //  return 0;
       // }else{
            char copy[34];
            strcpy(copy,argv[2]);
            strcat(copy,"/");
            strcat(copy,argv[1]);
            int j=rename(argv[1], copy);
            if(j==0){
                printf("the %s is moved to %s\n", argv[1], copy);
                return 0;
            }else{
                printf("Error while moving mv\n");
                return -1;
            }
        //}
    }
}
