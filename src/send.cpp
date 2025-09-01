#include "../hdr/send.h"
#include <stdint.h>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

using std::size_t;


void print_hex(const unsigned char* str, size_t len){
    for(size_t i = 0; i < len; i++){
        printf("%02x", str[i]);
    }
}

data_packet::data_packet(uint16_t type)
: type(type), msg(nullptr), msglen(0) {}
data_packet::~data_packet(){
    delete[] msg;
    msg = nullptr;
    msglen = 0;
    type = 0;
}

int data_packet::create_msg(uint16_t type, const std::string& to_msg){
    if(to_msg.length() > LIMIT) return -1; 
    if(to_msg.length() == 0) return -1;
    this->type = type;
    msglen = to_msg.length();
    delete[] msg;
    msg = new unsigned char[msglen];
    if(!msg) return -1;

    for(size_t i = 0; i < msglen; i++){
        msg[i] = (unsigned char)to_msg[i];
    }
    return 0;
}


int data_packet::create_msg(uint16_t type, unsigned char* to_msg, uint32_t to_msg_len){
    if(to_msg_len > (LIMIT - 2 * sizeof (uint16_t))) return -1; 
    if(to_msg_len == 0) return -1;
    this->type = type;
    msglen = to_msg_len;
    delete[] msg;
    msg = new unsigned char[msglen];
    if(!msg) return -1;

    for(size_t i = 0; i < msglen; i++){
        msg[i] = to_msg[i];
    }
    return 0;
}


int data_packet::send_packet(int sockfd){
    uint16_t version = VERSION;

    unsigned char* packet = new unsigned char[LIMIT];
    for(size_t i = 0; i < LIMIT; i++) packet[i] = 0;
    

    memcpy(packet, &version, sizeof version);
    memcpy(packet + sizeof version, &type, sizeof type);
    //htonl
    uint32_t msglen_send = htonl(msglen);

    memcpy(packet + sizeof version + sizeof type, &msglen_send, sizeof(msglen_send));
    memcpy(packet + sizeof version + sizeof type + sizeof(msglen_send), msg, msglen);


    auto sent = send(sockfd, packet, LIMIT, 0);
    if((sent <= 0)) return -1;
    printf("\nsent = %ld\n", sent);


    delete[] packet;
    return 0;
}

int data_packet::recv_packet(int sockfd){
    unsigned char* packet = new unsigned char[LIMIT];
    int wait = 20;

    ssize_t recvd = 0;
    while((recvd += recv(sockfd, packet + recvd, LIMIT - recvd, 0)) != LIMIT){
        printf("\nrecvd= %ld\n", recvd);
        if(wait <= 0) return 1;
        wait--;
    }


    
    //check if version less than recvd???
    uint16_t version = 0;

    memcpy(&version, packet, sizeof version);
    if(version != VERSION) return -2;/////////////////////////<------------------TO COŚ
    
    memcpy(&type, packet + sizeof version, sizeof type);
    memcpy(&msglen, packet + sizeof version + sizeof type, sizeof msglen);
    msglen = ntohl(msglen);
    delete[] msg;

    msg = new unsigned char[msglen];
    memcpy(msg, packet + sizeof version + sizeof type + sizeof msglen, msglen);


    delete[] packet;
    return 0;
}
std::vector<unsigned char> data_packet::get_msg(){
    std::vector<unsigned char> ret;
    for(size_t i = 0; i < msglen; i++) ret.push_back(msg[i]);
    return ret;
}
