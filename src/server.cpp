#include "../hdr/connection.h"
#include "../hdr/convinience.h"

#include <cstdio>
#include <cstring>

#include <unistd.h>
#include <sys/time.h>

#include <string>
#include <iostream>

#define B_MAX 3000
//Recomended port:
#define PORT 15050
//////////////////
#define BLOG 10
#define TIMEOUT 10///////////////////////////////////////////DO timeout


class Server{
public:
    Server(int domain, int service, int protocol, unsigned long interface, int port, int backlog, std::string filepath, int timeout)
        :   con(domain, service, protocol, interface, port, backlog), filepath(filepath), clientfd(0), timeout(timeout) {}
    ~Server(){
        std::cout << "Server closed\n";
        shutdown(clientfd, SHUT_RDWR);//sys/socket.h
        close(clientfd);//unistd.h
    }

    void Start(){
        timeval tout;
        tout.tv_sec = timeout;
        tout.tv_usec = 0;
        if(setsockopt(con.sock, SOL_SOCKET, SO_RCVTIMEO, &tout, sizeof(tout))){
            std::cerr << "setsockopt failed\n";
            return;
        }
        if(net() < 0) return;
        if(launch() < 0) return;
    } 

private:

    int net(){
        if(con.sock < 0){
            std::cerr << "Failed to create socket\n";
            return -1;
        }
        if(bind(con.sock, (sockaddr*)&con.address, sizeof(con.address)) < 0){//socket.h
            std::cerr << "Failed to bind\n";
            return -1;
        }

        if(listen(con.sock, con.backlog) < 0){//socket.h
            std::cerr << "Failed to listen\n";
            return -1;
        }

        return 1;
    }

    int launch(){
        unsigned char buffer[B_MAX] = { 0 };

        if((clientfd = accept(con.sock, 0, 0)) < 0){//socket.h
            std::cerr << "Failed to accept connection\n";
            return -1;
        }

        std::FILE* file = std::fopen(filepath.c_str(), "wb");
        if(!file){
            std::cerr << "Can't open file\n";
            return -1;
        }
        size_t rec_s = 0;
        while((rec_s = recv(clientfd, buffer, B_MAX, 0)) > 0){
            std::fwrite(buffer, sizeof(unsigned char), rec_s, file);
            zero_arr(buffer, B_MAX);
        }
        std::fclose(file);

        return 1;
    }



private:
    Connection con;
    std::string filepath;
    int clientfd;
    int timeout;
};




int main(int argc, char** argv){
    int c = 0;
    int port = 0;
    char* filepath = NULL;
    int timeout = 0;

    opterr = 0;
    while ((c = getopt (argc, argv, "f:p:t:")) != -1){
        switch (c){
            case 'p':
                try{
                    port = std::stoi(optarg);
                }
                catch(...){
                    std::cerr <<  "Given port is NAN\n";
                    return 1;
                }
                break;
            case 'f':
                filepath = optarg;
                break;
            case 't':
                try{
                    timeout = std::stoi(optarg);
                }catch(...){
                    std::cerr << "Given timeout is NAN\n";
                    return 1;
                }
                break;
            case '?':
                if (optopt == 'p' || optopt == 'f' || optopt == 't')
                    std::cerr << "Option -" << optopt << " requires an argument.\n";
                else if (isprint (optopt))
                    std::cerr << "Unknown option '-" << (char)optopt << "'.\n";
                else 
            std::cerr << "Unknown option character '" << optopt << "'.\n";
                return 1;
            default:
                return 1;
        }
    }
    int failed = 0;
    if(filepath == NULL){
        std::cerr << "No filepath given\n";
        failed = 1;
    }
    if(port <= 0){
        std::cerr << "Bad port\n";
        failed = 1;
    }
    if(timeout <= 0){
        std::cout << "No timeout given - using defaults\n";
        timeout = TIMEOUT;
    }
    if(failed == 1){
        std::cerr << "Use: server.elf [-p port] [-f filepath] [-t timeout]\n";
        return 1;
    }

    Server server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, BLOG, filepath, timeout);
    server.Start();

    return 0;
}
