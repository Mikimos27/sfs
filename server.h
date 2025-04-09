#ifndef Server_h
#define Server_h

#include <sys/socket.h>
#include <netinet/in.h>

struct Server{
    int domain;
    int service;
    int protocol;
    unsigned long interface;
    int port;
    int backlog;

    int socket;

    struct sockaddr_in address;

    void (*net)(struct Server*);
    void (*launch)(struct Server*);

};
typedef struct Server Server;
Server server_init(int, int, int, unsigned long, int, int, void (*)(Server*));
Server client_init(int, int, int, unsigned long, int, int, void (*)(Server*), void (*)(Server*));

#endif
