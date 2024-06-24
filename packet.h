#include <cstdint>
#include <cstdlib>

//Command and Status Messages
#define SUBSCRIBE_START 0x01
#define SUBSCRIBE_STOP 0x02
#define SET_POSITION 0x04
#define POSE_SET 0x08

namespace packet {

//Packet for Position
struct pose_packet {
    int _idx;
    int _x;
    int _y;
};

//Packet for Command
struct cmd_packet {
    uint8_t command;
    pose_packet position;
};

struct rsp_packet {
    uint8_t status;
    pose_packet position;
};


}