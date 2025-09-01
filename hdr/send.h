#include <string>
#include <stdint.h>

class data_packet{
public:

    data_packet(uint8_t version, uint8_t type, uint32_t limit);
    ~data_packet();
    int create_msg(const std::string& to_msg);
    int create_msg(unsigned char* to_msg, uint32_t to_msg_len);
    int send_packet(int sockfd);

private:
    uint8_t version;
    uint8_t type;
    unsigned char* msg;
    uint32_t msglen;
    uint32_t limit;
};

