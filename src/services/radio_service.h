#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "../config/constants.h"
#include "../drivers/nrf24_driver.h"
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
    bool linkAlive() const;
    bool heartbeatTimedOut() const;
    bool failsafeActive() const;

private:
    void evaluateLinkState();
    void processIncomingFrame(const protocol::PacketFrame& frame);

    Nrf24Driver radioDriver_;
    RadioPacket latestPacket_;
    protocol::PacketFrame currentFrame_;
    protocol::PacketType currentPacketType_ = protocol::PacketType::Status;
    std::array<std::uint8_t, protocol::kMaxPacketBytes> encodedFrameBuffer_{};
    std::array<std::uint8_t, protocol::kMaxPacketBytes> rxBuffer_{};
    std::size_t encodedFrameSize_ = 0;
    bool linkAlive_ = false;
    bool heartbeatTimedOut_ = false;
    bool failsafeActive_ = false;
    std::uint8_t lastReceivedSequence_ = 0;
    unsigned long lastRxUpdateCount_ = 0;
    unsigned long updateCount_ = 0;
};
