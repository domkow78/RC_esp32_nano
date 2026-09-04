#include "radio_service.h"

void RadioService::begin() {
    updateCount_ = 0;
    currentPacketType_ = protocol::PacketType::Status;
    latestPacket_ = {};
    linkAlive_ = radioDriver_.begin();
    radioDriver_.setIrqEnabled(true);
    heartbeatTimedOut_ = false;
    failsafeActive_ = false;
    lastRxUpdateCount_ = 0;
    currentFrame_ = {};
    currentFrame_.header.type = static_cast<std::uint8_t>(currentPacketType_);
    currentFrame_.header.version = protocol::kVersion;
    currentFrame_.header.flags = 0;
    currentFrame_.payloadLength = 0;
    lastReceivedSequence_ = 0;
    encodedFrameSize_ = 0;

    linkAlive_ = linkAlive_ && protocol::encodePacket(currentFrame_, encodedFrameBuffer_.data(), encodedFrameBuffer_.size(), encodedFrameSize_);
}

void RadioService::update() {
    ++updateCount_;

    currentFrame_.header.sequence = static_cast<std::uint8_t>(updateCount_ & 0xFFU);
    currentFrame_.header.type = static_cast<std::uint8_t>(currentPacketType_);
    currentFrame_.header.flags = 0;

    if (!protocol::encodePacket(currentFrame_, encodedFrameBuffer_.data(), encodedFrameBuffer_.size(), encodedFrameSize_)) {
        linkAlive_ = false;
        evaluateLinkState();
        return;
    }

    if (!radioDriver_.write(encodedFrameBuffer_.data(), encodedFrameSize_)) {
        linkAlive_ = false;
        evaluateLinkState();
        return;
    }

    std::size_t receivedLength = 0;
    if (!radioDriver_.hasPendingInterrupt()) {
        evaluateLinkState();
        return;
    }

    if (!radioDriver_.read(rxBuffer_.data(), rxBuffer_.size(), receivedLength)) {
        radioDriver_.clearPendingInterrupt();
        evaluateLinkState();
        return;
    }

    protocol::PacketFrame decodedFrame;
    if (!protocol::decodePacket(rxBuffer_.data(), receivedLength, decodedFrame)) {
        evaluateLinkState();
        return;
    }

    lastRxUpdateCount_ = updateCount_;
    linkAlive_ = true;
    heartbeatTimedOut_ = false;
    failsafeActive_ = false;
    processIncomingFrame(decodedFrame);
}

const RadioPacket& RadioService::latestPacket() const {
    return latestPacket_;
}

protocol::PacketType RadioService::currentPacketType() const {
    return currentPacketType_;
}

const protocol::PacketFrame& RadioService::currentFrame() const {
    return currentFrame_;
}

std::size_t RadioService::encodedFrameSize() const {
    return encodedFrameSize_;
}

bool RadioService::linkAlive() const {
    return linkAlive_;
}

bool RadioService::heartbeatTimedOut() const {
    return heartbeatTimedOut_;
}

bool RadioService::failsafeActive() const {
    return failsafeActive_;
}

void RadioService::evaluateLinkState() {
    const unsigned long elapsedUpdates = updateCount_ - lastRxUpdateCount_;
    const unsigned long elapsedMs = elapsedUpdates * config::kControlLoopPeriodMs;

    heartbeatTimedOut_ = elapsedMs >= config::kFailsafeTimeoutMs;

    if (!heartbeatTimedOut_) {
        return;
    }

    failsafeActive_ = true;
    linkAlive_ = false;
    currentFrame_.header.flags |= static_cast<std::uint8_t>(protocol::PacketFlag::FailsafeActive);
    latestPacket_.emergencyStop = true;
    latestPacket_.valid = false;
}

void RadioService::processIncomingFrame(const protocol::PacketFrame& frame) {
    lastReceivedSequence_ = frame.header.sequence;
    currentPacketType_ = static_cast<protocol::PacketType>(frame.header.type);

    latestPacket_.sequence = frame.header.sequence;
    latestPacket_.flags = frame.header.flags;
    latestPacket_.valid = true;

    if (currentPacketType_ == protocol::PacketType::Control && frame.payloadLength >= 7U) {
        latestPacket_.throttle = static_cast<int>(static_cast<std::int16_t>(
            static_cast<std::uint16_t>(frame.payload[0]) |
            (static_cast<std::uint16_t>(frame.payload[1]) << 8)));

        latestPacket_.steering = static_cast<int>(static_cast<std::int16_t>(
            static_cast<std::uint16_t>(frame.payload[2]) |
            (static_cast<std::uint16_t>(frame.payload[3]) << 8)));

        latestPacket_.lights = frame.payload[5] != 0;
        latestPacket_.emergencyStop = frame.payload[6] != 0;
    }
}