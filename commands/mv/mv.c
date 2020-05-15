#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include <errno.h>
#include <stdio.h>

int normalmv (char **argv){
   char copy[256];
   strcpy(copy,argv[2]);
   strcat(copy,"/");
   strcat(copy,argv[1]);
   int j=rename(argv[1], copy);

        if(j==0){
             printf("the %s is moved to %s\n", argv[1], copy);
         return 0;
        }else{
         perror("Error while mv");
         return -1;
        }
}


int forcemv (char **argv){
   char copy[256];
   strcpy(copy,argv[3]);
   strcat(copy,"/");
   strcat(copy,argv[2]);
   int j=rename(argv[2], copy);

        if(j==0){
        printf("the %s is moved to %s\n", argv[2], copy);
         return 0;
        }else{
          perror("Error while mv");
         return -1;
        }
}

int renamemv (char **argv){
   int j=rename(argv[2],argv[3]);
        if(j==0){
        printf("the %s is renamed to %s\n", argv[2], argv[3]);
         return 0;
        }else{
          perror("Error while mv");
         return -1;
        }
}


int intermv (char **argv){
   char copy[256];
   strcpy(copy,argv[3]);
   strcat(copy,"/");
   strcat(copy,argv[2]);

 if(access(copy,F_OK)!=-1){
  write(1,"the destination already exist do you want to overwrite it? [y/n]\n", 66);
  char respuesta[1];
  read(0,respuesta,1);
  if(respuesta[0]=='y'){
   int m=forcemv(argv);
   return m;
 }else{
   write(1,"The mv command was canceled \n", 30);
   return -1;
}
}else{
int m=forcemv(argv);
return m;
}
}



int noclobmv (char **argv){
   char copy[256];
   strcpy(copy,argv[3]);
   strcat(copy,"/");
   strcat(copy,argv[2]);

 if(access(copy,F_OK)!=-1){
  write(1,"the destination already exist \n", 32);
 return -1;
}else{
int m=forcemv(argv);
return m;
}
 }


int mv (int argc,char **argv){   
       int n=0;
    if(argc==4){
         if(argv[1][0]=='-') {
                  if(argv[1][1]=='f'){

                  n=forcemv(argv);
                  return n;
                  }else if(argv[1][1]=='i'){
 
                  n=intermv(argv);  
                  return n;
                  }else if(argv[1][1]=='n'){

                  n=noclobmv(argv);
                  return n;
                  }else if(argv[1][1]=='r'){

                  n=renamemv(argv);
                  return n;
                  }else{

                  printf("Enter a valid option\n");
                  return -1;
                  }
      }else{
         printf("Enter a valid number of parameters\n");
         return -1;
    }
    }else if(argc==3){
        n=normalmv(argv);
        return n;
    }else{
       printf("Enter a valid number of parameters\n");
        return -1;
    }

}












