#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>

void help()
{
    write(STDOUT_FILENO, "\t-- cd: Change the current directory and print a description of the directory we are moving in\n", 93);
    write(STDOUT_FILENO, "\t-- less: Print the text in a file\n", 33);
    write(STDOUT_FILENO, "\t-- ls: Print the Locations (folders) and Items(files)\n", 53);
    write(STDOUT_FILENO, "\t-- pwd: Print the current working directory\n", 43);
    write(STDOUT_FILENO, "", 0);
    write(STDOUT_FILENO, "", 0);
    write(STDOUT_FILENO, "", 0);
}
