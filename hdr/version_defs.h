#ifndef VERSION_DEFS_H
#define  VERSION_DEFS_H
#include <cstdint>
#include <cstddef>

#define VERSION 0x00
#define LIMIT 16384


#define PACKET_END 0x00
#define PACKET_OK 0x01
#define PACKET_MSG 0x02

#define PACKET_AUTH 0x04
#define PACKET_EXCH 0x05


using version_t = uint16_t;
using type_t = uint16_t;
using timems_t = uint64_t;
using msglen_t = uint32_t;
constexpr size_t version_s = 2;
constexpr size_t type_s = 2;
constexpr size_t timems_s = 8;
constexpr size_t msglen_s = 4;

#endif
