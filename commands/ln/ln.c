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
		write(STDOUT_FILENO, "Not enough arguments given.\n", 28);
        return EXIT_FAILURE;
		
	}else if(argc > 4){
		write(STDOUT_FILENO, "Too many arugments given.\n",26);
		return EXIT_FAILURE;
	}else if(argc == 3){
		i = 0; // filename position
	}else{
		i = 1; // filename position
	}
	
	
	if(argc == 4 && strcmp(argv[1],"-s") != 0){
		write(STDOUT_FILENO, "Only symbolic link option '-s' is implemented.\n", 47);
		return EXIT_FAILURE;
	}
	
	if(access(argv[i+1],F_OK)){
		write(STDOUT_FILENO, "Given file name is not found.\n", 30);
		return EXIT_FAILURE;
	}
	
	if(!access(argv[i+2],F_OK)){
		write(STDOUT_FILENO, "Given file name already exists.\n", 32);
		return EXIT_FAILURE;
	}
	
	a = stat(argv[i+1],&s);
	
	if(a < 0){
		write(STDOUT_FILENO, "Unable to get stat information.\n", 32);
		return EXIT_FAILURE;
	}else{
		if(!S_ISREG(s.st_mode)){
		  write(STDOUT_FILENO, "Given file is not a regular file.\n", 34);
		  return EXIT_FAILURE;
		}
	}
	
    getcwd(cwd, sizeof(cwd));
	strcat(cwd,"/");
    strcat(cwd,argv[i+1]);
	
	if(argc == 3){
		if(link(cwd,argv[i+2])<0){
			write(STDOUT_FILENO,"Unable to create a hard link.\n",29);
			return EXIT_FAILURE;
		}
	}else if(argc == 4){
		if(symlink(cwd,argv[i+2])<0){
			write(STDOUT_FILENO, "Unable to create a symbolic link.\n", 34);
			return EXIT_FAILURE;
		}
	}

	return 0;
}
