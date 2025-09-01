#include "server.h"
#include <stdio.h>
#include <stdlib.h>

Server server_init(int domain, int service, int protocol, unsigned long interface, int port, int backlog, void (*launch)(Server*)){
    
    Server server;
    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;


    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);

    server.socket = socket(domain, service, protocol);

    if(server.socket < 0){
        perror("Failed to connect...");
        exit(1);
    }
    if(bind(server.socket, (struct sockaddr*)&server.address,
                sizeof(server.address)) < 0){
        perror("Failed to bind...");
        exit(1);
    }

    if(listen(server.socket, server.backlog) < 0){
        perror("Failed to listen...");
        exit(1);
    }

    server.launch = launch;

    return server;
}

Server client_init(int domain, int service, int protocol, unsigned long interface, int port, int backlog, void (*net)(Server*, const char*), void (*launch)(Server*)){

    Server client;
    client.domain = domain;
    client.service = service;
    client.protocol = protocol;
    client.interface = interface;
    client.port = port;
    client.backlog = backlog;
    client.launch = launch;

    client.address.sin_family = domain;
    client.address.sin_port = htons(port);
    client.address.sin_addr.s_addr = htonl(interface);

    client.socket = socket(domain, service, protocol);

    if(client.socket < 0){
        perror("Failed to connect to socket...");
        exit(1);
    }

    client.net = net;
    client.launch = launch;

    return client;
}
