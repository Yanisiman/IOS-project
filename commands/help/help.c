#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>

void help()
{
    write(STDOUT_FILENO, "\033[0;33m", 7);
    write(STDOUT_FILENO, "  cat <file>: ", 14);
    write(STDOUT_FILENO, "\033[0m", 4);
    write(STDOUT_FILENO, "Print the content of a file\n", 28);
    write(STDOUT_FILENO, "\033[0;33m", 7);
    write(STDOUT_FILENO, "  cd <directory>: ", 18);
    write(STDOUT_FILENO, "\033[0m", 4);
    write(STDOUT_FILENO, "Change the current directory and print a description of the directory we are moving in. You can go back with cd -.\n", 115);
    write(STDOUT_FILENO, "\033[0;33m", 7);
    write(STDOUT_FILENO, "  cp <from> <to>: ", 18);
    write(STDOUT_FILENO, "\033[0m", 4);
    write(STDOUT_FILENO, "Copy a file into another file\n", 30);
    write(STDOUT_FILENO, "\033[0;33m", 7);
    write(STDOUT_FILENO, "  echo <something>: ", 20);
    write(STDOUT_FILENO, "\033[0m", 4);
    write(STDOUT_FILENO, "It will print <something> on screen\n", 36);
    write(STDOUT_FILENO, "\033[0;33m", 7);
    write(STDOUT_FILENO, "  less <file>: ", 15);
    write(STDOUT_FILENO, "\033[0m", 4);
    write(STDOUT_FILENO, "Print the text of a file\n", 25);
    write(STDOUT_FILENO, "\033[0;33m", 7);
    write(STDOUT_FILENO, "  ls: ", 6);
    write(STDOUT_FILENO, "\033[0m", 4);
    write(STDOUT_FILENO, "Print the Locations (folders) and Items(files)\n", 47);
    write(STDOUT_FILENO, "\033[0;33m", 7);
    write(STDOUT_FILENO, "  makedir <name>: ", 18);
    write(STDOUT_FILENO, "\033[0m", 4);
    write(STDOUT_FILENO, "Creates a new directory\n", 24);
    write(STDOUT_FILENO, "\033[0;33m", 7);
    write(STDOUT_FILENO, "  man <name_of_command>: ",25);
    write(STDOUT_FILENO, "\033[0m", 4);
    write(STDOUT_FILENO, "Prints the specifications of that command.\n", 43);
    write(STDOUT_FILENO, "\033[0;33m", 7);
    write(STDOUT_FILENO, "  mv <which> <where>: ", 22);
    write(STDOUT_FILENO, "\033[0m", 4);
    write(STDOUT_FILENO, "Moves the file to the selected directory\n", 41);
    write(STDOUT_FILENO, "\033[0;33m", 7);
    write(STDOUT_FILENO, "  pwd: ", 7);
    write(STDOUT_FILENO, "\033[0m", 4);
    write(STDOUT_FILENO, "Print the current working directory\n", 36);
    write(STDOUT_FILENO, "\033[0;33m", 7);
    write(STDOUT_FILENO, "  rm <file/directory>: ", 23);
    write(STDOUT_FILENO, "\033[0m", 4);
    write(STDOUT_FILENO, "Removes the file or directory which has the right permissions in order to be removed\n",85);
    write(STDOUT_FILENO, "\033[0;33m", 7);
    write(STDOUT_FILENO, "  touch <name>: ", 16);
    write(STDOUT_FILENO, "\033[0m", 4);
    write(STDOUT_FILENO, "Creates a new file if it doesn't exist\n", 39);

}
