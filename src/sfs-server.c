#include "../hdr/server.h"
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>

#define B_MAX 3000
#define PORT 5050
#define BLOG 10
#define TIMEOUT 1000

void launch(Server* server){

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
    Server server = server_init(AF_INET, SOCK_STREAM, 0, INADDR_ANY, PORT, BLOG, launch);
    server.launch(&server);
    return 0;
}
