#include "../hdr/send.h"
#include <stdint.h>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#include <chrono>

#if __BIG_ENDIAN__
# define htonll(x) (x)
# define ntohll(x) (x)
#else
# define htonll(x) (((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
# define ntohll(x) (((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))
#endif




uint64_t time_epoch_ms() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}


using std::size_t;


void print_hex(const unsigned char* str, size_t len){
    for(size_t i = 0; i < len; i++){
        printf("%02x", str[i]);
    }
}

packet_aggregate::packet_aggregate(version_t v, type_t ty, timems_t tms, msglen_t ml, unsigned char* msg)
: version(v), type(ty), timems(tms), msglen(ml), msg(msg) {}
packet_aggregate::packet_aggregate() : packet_aggregate(0, 0, 0, 0, nullptr) {}
packet_aggregate::packet_aggregate(const packet_aggregate& other)
: version(other.version), type(other.type), timems(other.timems), msglen(other.msglen){
    msg = new unsigned char[msglen];
    memcpy(msg, other.msg, msglen);
}
packet_aggregate::packet_aggregate(packet_aggregate&& other)
: version(other.version), type(other.type), timems(other.timems), msglen(other.msglen){
    msg = other.msg;
    other.msg = nullptr;
    other.version = 0;
    other.type = 0;
    other.timems = 0;
    other.msglen = 0;
}
packet_aggregate::~packet_aggregate(){
    if(msg != nullptr) delete[] msg;
    msg = nullptr;
}
const msglen_t packet_aggregate::length() const { return msglen; }
const unsigned char* const packet_aggregate::get_msg() const { return msg; }




data_packet::data_packet()
: data(0) {}
data_packet::~data_packet(){
    memset(data, 0, LIMIT);
}


int data_packet::create_msg(const unsigned char* msg, uint32_t len){
    if(len > MAX_MSGLEN) return -1;
    if(len <= 0) return -1;

    unsigned char* dptr = data;
    memset(dptr, 0, LIMIT);

    version_t version = htons(VERSION);
    type_t type = htons(PACKET_MSG);
    timems_t time = htonll(time_epoch_ms());
    msglen_t msglen = htonl(len);

    memcpy(dptr, &version, version_s);
    dptr += version_s;
    memcpy(dptr, &type, type_s);
    dptr += type_s;
    memcpy(dptr, &time, timems_s);
    dptr += timems_s;
    memcpy(dptr, &msglen, msglen_s);
    dptr += msglen_s;
    memcpy(dptr, msg, len);

    return 0;
}

int data_packet::create_msg(const std::string& msg){
    return create_msg((const unsigned char*)msg.c_str(), msg.length());
}



int data_packet::create_auth(Ed25519& key){
    version_t version = htons(VERSION);
    type_t type = htons(PACKET_AUTH);
    timems_t time = htonll(time_epoch_ms());
    msglen_t msglen = htonl(ED25519_KEY + ED25519_SIG);
    unsigned char key_raw[ED25519_KEY];

    if(key.get_raw_pub() || key.get_out_size() != ED25519_KEY) return -1;
    memcpy(key_raw, key.get_out_buff(), ED25519_KEY);

    unsigned char* dptr = data;
    memcpy(dptr, &version, version_s);
    dptr += version_s;
    memcpy(dptr, &type, type_s);
    dptr += type_s;
    memcpy(dptr, &time, timems_s);
    dptr += timems_s;
    memcpy(dptr, &msglen, msglen_s);
    dptr += msglen_s;
    memcpy(dptr, key_raw, ED25519_KEY);
    dptr += ED25519_KEY;

    if(key.sign(data, dptr - data) || key.get_out_size() != ED25519_SIG) return -1;


    memcpy(dptr, key.get_out_buff(), ED25519_SIG);
    return 0;
}

int data_packet::validate_packet(Ed25519& key){
    type_t type = 0;
    memcpy(&type, data + version_s, type_s);
    if(ntohs(type) != PACKET_AUTH) return -1;
    return key.verify(data, version_s + type_s + timems_s + msglen_s + ED25519_KEY,
                      data + version_s + type_s + timems_s + msglen_s + ED25519_KEY, ED25519_SIG);
}

