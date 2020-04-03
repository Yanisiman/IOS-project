#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>

void pwd(char arr[], size_t n)
{
    char *path = getcwd(arr, n);
    size_t len = strlen(path);
    if (len <= n)
    {
        path[len] = '\n';
        path[len+1] = '\0';
    }
    int w = write(STDOUT_FILENO, path, len+1);
    if (w == -1)
        write(STDERR_FILENO, "Error while writing pwd\n", 24);
}
