#pragma once

#include "../models/radio_packet.h"
#include "../protocol/vcp.h"

class RadioService {
public:
    void begin();
    void update();

    const RadioPacket& latestPacket() const;
    protocol::PacketType currentPacketType() const;

private:
    RadioPacket latestPacket_;
    protocol::PacketType currentPacketType_ = protocol::PacketType::Status;
    unsigned long updateCount_ = 0;
};
