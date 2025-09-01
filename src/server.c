#include "../hdr/connection.h"
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <stdlib.h>

#define B_MAX 3000
#define PORT 5050
#define BLOG 10
#define TIMEOUT 1000


void net(Connection* server, const char* addr){
    
    if(server->socket < 0){
        perror("Failed to connect...\n");
        exit(1);
    }
    if(bind(server->socket, (struct sockaddr*)&server->address,
                sizeof(server->address)) < 0){
        perror("Failed to bind...\n");
        exit(1);
    }

    if(listen(server->socket, server->backlog) < 0){
        perror("Failed to listen...\n");
        exit(1);
    }

}


void launch(Connection* server){

    int clientfd = accept(server->socket, 0, 0);

    struct pollfd fds[1] = { { clientfd, POLLIN, 0} };

    while(1){
        
        char buffer[B_MAX] = { 0 };

        poll(fds, 1, TIMEOUT);

        if(fds[0].revents & POLLIN){
            if(recv(clientfd, buffer, B_MAX - 1, 0) == 0){
                close(clientfd);
                return;
            }
            printf("%s", buffer);
        }

    }
}



int main(){
    Connection server = con_init(AF_INET, SOCK_STREAM, 0, INADDR_ANY, PORT, BLOG, net, launch);
    server.net(&server, 0);
    server.launch(&server);
    return 0;
}
