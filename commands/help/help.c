#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>

void help()
{
    write(STDOUT_FILENO, "--cd: Change the current directory and print a description of the directory we are moving in\n", 93);
    write(STDOUT_FILENO, "--less: Print the text in a file\n", 33);
    write(STDOUT_FILENO, "--ls: Print the Locations (folders) and Items(files)\n", 53);
    write(STDOUT_FILENO, "--pwd: Print the current working directory\n", 43);

}
