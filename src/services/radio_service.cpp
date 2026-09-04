#include "radio_service.h"

#include <cstring>

namespace {

unsigned long telemetryPeriodTicks() {
    const unsigned long ticks = config::kTelemetryPeriodMs / config::kControlLoopPeriodMs;
    return ticks == 0 ? 1UL : ticks;
}

}  // namespace

void RadioService::writeUInt16Le(std::uint8_t* data, std::uint16_t value) {
    data[0] = static_cast<std::uint8_t>(value & 0xFFU);
    data[1] = static_cast<std::uint8_t>((value >> 8) & 0xFFU);
}

void RadioService::writeInt16Le(std::uint8_t* data, std::int16_t value) {
    writeUInt16Le(data, static_cast<std::uint16_t>(value));
}

void RadioService::writeFloatLe(std::uint8_t* data, float value) {
    std::uint32_t raw = 0;
    std::memcpy(&raw, &value, sizeof(value));
    data[0] = static_cast<std::uint8_t>(raw & 0xFFU);
    data[1] = static_cast<std::uint8_t>((raw >> 8) & 0xFFU);
    data[2] = static_cast<std::uint8_t>((raw >> 16) & 0xFFU);
    data[3] = static_cast<std::uint8_t>((raw >> 24) & 0xFFU);
}

std::int8_t RadioService::clampInt8(int value, int minValue, int maxValue) {
    if (value < minValue) {
        return static_cast<std::int8_t>(minValue);
    }
    if (value > maxValue) {
        return static_cast<std::int8_t>(maxValue);
    }
    return static_cast<std::int8_t>(value);
}

std::int16_t RadioService::readInt16Le(const std::uint8_t* data) {
    return static_cast<std::int16_t>(
        static_cast<std::uint16_t>(data[0]) |
        (static_cast<std::uint16_t>(data[1]) << 8));
}

std::uint16_t RadioService::readUInt16Le(const std::uint8_t* data) {
    return static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(data[0]) |
        (static_cast<std::uint16_t>(data[1]) << 8));
}

float RadioService::readFloatLe(const std::uint8_t* data) {
    std::uint32_t raw = 0;
    raw |= static_cast<std::uint32_t>(data[0]);
    raw |= static_cast<std::uint32_t>(data[1]) << 8;
    raw |= static_cast<std::uint32_t>(data[2]) << 16;
    raw |= static_cast<std::uint32_t>(data[3]) << 24;

    float value = 0.0f;
    std::memcpy(&value, &raw, sizeof(value));
    return value;
}

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
    if ((updateCount_ % telemetryPeriodTicks()) == 0UL) {
        buildTelemetryFrame();
    } else {
        buildStatusFrame();
    }
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

void RadioService::setTelemetryData(const VehicleState& vehicleState) {
    telemetryBatteryVoltage_ = vehicleState.battery.voltage;
    telemetryBatteryCurrent_ = vehicleState.battery.current;
    telemetryImuHeading_ = vehicleState.imu.heading;
    telemetryLidarDistanceCm_ = vehicleState.lidar.distanceCm;

    telemetryRssi_ = static_cast<std::int16_t>(vehicleState.radio.telemetryRssi);
    telemetryPacketLoss_ = vehicleState.radio.telemetryPacketLoss;
    telemetryErrorFlags_ = vehicleState.radio.telemetryErrorFlags;

    telemetryAppliedThrottle_ = static_cast<std::int16_t>(vehicleState.escAppliedThrottle);
    telemetryAppliedSteering_ = clampInt8(vehicleState.servoAppliedSteering, -100, 100);
    telemetryEscBrake_ = vehicleState.escBrakeActive;
    telemetryEscArmed_ = vehicleState.escArmed;
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

void RadioService::buildStatusFrame() {
    currentFrame_.header.type = static_cast<std::uint8_t>(protocol::PacketType::Status);
    currentFrame_.payloadLength = 2;
    currentFrame_.payload[0] = static_cast<std::uint8_t>(failsafeActive_ ? 2U : 1U);

    std::uint8_t flags = 0;
    if (linkAlive_) {
        flags |= 0x01U;
    }
    if (heartbeatTimedOut_) {
        flags |= 0x02U;
    }
    currentFrame_.payload[1] = flags;
}

void RadioService::buildTelemetryFrame() {
    currentFrame_.header.type = static_cast<std::uint8_t>(protocol::PacketType::Telemetry);
    currentFrame_.payloadLength = 24;

    writeFloatLe(currentFrame_.payload.data(), telemetryBatteryVoltage_);
    writeFloatLe(currentFrame_.payload.data() + 4, telemetryBatteryCurrent_);
    writeFloatLe(currentFrame_.payload.data() + 8, telemetryImuHeading_);
    writeFloatLe(currentFrame_.payload.data() + 12, telemetryLidarDistanceCm_);
    writeInt16Le(currentFrame_.payload.data() + 16, telemetryRssi_);
    currentFrame_.payload[18] = telemetryPacketLoss_;
    currentFrame_.payload[19] = telemetryErrorFlags_;
    writeInt16Le(currentFrame_.payload.data() + 20, telemetryAppliedThrottle_);
    currentFrame_.payload[22] = static_cast<std::uint8_t>(telemetryAppliedSteering_);

    std::uint8_t driveFlags = 0;
    if (telemetryEscBrake_) {
        driveFlags |= 0x01U;
    }
    if (telemetryEscArmed_) {
        driveFlags |= 0x02U;
    }
    currentFrame_.payload[23] = driveFlags;
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
        latestPacket_.throttle = static_cast<int>(readInt16Le(frame.payload.data()));
        latestPacket_.steering = static_cast<int>(readInt16Le(frame.payload.data() + 2));

        latestPacket_.lights = frame.payload[5] != 0;
        latestPacket_.emergencyStop = frame.payload[6] != 0;
        return;
    }

    if (currentPacketType_ == protocol::PacketType::Heartbeat && frame.payloadLength >= 4U) {
        latestPacket_.heartbeatUptimeMs = readUInt16Le(frame.payload.data());
        latestPacket_.heartbeatLinkState = frame.payload[2];
        latestPacket_.heartbeatErrorCode = frame.payload[3];
        return;
    }

    if (currentPacketType_ == protocol::PacketType::Status && frame.payloadLength >= 2U) {
        latestPacket_.statusCode = frame.payload[0];
        latestPacket_.statusFlags = frame.payload[1];
        return;
    }

    if (currentPacketType_ == protocol::PacketType::Telemetry && frame.payloadLength >= 20U) {
        latestPacket_.telemetryBatteryVoltage = readFloatLe(frame.payload.data());
        latestPacket_.telemetryBatteryCurrent = readFloatLe(frame.payload.data() + 4);
        latestPacket_.telemetryImuHeading = readFloatLe(frame.payload.data() + 8);
        latestPacket_.telemetryLidarDistanceCm = readFloatLe(frame.payload.data() + 12);
        latestPacket_.telemetryRssi = static_cast<short>(readInt16Le(frame.payload.data() + 16));
        latestPacket_.telemetryPacketLoss = frame.payload[18];
        latestPacket_.telemetryErrorFlags = frame.payload[19];

        if (frame.payloadLength >= 24U) {
            latestPacket_.throttle = static_cast<int>(readInt16Le(frame.payload.data() + 20));
            latestPacket_.steering = static_cast<int>(static_cast<std::int8_t>(frame.payload[22]));

            const std::uint8_t driveFlags = frame.payload[23];
            latestPacket_.telemetryDriveFlags = driveFlags;
        }
    }
}