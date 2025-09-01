#include "../hdr/connection.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <arpa/inet.h>
#include "convinience.c"

#define B_MAX 3000
#define PORT 5050
#define BLOG 10
#define TIMEOUT 1000
#define MAX_ADDRLEN 15


void net(Connection* client, const char* addr){
    if(inet_pton(client->domain, addr, &client->address.sin_addr) <= 0){
        perror("Failed net connection...");
        exit(1);
    }
    client->interface = 0;
}

void launch(Connection* client){

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



int addr_validation(const char* str){
    int block_counts[4] = {0};
    int dot_num = 0;
    int addrlen = strlen(str);

    for(int i = 0; i < addrlen; i++){
        if(str[i] == '.') { 
            dot_num++;
            continue;
        }
        if(str[0] < '0' || str[i] > '9') return 1;
        if(dot_num > 3) return 1;
        block_counts[dot_num]++;
    }

    for(int i = 0; i < 4; i++){
        if(block_counts[i] == 0 || block_counts[i] > 3) return 1;
    }
    if(dot_num > 3) return 1;
    return 0;
}

int main(int argc, char** argv){
    if(argc < 2) {
        printf("No address given\n");
        exit(1);
    }
    if(addr_validation(argv[1])){
        printf("Bad address\n");
        exit(1);
    }

    Connection client = con_init(AF_INET, SOCK_STREAM, 0, INADDR_ANY, PORT, 1, net, launch);

    client.net(&client, argv[1]);
    client.launch(&client);

    return 0;
}
