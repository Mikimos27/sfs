#ifndef CONNECTION_H
#define CONNECTION_H

#include <sys/socket.h>
#include <netinet/in.h>

typedef struct Connection{
    int domain;
    int service;
    int protocol;
    unsigned long interface;
    int port;
    int backlog;

    int socket;

    struct sockaddr_in address;

    void (*net)(struct Connection*, const char*);
    void (*launch)(struct Connection*);

} Connection;
Connection con_init(int, int, int, unsigned long, int, int, void (*)(Connection*, const char*), void (*)(Connection*));

#endif
