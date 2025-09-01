#include "../hdr/connection.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Connection::Connection (int domain, int service, int protocol, unsigned long interface, int port, int backlog){
    this->domain = domain;
    this->service = service;
    this->protocol = protocol;
    this->interface = interface;
    this->port = port;
    this->backlog = backlog;


    this->address.sin_family = domain;
    this->address.sin_port = htons(port);
    this->address.sin_addr.s_addr = htonl(interface);

    this->sock = socket(domain, service, protocol);
}

Connection::~Connection(){
    printf("Closed connection\n");
    shutdown(this->sock, SHUT_RDWR);
    close(this->sock);
}
