#pragma once

#include <cstddef>

#include "../models/radio_packet.h"
#include "../protocol/vcp.h"

class RadioService {
public:
    void begin();
    void update();

    const RadioPacket& latestPacket() const;
    protocol::PacketType currentPacketType() const;
    const protocol::PacketFrame& currentFrame() const;
    std::size_t encodedFrameSize() const;

private:
    RadioPacket latestPacket_;
    protocol::PacketFrame currentFrame_;
    protocol::PacketType currentPacketType_ = protocol::PacketType::Status;
    std::array<std::uint8_t, protocol::kMaxPacketBytes> encodedFrameBuffer_{};
    std::size_t encodedFrameSize_ = 0;
    unsigned long updateCount_ = 0;
};
