#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>

void help()
{
    write(STDOUT_FILENO, "--cd <directory>: Change the current directory and print a description of the directory we are moving in\n", 105);
    write(STDOUT_FILENO, "--less <file>: Print the text in a file\n", 40);
    write(STDOUT_FILENO, "--ls: Print the Locations (folders) and Items(files)\n", 53);
    write(STDOUT_FILENO, "--pwd: Print the current working directory\n", 43);
    write(STDOUT_FILENO, "--rm <file/directory>: removes the file or directroy which can be deleted on game\n",82);
    write(STDOUT_FILENO, "--mv <which> <where>: Moves the file to the selected directory\n", 63);
}
