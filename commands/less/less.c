#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> 
#include <err.h>
#include <sys/stat.h>
#define BUFFERSIZE 512

int less(int argc, char *argv[])
{
  	struct stat file_info; 
    stat(argv[1], &file_info);
	
	if (argc < 2 || (S_ISDIR(file_info.st_mode))){ 
		
        write(STDOUT_FILENO, "Pick an item to less.\n", 22);
        return EXIT_FAILURE;
		
    }else if(access( argv[1], F_OK ) == -1){
		ssize_t k = strlen(argv[1]);
		write(STDOUT_FILENO, "There is no ", 12);
		write(STDOUT_FILENO, argv[1], k);
		write(STDOUT_FILENO, " here.\n", 7);
		return EXIT_FAILURE;
	}else{        
		int filedesc = open(argv[1], O_RDONLY);
		if(filedesc < 0){
			write(STDOUT_FILENO, "Error opening a file for reading.\n", 34);
			return EXIT_FAILURE;
		}else{
			char message[BUFFERSIZE] = { 0 };
			int desc = read(filedesc, message, BUFFERSIZE);
			if (desc < 0){
				write(STDOUT_FILENO, "Error reading an item.\n", 23);
				return EXIT_FAILURE;
			}
			write(STDOUT_FILENO, message, desc);
			close(filedesc);
		}	
	}
    
    write(STDOUT_FILENO, "\n", 1);
    return 0;
}
