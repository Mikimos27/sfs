#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>

#define B_MAX 3000
#define PORT 5050
#define BLOG 10
#define TIMEOUT 1000

void launch(Server* client){

    if(connect(client->socket, (struct sockaddr*)&client->address, sizeof(client->address)) < 0){
        perror("Failed to connect to server...");
        exit(1);
    } 

    struct pollfd fds[1] = { { 0, POLLIN, 0} };

    while(1){

        char buffer[B_MAX] = { 0 };

        poll(fds, 1, TIMEOUT);

        if(fds[0].revents & POLLIN){
            read(0, buffer, B_MAX - 1);
            send(client->socket, buffer, B_MAX - 1, 0);
        }
    }

}


int main(){
    Server client = client_init(AF_INET, SOCK_STREAM, 0, INADDR_ANY, PORT, 1, launch);

    client.launch(&client);

    return 0;
}
