#include "../hdr/connection.h"
#include "../hdr/convinience.h"
#include <stdio.h>
#include <stdlib.h>


#include <arpa/inet.h>


#include <string>
#include <iostream>
#include <fstream>


#define B_MAX 3000
#define BLOG 10
#define TIMEOUT 1000
#define MAX_ADDRLEN 15



class Client{
public:
    Client(int domain, int service, int protocol, unsigned long interface, int port, int backlog, std::string address, std::string filepath)
        : con(domain, service, protocol, interface, port, backlog), address(address), filepath(filepath) {}

   
    ~Client(){
        std::cout << "Client closed\n";
    }
    

    void Start(){
        if(net() < 0) return;
        if(launch() < 0) return;
        if(manage() < 0) return;
    }

private:
    int net(){
        if(inet_pton(con.domain, address.c_str(), &con.address.sin_addr) <= 0){
            std::cerr << "Bad address given\n";
            return -1;
        }
        con.interface = inet_addr(address.c_str());
        return 1;
    }


    int launch(){
        if(connect(con.sock, (sockaddr*)&con.address, sizeof(con.address)) < 0){
            std::cerr << "Failed to connect to server\n";
            return -1;
        } 
        return 1;
    }


    int manage(){
        char buffer[B_MAX] = { 0 };
        
        std::fstream file(filepath, std::ios::in);
        if(!file){
            std::cerr << "BAD FILE PATH\n";
            return -1;
        }
        while(file){
            file.read(buffer, B_MAX - 1);
            send(con.sock, buffer, B_MAX - 1, 0);
            zero_arr(buffer, B_MAX);
        }

        return 1;
    }

private:
    Connection con;
    std::string address;
    std::string filepath;
};






int main(int argc, char** argv){
    if(argc < 2) {
        std::cerr << "No address given\n";
        return 1;
    }
    if(argc < 3){
        std::cerr << "No filepath given\n";
        return 1;
    }
    if(argc < 4){
        std::cerr << "No port given\n";
        return 1;
    }

    char* address = argv[1];
    char* filepath = argv[2];
    int port = atoi(argv[3]);

    if(port < 0){
        std::cerr << "Invalid port input\n";
        return 1;
    }




    Client client(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, 1, address, filepath);
    client.Start();

    return 0;
}
