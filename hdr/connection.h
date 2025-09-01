#ifndef CONNECTION_H
#define CONNECTION_H

#include <sys/socket.h>
#include <netinet/in.h>

struct Connection{
    int domain;
    int service;
    int protocol;
    unsigned long interface;
    int port;
    int backlog;

    int sock;

    struct sockaddr_in address;

    Connection(int, int, int, unsigned long, int, int);
    ~Connection();
};

#endif
