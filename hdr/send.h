#include <string>
#include <cstdint>
#include <vector>
#include <expected>
#include "version_defs.h"
#include "crypto.h"


class data_packet{
public:
    static constexpr size_t MAX_MSGLEN = LIMIT - version_s - type_s - timems_s - msglen_s;
    struct packet_aggregate{
        version_t version = 0;
        type_t type = 0;
        timems_t timems = 0;

        packet_aggregate(version_t v, type_t ty, timems_t tms, msglen_t ml, unsigned char* msg);
        packet_aggregate();
        packet_aggregate(const packet_aggregate&);
        packet_aggregate(packet_aggregate&&);
        ~packet_aggregate();
        const msglen_t length() const;
        const unsigned char* const get_msg() const;
    private:
        msglen_t msglen = 0;
        unsigned char* msg = nullptr;
    };

    data_packet();
    ~data_packet();
    int create_msg(const unsigned char* msg, uint32_t len);
    int create_msg(const std::string& msg);

    int create_auth(Ed25519& key);

    int send_packet(int sockfd);
    int recv_packet(int sockfd);

    bool check_version() const;
    bool check_type(type_t compared) const;

    const unsigned char* const get_data_raw() const;

    const std::expected<packet_aggregate, const char*> get_data() const;


private:
    unsigned char data[LIMIT];
};

