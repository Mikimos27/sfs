#include "../hdr/connection.h"
#include "../hdr/send.h"

#include <cstdio>
#include <cstdlib>


#include <unistd.h>
#include <arpa/inet.h>


#include <string>
#include <iostream>


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
        if(launch() < 0) return;
        if(work() < 0) return;
    }

private:
    int launch(){
        if(inet_pton(con.domain, address.c_str(), &con.address.sin_addr) <= 0){
            std::cerr << "Bad address given\n";
            return -1;
        }

        con.interface = inet_addr(address.c_str());
        if(connect(con.sock, (sockaddr*)&con.address, sizeof(con.address)) < 0){
            std::cerr << "Failed to connect to server\n";
            return -1;
        } 
        return 1;
    }


    int work(){

        /*
        unsigned char buffer[B_MAX] = { 0 };
        data_packet pack(0x10);

        std::FILE* file = std::fopen(filepath.c_str(), "rb");
        if(file == NULL){
            std::cerr << "BAD FILE PATH\n";
            return -1;
        }
        size_t chars_read = 0;

        while(!std::feof(file) && (chars_read = std::fread(buffer, sizeof(unsigned char), B_MAX, file)) > 0){
            pack.create_msg(0x10, buffer, chars_read);
            pack.send_packet(con.sock);

            chars_read = 0;
        }
        pack.create_msg(PACKET_END, (unsigned char*)"\0", 1);
        pack.send_packet(con.sock);
        std::fclose(file);
        */
        return 1;
    }

private:
    Connection con;
    std::string address;
    std::string filepath;
};






int main(int argc, char** argv){
    int c = 0;
    int port = 0;
    char* filepath = NULL;
    char* address = NULL;

    opterr = 0;
    while ((c = getopt (argc, argv, "a:f:p:")) != -1){
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
            case 'a':
                address = optarg;
                break;
            case 'f':
                filepath = optarg;
                break;
            case '?':
                if (optopt == 'p' || optopt == 'a' || optopt == 'f')
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
    if(address == NULL){
        std::cerr << "No address given\n";
        failed = 1;
    }
    if(filepath == NULL){
        std::cerr << "No filepath given\n";
        failed = 1;
    }
    if(port <= 0){
        std::cerr << "Bad port\n";
        failed = 1;
    }
    if(failed == 1){
        std::cerr << "Use: client.elf [-p port] [-a address] [-f filepath]\n";
        return 1;
    }


    Client client(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, 1, address, filepath);
    client.Start();

    return 0;
}
