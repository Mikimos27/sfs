#include "../hdr/connection.h"
#include "../hdr/convinience.h"
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
        close(server->socket);
        exit(1);
    }
    if(bind(server->socket, (struct sockaddr*)&server->address,
                sizeof(server->address)) < 0){
        perror("Failed to bind...\n");
        close(server->socket);
        exit(1);
    }

    if(listen(server->socket, server->backlog) < 0){
        perror("Failed to listen...\n");
        close(server->socket);
        exit(1);
    }

}

void launch(Connection* server, const char* filepath){
    char buffer[B_MAX] = { 0 };

    int clientfd = accept(server->socket, 0, 0);
    FILE* file = fopen(filepath, "w");
    if(file == NULL){
        perror("CANT OPEN FILE\n");
        close(clientfd);
        return;
    }
    while(recv(clientfd, buffer, B_MAX - 1, 0) != 0){
        buffer[B_MAX - 1]  = 0;
        fprintf(file, buffer, B_MAX - 1); //TODO - change to use fwrite()
    }

    fclose(file);
    close(clientfd);

}


int main(int argc, char** argv){
    if(argc < 2){
        perror("GIVE FILEPATH\n");
        exit(1);
    }
    Connection server = con_init(AF_INET, SOCK_STREAM, 0, INADDR_ANY, PORT, BLOG);
    

    net(&server, 0);
    launch(&server, argv[1]);
    close(server.socket);
    return 0;
}
