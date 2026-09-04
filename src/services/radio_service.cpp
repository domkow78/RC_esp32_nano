#include "radio_service.h"

void RadioService::begin() {
    updateCount_ = 0;
    currentPacketType_ = protocol::PacketType::Status;
    latestPacket_ = {};
    currentFrame_ = {};
    currentFrame_.header.type = static_cast<std::uint8_t>(currentPacketType_);
    currentFrame_.header.version = protocol::kVersion;
    currentFrame_.header.flags = 0;
    currentFrame_.payloadLength = 0;
    encodedFrameSize_ = 0;

    protocol::encodePacket(currentFrame_, encodedFrameBuffer_.data(), encodedFrameBuffer_.size(), encodedFrameSize_);
}

void RadioService::update() {
    ++updateCount_;

    currentFrame_.header.sequence = static_cast<std::uint8_t>(updateCount_ & 0xFFU);
    currentFrame_.header.type = static_cast<std::uint8_t>(currentPacketType_);
    protocol::encodePacket(currentFrame_, encodedFrameBuffer_.data(), encodedFrameBuffer_.size(), encodedFrameSize_);
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