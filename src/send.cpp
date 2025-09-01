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

data_packet::data_packet(uint8_t version, uint8_t type, uint32_t limit)
: version(version), type(type), msg(nullptr), msglen(0), limit(limit) {}
data_packet::~data_packet(){
    delete[] msg;
    msg = nullptr;
    msglen = 0;
    version = 0;
    type = 0;
    limit = 0;
}

int data_packet::create_msg(const std::string& to_msg){
    if(to_msg.length() > limit) return -1; 
    if(to_msg.length() == 0) return -1;
    msglen = to_msg.length();
    delete[] msg;
    msg = new unsigned char[msglen];
    if(!msg) return -1;

    for(size_t i = 0; i < msglen; i++){
        msg[i] = (unsigned char)to_msg[i];
    }
    return 0;
}


int data_packet::create_msg(unsigned char* to_msg, uint32_t to_msg_len){
    if(to_msg_len > limit) return -1; 
    if(to_msg_len == 0) return -1;
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
    size_t packet_size = sizeof(version) + sizeof(type) + sizeof(msglen) + msglen;
    unsigned char* packet = new unsigned char[packet_size];
    

    packet[0] = version;
    packet[1] = type;
    //htonl
    uint32_t msglen_send = htonl(msglen);

    memcpy(packet + sizeof version + sizeof type, &msglen_send, sizeof(msglen_send));
    memcpy(packet + sizeof version + sizeof type + sizeof(msglen_send), msg, msglen);


    auto sent = write(sockfd, packet, packet_size);
    if((sent <= 0)) return -1;
    printf("\nsent = %ld\n", sent);


    delete[] packet;
    return 0;
}
