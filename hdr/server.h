#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>

typedef struct Server{
    int domain;
    int service;
    int protocol;
    unsigned long interface;
    int port;
    int backlog;

    int socket;

    struct sockaddr_in address;

    void (*net)(struct Server*, const char*);
    void (*launch)(struct Server*);

} Server;
Server server_init(int, int, int, unsigned long, int, int, void (*)(Server*));
Server client_init(int, int, int, unsigned long, int, int, void (*)(Server*, const char*), void (*)(Server*));

#endif
