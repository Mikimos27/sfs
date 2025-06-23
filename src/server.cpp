#include "../hdr/connection.h"
#include "../hdr/convinience.h"
#include <stdio.h>
#include <unistd.h>
#include <cstring>
//#include <poll.h>
#include <stdlib.h>

#include <string>

#define B_MAX 3000
#define PORT 5050
#define BLOG 10
#define TIMEOUT 1000


class Server{
public:
    Server(int domain, int service, int protocol, unsigned long interface, int port, int backlog, std::string filepath)
        :   con(domain, service, protocol, interface, port, backlog), filepath(filepath), clientfd(0) {}
    ~Server(){
        printf("Server closed\n");
        shutdown(clientfd, SHUT_RDWR);
        close(clientfd);
    }

    void Start(){
        if(net() < 0) return;
        if(launch() < 0) return;
    } 

private:

    int net(){
        if(con.sock < 0){
            perror("Failed to connect...\n");
            return -1;
        }
        if(bind(con.sock, (struct sockaddr*)&con.address,
                sizeof(con.address)) < 0){
            perror("Failed to bind...\n");
            return -1;
        }

        if(listen(con.sock, con.backlog) < 0){
            perror("Failed to listen...\n");
            return -1;
        }

        return 1;
    }

    int launch(){
        char buffer[B_MAX] = { 0 };

        clientfd = accept(con.sock, 0, 0);
        FILE* file = fopen(filepath.c_str(), "w");
        if(file == NULL){
            perror("CANT OPEN FILE\n");
            close(clientfd);
            return -1;
        }
        while(recv(clientfd, buffer, B_MAX - 1, 0) != 0){
            buffer[B_MAX - 1]  = 0;
            fwrite(buffer, sizeof(char), std::strlen(buffer), file);
        }

        fclose(file);
        close(clientfd);

        return 1;
    }



private:
    Connection con;
    std::string filepath;
    int clientfd;
};




int main(int argc, char** argv){
    if(argc < 2){
        perror("No filepath given");
        return 1;
    }
    if(argc < 3){
        perror("No port given\n");
        return 1;
    }
    char* filepath = argv[1];
    int port = atoi(argv[2]);

    if(port < 0){
        perror("Invalid port input\n");
        return 1;
    }

    Server server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, BLOG, filepath);
    server.Start();

    return 0;
}
