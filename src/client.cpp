#include "../hdr/connection.h"
#include "../hdr/convinience.h"
//#include "../hdr/getch.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>


#include <string>


#define B_MAX 3000
#define BLOG 10
#define TIMEOUT 1000
#define MAX_ADDRLEN 15



class Client{
public:
    Client(int domain, int service, int protocol, unsigned long interface, int port, int backlog, std::string address, std::string filepath)
        : con(domain, service, protocol, interface, port, backlog), address(address), filepath(filepath) {}

   
    ~Client(){
        printf("Client closed\n");
    }
    

    void Start(){
        if(net() < 0) return;
        if(launch() < 0) return;
        if(manage() < 0) return;
    }

private:
    int net(){
        if(inet_pton(con.domain, address.c_str(), &con.address.sin_addr) <= 0){
            perror("Bad address given\n");
            return -1;
        }
        con.interface = inet_addr(address.c_str());
        return 1;
    }


    int launch(){
        if(connect(con.sock, (struct sockaddr*)&con.address, sizeof(con.address)) < 0){
            perror("Failed to connect to server\n");
            return -1;
        } 
        return 1;
    }


    int manage(){
        char buffer[B_MAX] = { 0 };

        FILE* file = fopen(filepath.c_str(), "r");
        if(file == NULL){
            perror("BAD FILE PATH\n");
            return -1;
        }

        size_t chars_read = 0;
        while((chars_read = fread(buffer, sizeof(char), B_MAX - 1, file)) > 0){
            buffer[chars_read] = 0;
            send(con.sock, buffer, B_MAX - 1, 0);
            zero_arr(buffer, B_MAX);
        }
        fclose(file);
        return 1;
    }

private:
    Connection con;
    std::string address;
    std::string filepath;
};






int main(int argc, char** argv){
    if(argc < 2) {
        perror("No address given\n");
        return 1;
    }
    if(argc < 3){
        perror("No filepath given\n");
        return 1;
    }
    if(argc < 4){
        perror("No port given\n");
        return 1;
    }

    char* address = argv[1];
    char* filepath = argv[2];
    int port = atoi(argv[3]);

    if(port < 0){
        perror("Invalid port input\n");
        return 1;
    }




    Client client(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, 1, address, filepath);
    client.Start();

    return 0;
}
