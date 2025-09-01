#include "../hdr/connection.h"
#include "../hdr/convinience.h"
//#include "../hdr/getch.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>

#define B_MAX 3000
#define BLOG 10
#define TIMEOUT 1000
#define MAX_ADDRLEN 15


void net(Connection* client, const char* addr){
    if(inet_pton(client->domain, addr, &client->address.sin_addr) <= 0){
        perror("Bad address given\n");
        close(client->socket);
        exit(1);
    }
    client->interface = inet_addr(addr);
}


void launch(Connection* client){
    if(connect(client->socket, (struct sockaddr*)&client->address, sizeof(client->address)) < 0){
        perror("Failed to connect to server\n");
        close(client->socket);
        exit(1);
    } 
}


void manage(Connection* client, const char* filepath){
    char buffer[B_MAX] = { 0 };

    FILE* file = fopen(filepath, "r");
    if(file == NULL){
        perror("BAD FILE PATH\n");
        close(client->socket);
        exit(1);
    }
    size_t chars_read = 0;
    while((chars_read = fread(buffer, sizeof(char), B_MAX - 1, file)) > 0){
        buffer[chars_read] = 0;
        send(client->socket, buffer, B_MAX - 1, 0);
        zero_arr(buffer, B_MAX);
    }
    fclose(file);
}



int main(int argc, char** argv){
    if(argc < 2) {
        perror("No address given\n");
        exit(1);
    }
    if(argc < 3){
        perror("No filepath given\n");
        exit(1);
    }
    if(argc < 4){
        perror("No port given\n");
        exit(1);
    }
    
    char* address = argv[1];
    char* filepath = argv[2];
    int port = atoi(argv[3]);

    if(port < 0){
        perror("Invalid port input\n");
        exit(1);
    }
    
    Connection client = con_init(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, 1);

    net(&client, address);
    launch(&client);
    manage(&client, filepath);
    close(client.socket);

    return 0;
}
