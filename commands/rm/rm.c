#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <sys/stat.h>

#define BUFF_SIZE 512


char** parse_in(char *buf)
{
    char **parse = calloc(8, sizeof(char *));
    if (parse == NULL)
        errx(EXIT_FAILURE, "Error trying to allocate memory for the parse_input");

    int args = 0;
    char *separator = " ";
    char *parsed;

    parsed = strtok(buf, separator);
    while (parsed != NULL)
    {
        parse[args] = parsed;
        args++;
        parsed = strtok(NULL, separator);
    }
    parse[args] = NULL;
    return parse;

}

int perform_rm(char *argv[]){

    struct stat file_info; //pointer to stat struct
    stat(argv[1], &file_info); //stat system call
    int a; // for indicating if rm command was success

    if (S_ISDIR(file_info.st_mode)) { // is a directory
        a = rmdir(argv[1]);
    }else{                            // is a file
        a = unlink(argv[1]);
    }

    if(a<0){
        write(STDOUT_FILENO, "Error occured doing rm command\n", 31);
        return -1;
    }
    return 0;
}
int rm(char *argv[]){

	if( access( argv[1], F_OK ) != -1 ) {
		
		char tmp[256];
		char *pwd = getcwd(tmp, sizeof(tmp));
		struct stat file_stat;
		stat(pwd, &file_stat);
		if( (file_stat.st_mode & S_IWUSR) &&
			(file_stat.st_mode & S_IWGRP) &&
			(file_stat.st_mode & S_IWOTH) ){
				perform_rm(argv);
			}else{
				write(STDOUT_FILENO, "Rm doesn't work on this.\n",25);
			}
		
		   
	
	} else {
		write(STDOUT_FILENO, "Wrong item name given.\n",23);
	}
	return 0;

}
