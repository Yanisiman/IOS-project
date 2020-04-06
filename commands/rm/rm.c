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
        write(STDERR_FILENO, "Error occured doing rm command\n", 31);
        return -1;
    }
    return 0;
}
int rm(char *argv[]){

	if( access( argv[1], F_OK ) != -1 ) {

	
	if( strcmp(argv[1], "Vendor") == 0 ){
		
		write(STDOUT_FILENO, "Ha! That spell doesn’t work on everything, you know. "
		"I may have forgotten to mention that before I sold it to you…\n", 119);
		
	}else if( strcmp(argv[1], "strangeTrinket") == 0 ||
			  strcmp(argv[1], "clockworkDragon") == 0){
				  
		write(STDOUT_FILENO, "Didn’t your mother ever teach you that it’s rude to erase "
		      "other people’s things from their plane of existence?\n", 117);	
	
	}else if( strcmp(argv[1], "largeBoulder") == 0){
		
		perform_rm(argv);
		write(STDOUT_FILENO,  "The boulder disappears with a pop. The way is clear now.\n", 57);
		
	}else if(strcmp(argv[1],"earOfCorn") == 0){
		
		write(STDOUT_FILENO,"Why would you destroy a starving man’s only food?\n", 52);
		
	}else if( strcmp(argv[1], "thornyBrambles") == 0 ){
		
		write(STDOUT_FILENO,"You speak the words of the Remove spell and the brambles glimmer a deep blue." 
			  " They seem to be protected by a password.\n", 119);

		char **parse_password;
		
		char buf[BUFF_SIZE] = { 0 };
		ssize_t r = 1;
		r = read(0, buf, BUFF_SIZE);
		if (r == -1)
			errx(EXIT_FAILURE, "Error while reading");

		int i = 0;
		for (; buf[i] != '\0'; i++)
			continue;
		buf[i-1] = ' ';	
		parse_password = parse_in(buf);

		int argc = 0;
		for (; parse_password[argc]; argc++)
			continue;

		if (strcmp(parse_password[0], "brambles_b_gone") == 0){
			
			perform_rm(argv);
			write(STDOUT_FILENO, "The brambles catch fire, choking you with a thick black smoke."
				  " When the smoke clears, they’re gone.\n",102);
				  
		}else{
			
			write(STDOUT_FILENO,"Red sparks arc across the brambles before fizzling out. They are "
			      "definitely still there. Nice try though.\n",106);
				  
		}
	}else if(strcmp(argv[1],"uglyTroll") == 0 || 
			 strcmp(argv[1],"uglierTroll") == 0){
				 
				perform_rm(argv);
				write(STDOUT_FILENO,"The troll looks briefy surprised, then vanishes with an unpleasant"
				" squelching sound.\n",85);	 
				
	}else if(strcmp(argv[1],"absolutelyHideousTroll") == 0){
		
				write(STDOUT_FILENO,"The troll belches spectacularly, and you could swear he actually "
				      "smirks. You won’t get rid of him that easily, not without admin "
					  "privileges.\n",143);	 
	}else{
		write(STDOUT_FILENO, "Command 'rm' not found in this room\n",36);
	}
	} else {
		write(STDOUT_FILENO, "Wrong item name given.\n",23);
	}
	return 0;

}
