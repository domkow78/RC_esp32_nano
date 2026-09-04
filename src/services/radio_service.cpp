#include "radio_service.h"

void RadioService::begin() {
    updateCount_ = 0;
    currentPacketType_ = protocol::PacketType::Status;
    latestPacket_ = {};
}

void RadioService::update() {
    ++updateCount_;
}

const RadioPacket& RadioService::latestPacket() const {
    return latestPacket_;
}

protocol::PacketType RadioService::currentPacketType() const {
    return currentPacketType_;
}