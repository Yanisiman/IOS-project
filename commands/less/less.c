#include <stdio.h> //printf
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> 
#include <err.h>

#define BUFFERSIZE 512

int less(int argc, char *argv[])
{
    if (argc < 2)
    {
        write(STDOUT_FILENO, "Pick a different item to less\n", 30);
        return -1;
    }

    int filedesc = open(argv[1], O_RDONLY);
    if(filedesc < 0)
    {
        printf("There is no %s here", argv[1]);
        printf("\n");
        return -1;
    }
    else
    {
        while(1)
        {
            char message[BUFFERSIZE] = { 0 };
            int desc = read(filedesc, message, BUFFERSIZE);
            if (desc <= 0)
                break;
            write(STDOUT_FILENO, message, desc);
        }

        close(filedesc);
    }


    char newline[1] = {'\n'};
    write(STDOUT_FILENO, newline, 1);
    return 0;
}
