#ifndef ZERGMAP_FUNCTIONS_H
#define ZERGMAP_FUNCTIONS_H


#include "zerg_list.h"
#include "zergmap_functions.h"
#include "zerg_functions.h"
#include "dijkstra.h"

enum
{ zerg_header_length = 12 };

enum
{ etherIpUdp = 14 + 20 + 8 };

enum
{ etherIp6Udp = 14 + 40 + 8 };

enum
{ udpProtocol = 17 };

enum
{ eat = 3751 };

enum
{ linkCheck = 0x1 };

//Used to read/write in the file header
struct FileHeader
{

  uint32_t fileType;
  uint32_t majorVersion:16;
  uint32_t minorVersion:16;
  uint32_t gmtOffset;
  uint32_t accuracyDelta;
  uint32_t maximumLength;
  uint32_t linkLayer;
};

//Used to read/write in the packet capture headers
struct PcapHeader
{
  uint32_t unixEpoch;
  uint32_t epochMicroseconds;
  uint32_t captureLength;
  uint32_t packetLength;
};

//Used to read/write in the ethernet frame
struct __attribute__ ((packed)) EthernetFrame
{
  uint64_t d_mac:48;
  uint64_t s_mac:48;
  uint32_t type:16;
};

//Used to read in the ipv4 header
struct __attribute__ ((packed)) Ipv4Header
{
  uint32_t version:16;
  uint32_t total_length:16;
  uint32_t id:16;
  uint32_t offset:16;
  uint32_t ttl:8;
  uint32_t protocol:8;
  uint32_t checksum:16;
  uint32_t s_ip;
  uint32_t d_ip;
};

struct __attribute__ ((packed)) Ipv6Header
{
  uint32_t version:4;
  uint32_t trafficClass:8;
  uint32_t flowLabel:20;
  uint32_t payLen:16;
  uint32_t next:8;
  uint32_t hop:8;
  uint64_t sourceOne;
  uint64_t sourceTwo;
  uint64_t destOne;
  uint64_t destTwo;
};

//Used to write the ipv4 header for the encoder
struct __attribute__ ((packed)) encodeIpv4
{
  uint32_t version:8;
  uint32_t dscpEcn:8;
  uint32_t total_length:16;
  uint32_t id:16;
  uint32_t offset:16;
  uint32_t protocol:16;
  uint32_t checksum:16;
  uint32_t s_ip;
  uint32_t d_ip;
};

//Used to read/write the Udp Header
struct UdpHeader
{
  uint32_t s_port:16;
  uint32_t d_port:16;
  uint32_t length:16;
  uint32_t checksum:16;
};

//Used to read the zerg packet header
struct __attribute__ ((packed)) ZergHeader
{
  uint32_t version;
  uint32_t source:16;
  uint32_t dest:16;
  uint32_t id;
};

//Used to write the zerg packet header in Encoder
struct __attribute__ ((packed)) EncodeZerg
{
  uint32_t type:4;
  uint32_t version:4;
  uint32_t length:24;
  uint32_t source:16;
  uint32_t dest:16;
  uint32_t id;
};

//Used to read status packet payloads
struct StatusPacket
{
  uint32_t hit_armor;
  uint32_t max_type;
  uint32_t speed;
  char name[1];
};

//Used to write the status packet payloads
struct __attribute__ ((packed)) EncodeStatusPacket
{
  uint32_t hitPoints:24;
  uint32_t armor:8;
  uint32_t maxPoints:24;
  uint32_t type:8;
  uint32_t speed;
};

//Used to read message packet payloads
struct MessagePacket
{
  char *message;
};

//Used to read/write command packet payloads
struct CommandPacket
{
  uint32_t command:16;
  uint32_t parameter_one:16;
  uint32_t parameter_two;
};

//Used to read Gps packet payloads
struct GpsPacket
{
  uint32_t long_first;
  uint32_t long_second;
  uint32_t lat_first;
  uint32_t lat_second;
  uint32_t altitude;
  uint32_t bearing;
  uint32_t speed;
  uint32_t accuracy;
};

//Used to write Gps packet payloads
struct EncodeGps
{
  uint64_t longitude;
  uint64_t latitude;
  uint32_t altitude;
  uint32_t bearing;
  uint32_t speed;
  uint32_t accuracy;
};

//Used to print and format payloads in decode.c
union PayloadStructs
{
  struct StatusPacket stat;
  struct MessagePacket mess;
  struct CommandPacket comm;
  struct GpsPacket gps;
};

union PayloadStructs *struct_init (int total, FILE * decode_file, int type);
double doub_converter (uint64_t * number);
uint64_t doub_flip (uint32_t * lat_long, uint32_t * long_lat);
void print_zerg_header (struct ZergHeader *zerg);
float converter (uint32_t * thing);
const char *raceId (uint32_t type);
char *decode_message (int total, FILE * decode_file);

#endif
