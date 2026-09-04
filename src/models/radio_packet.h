#pragma once

struct RadioPacket {
    unsigned char sequence = 0;
    unsigned char flags = 0;
    int throttle = 0;
    int steering = 0;
    bool lights = false;
    bool emergencyStop = false;
    bool valid = false;
};