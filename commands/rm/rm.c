#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <sys/stat.h>

#define BUFF_SIZE 512

int perform_rm(char *argv){

    struct stat file_info; //pointer to stat struct
    stat(argv, &file_info); //stat system call
    int a; // for indicating if rm command was success

    if (S_ISDIR(file_info.st_mode)) { // is a directory
        a = rmdir(argv);
    }else{                            // is a file
        a = unlink(argv);
    }

    if(a<0){
        write(STDOUT_FILENO, "Error occured doing rm command\n", 31);
        return EXIT_FAILURE;
    }
    return 0;
}
int rm(int argc, char *argv[]){
	
	char tmp[256];
	char *pwd = getcwd(tmp, sizeof(tmp));
	struct stat file_stat;
	stat(pwd, &file_stat);
	if( (file_stat.st_mode & S_IWUSR) &&
		(file_stat.st_mode & S_IWGRP) &&
		(file_stat.st_mode & S_IWOTH) ){
		for(int i=1; i < argc; i++){
			if( access( argv[i], F_OK ) != -1 ) {
				perform_rm(argv[i]);
			}else {
				write(STDOUT_FILENO, "Wrong item name given.\n",23);
				return EXIT_FAILURE;
			}
		}
	}else{
		write(STDOUT_FILENO, "Rm doesn't work on this.\n",25);
		return EXIT_FAILURE;
		}
	return 0;
}
