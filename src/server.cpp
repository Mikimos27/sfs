#include "../hdr/connection.h"
#include "../hdr/convinience.h"

#include <cstring>

#include <unistd.h>

#include <string>
#include <iostream>
#include <fstream>

#define B_MAX 3000
#define PORT 5050
#define BLOG 10
#define TIMEOUT 1000


class Server{
public:
    Server(int domain, int service, int protocol, unsigned long interface, int port, int backlog, std::string filepath)
        :   con(domain, service, protocol, interface, port, backlog), filepath(filepath), clientfd(0) {}
    ~Server(){
        std::cout << "Server closed\n";
        shutdown(clientfd, SHUT_RDWR);//sys/socket.h
        close(clientfd);//unistd.h
    }

    void Start(){
        if(net() < 0) return;
        if(launch() < 0) return;
    } 

private:

    int net(){
        if(con.sock < 0){
            std::cerr << "Failed to connect...\n";
            return -1;
        }
        if(bind(con.sock, (sockaddr*)&con.address, sizeof(con.address)) < 0){//socket.h
            std::cerr << "Failed to bind...\n";
            return -1;
        }

        if(listen(con.sock, con.backlog) < 0){//socket.h
            std::cerr << "Failed to listen...\n";
            return -1;
        }

        return 1;
    }

    int launch(){
        char buffer[B_MAX] = { 0 };

        clientfd = accept(con.sock, 0, 0);//socket.h


        std::fstream file(filepath, std::ios::out);
        if(!file){
            std::cerr << "CANT OPEN FILE\n";
            close(clientfd);
            return -1;
        }
        while(recv(clientfd, buffer, B_MAX - 1, 0) != 0){
            buffer[B_MAX - 1] = 0;
            file.write(buffer, std::strlen(buffer)); //cstring
        }

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
        std::cerr << "No filepath given";
        return 1;
    }
    if(argc < 3){
        std::cerr << "No port given\n";
        return 1;
    }
    char* filepath = argv[1];
    int port = std::stoi(argv[2]);

    if(port < 0){
        std::cerr << "Invalid port input\n";
        return 1;
    }

    Server server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, BLOG, filepath);
    server.Start();

    return 0;
}
