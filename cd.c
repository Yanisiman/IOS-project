#include <stdio.h> //printf
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> 

#define BUFFERSIZE 512


int tellDescription(){
	int filedesc = open("description.txt", O_RDONLY);
    if(filedesc < 0){
		printf("%s\n", "an error occured reading description"); 
	}else{
		
		char message[BUFFERSIZE];
		char n;
		int desc = read(filedesc, message, BUFFERSIZE);
		message[desc] = '\0'; 
		write(1, message, desc );
	}
	return 0;
}
int main(int argc, char *argv[]) 
{    
	
	char s[BUFFERSIZE]; 
	
    printf("%s\n", getcwd(s, BUFFERSIZE));     // printing current working directory 
    int a; // for indicating if cd command was success
	char path[BUFFERSIZE];
	path[0]='\0';
	
	//going to root with no args
	if(argc < 2){
		chdir(getenv("HOME"));
		printf("%s\n", "this works"); 
		printf("%s\n", getcwd(s, BUFFERSIZE));
		return 0;	
	}
	
    strcat(path, argv[1]);
    char cwd[BUFFERSIZE];
	
    if(path[0] != '/'){
		
		// true for the dir in cwd
        getcwd(cwd,sizeof(cwd));
        strcat(cwd,"/");
        strcat(cwd,path);
		
        a = chdir(cwd);
		tellDescription();
		
		if(a < 0){
			printf("%s\n", "an error occured performing cd"); 
		}
		
	}else{
	
		a = chdir(path);
		tellDescription();
		
		if(a < 0){
			printf("%s\n", "an error occured performing cd"); 
		}
    }
	 printf("%s\n", getcwd(s, BUFFERSIZE)); 
	
    return 0; 
} 
