#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <gmodule.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <glib/gstdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <pthread.h>

#define BUFF 2048

void* worker(void *arg_)
{
    int fd = (int) ((long)arg_);
    char buf[BUFF] = { 0 };
    ssize_t r = read(fd, buf, BUFF);

    close(fd);

    pthread_exit(NULL);
}

int main()
{
    struct addrinfo hints;
    struct addrinfo *results;
    int addrinfo_error;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    addrinfo_error = getaddrinfo("localhost", "2048", &hints, &results);

    if (addrinfo_error != 0)
        errx(EXIT_FAILURE, "Fail to get address for localhost on port 2048: %s", gai_strerror(addrinfo_error));

    struct addrinfo *rp;
    int cnx;

    for (rp = results; rp; rp = rp->ai_next)
    {
        cnx = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (cnx == -1)
            continue;
        int value = 1;
        if (setsockopt(cnx, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) == -1)
            err(EXIT_FAILURE, "Error caught by setsockopt");
        if (bind(cnx, rp->ai_addr, rp->ai_addrlen) == 0)
            break;
        close(cnx);
    }

    freeaddrinfo(results);

    if (!rp)
        err(EXIT_FAILURE, "Couldn't connect");
    if (listen(cnx, 5) == -1)
        err(EXIT_FAILURE, "Error listening");
    int fd;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    socklen_t addrlen = sizeof(struct sockaddr_in);

    write(STDOUT_FILENO, "Static Server\nListening to port 2048...\n", 40);
    while(1)
    {
        fd = accept(cnx, (struct sockaddr *) &addr, &addrlen);
        if (fd == -1)
            err(EXIT_FAILURE, "Error with accept");

        pthread_t thread;
        int error = pthread_create(&thread, NULL, worker, (void*) ((long)fd));
        if (error != 0)
            err(EXIT_FAILURE, "Creating thread failed");
    }
    close(cnx);
    return 0;
}
