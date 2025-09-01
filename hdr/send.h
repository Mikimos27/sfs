#include <string>
#include <stdint.h>
#include <vector>
#include "version_defs.h"

class data_packet{
public:

    data_packet(uint16_t type);
    ~data_packet();
    int create_msg(uint16_t type, const std::string& to_msg);
    int create_msg(uint16_t type, unsigned char* to_msg, uint32_t to_msg_len);
    int send_packet(int sockfd);
    int recv_packet(int sockfd);
    std::vector<unsigned char> get_msg();

    uint16_t type;
private:
    unsigned char* msg;
    uint32_t msglen;
};

