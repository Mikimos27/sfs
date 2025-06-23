#include "../hdr/connection.h"
#include <stdio.h>
#include <stdlib.h>

Connection con_init(int domain, int service, int protocol, unsigned long interface, int port, int backlog){
    
    Connection con;
    con.domain = domain;
    con.service = service;
    con.protocol = protocol;
    con.interface = interface;
    con.port = port;
    con.backlog = backlog;


    con.address.sin_family = domain;
    con.address.sin_port = htons(port);
    con.address.sin_addr.s_addr = htonl(interface);

    con.socket = socket(domain, service, protocol);

    return con;
}

