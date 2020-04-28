#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFERSIZE 512

int ln(int argc, char* argv[]){
	
	int i, a;
	struct stat s;
	char cwd[BUFFERSIZE] = { 0 };
	
	if (argc < 3){
		perror("Not enough arguments given.");
		exit(1);
	}else if(argc > 4){
		perror("Too many arugments given.");
		exit(1);
	}else if(argc == 3){
		i = 0; // filename position
	}else{
		i = 1; // filename position
	}
	
	
	if(argc == 4 && strcmp(argv[1],"-s") != 0){
		perror("Only symbolic link option '-s' is implemented.");
		exit(1);
	}
	
	if(access(argv[i+1],F_OK)){
		perror("Given file name is not found.");
		exit(1);
	}
	
	if(!access(argv[i+2],F_OK)){
		perror("Given file name already exists.");
		exit(1);
	}
	
	a = stat(argv[i+1],&s);
	
	if(a < 0){
		perror("Unable to get stat information.");
		exit(1);
	}else{
		if(!S_ISREG(s.st_mode)){
		  perror("Given file is not a regular file.");
		  exit(1); 
		}
	}
	
    getcwd(cwd, sizeof(cwd));
	strcat(cwd,"/");
    strcat(cwd,argv[i+1]);
	
	if(argc == 3){
		if(link(cwd,argv[i+2])<0){
			perror("Unable to create a hard link");
			exit(1);
		}
	}else if(argc == 4){
		if(symlink(cwd,argv[i+2])<0){
			perror("Unable to create a symbolic link");
			exit(1);
		}
	}

	return 0;
}