int data_packet::send_packet(int sockfd){
    version_t version = 0;
    type_t type = 0;
    timems_t timems = 0;
    msglen_t msglen = 0;

    unsigned char* dptr = data;
    memcpy(&version, dptr, version_s);
    dptr += version_s;
    memcpy(&type, dptr, type_s);
    dptr += type_s;
    memcpy(&timems, dptr, timems_s);
    dptr += timems_s;
    memcpy(&msglen, dptr, msglen_s);
    dptr += msglen_s;

    version = ntohs(version);
    type = ntohs(type);
    timems = ntohll(timems);
    msglen = ntohl(msglen);
    if(msglen > MAX_MSGLEN) {
        printf("Msglen too big\n\n");
        return -1;
    }

    printf("ver = %d\ntype = %d\ntime = %ld\nmsglen = %d\n", version, type, timems, msglen);
    if(type == PACKET_AUTH){
        printf("key = ");
        print_hex(dptr, ED25519_KEY);
        printf("\nsig = ");
        dptr += ED25519_KEY;
        print_hex(dptr, ED25519_SIG);
    }
    else{
        printf("msg = ");
        for(msglen_t i = 0; i < msglen; i++){
            printf("%c", (char)dptr[i]);
        }
    }
    dptr += msglen;
    printf("\n");

    return 0;
}

int data_packet::recv_packet(int sockfd){
    int wait = 20;
    ssize_t recvd = 0;

    while((recvd += recv(sockfd, data + recvd, LIMIT - recvd, 0)) != LIMIT){
        printf("\nrecvd= %ld\n", recvd);
        if(wait <= 0) return 1;
        wait--;
    }


    
    return 0;
}

const unsigned char* const data_packet::get_data_raw() const {
    return data;
}

bool data_packet::check_version() const {
    if(get_version() == VERSION) return true;
    return false;
}
bool data_packet::check_type(type_t compared) const {
    if(get_type() == compared) return true;
    return false;
}

const version_t data_packet::get_version() const {
    version_t ver = 0;
    const unsigned char* dptr = data;
    memcpy(&ver, dptr, version_s);
    return ntohs(ver);
}
const type_t data_packet::get_type() const {
    type_t type = 0;
    const unsigned char* dptr = data + version_s;
    memcpy(&type, dptr, type_s);
    return ntohs(type);
}
const timems_t data_packet::get_time_ms() const {
    timems_t timems = 0;
    const unsigned char* dptr = data + version_s + type_s;
    memcpy(&timems, dptr, timems_s);
    return ntohll(timems);
}
const msglen_t data_packet::get_msglen() const {
    msglen_t msglen = 0;
    const unsigned char* dptr = data + version_s + type_s + timems_s;
    memcpy(&msglen, dptr, msglen_s);
    return ntohl(msglen);
}


const std::expected<packet_aggregate, const char*> data_packet::get_data() const{
    version_t ver = 0;
    type_t type = 0;
    timems_t timems = 0;
    msglen_t msglen = 0;
    unsigned char* msg = nullptr;
    const unsigned char* dptr = data;

    memcpy(&ver, dptr, version_s);
    dptr += version_s;
    memcpy(&type, dptr, type_s);
    dptr += type_s;
    memcpy(&timems, dptr, timems_s);
    dptr += timems_s;
    memcpy(&msglen, dptr, msglen_s);
    dptr += msglen_s;

    ver = ntohs(ver);
    type = ntohs(type);
    timems = ntohll(timems);
    msglen = ntohl(msglen);

    if(msglen > MAX_MSGLEN) return std::unexpected("Msglen bigger than limit");//To std::expected

    msg = new unsigned char[msglen];
    if(!msg) return std::unexpected("Heap allocation error");
    memcpy(msg, dptr, msglen);
    dptr = nullptr;

    packet_aggregate ret{ ver, type, timems, msglen, msg };
    return ret;
}
