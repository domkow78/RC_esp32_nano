#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "../utils/crc.h"

namespace protocol {

enum class PacketType : std::uint8_t {
    Control = 0,
    Telemetry = 1,
    Heartbeat = 2,
    Status = 3,
    ConfigGet = 4,
    ConfigSet = 5,
    Debug = 6,
    Bootloader = 7
};

enum class PacketFlag : std::uint8_t {
    None = 0x00,
    AckRequested = 0x01,
    EmergencyStop = 0x02,
    FailsafeActive = 0x04,
    Reserved = 0x80
};

inline constexpr std::uint8_t kSyncByte = 0xAA;
inline constexpr std::uint8_t kVersion = 1;
inline constexpr std::uint8_t kMaxPayloadBytes = 24;
inline constexpr std::uint8_t kMaxPacketBytes = 32;

struct PacketHeader {
    std::uint8_t sync = kSyncByte;
    std::uint8_t type = 0;
    std::uint8_t version = kVersion;
    std::uint8_t sequence = 0;
    std::uint8_t flags = 0;
};

struct PacketFrame {
    PacketHeader header;
    std::uint8_t payloadLength = 0;
    std::array<std::uint8_t, kMaxPayloadBytes> payload{};
    std::uint16_t crc = 0;
};

struct ControlPayload {
    std::int16_t throttle = 0;
    std::int16_t steering = 0;
    std::uint8_t mode = 0;
    std::uint8_t aux = 0;
    bool emergencyStop = false;
};

struct HeartbeatPayload {
    std::uint16_t uptimeMs = 0;
    std::uint8_t linkState = 0;
    std::uint8_t errorCode = 0;
};

struct TelemetryPayload {
    float batteryVoltage = 0.0f;
    float batteryCurrent = 0.0f;
    float imuHeading = 0.0f;
    float lidarDistanceCm = 0.0f;
    std::int16_t rssi = 0;
    std::uint8_t packetLoss = 0;
    std::uint8_t errorFlags = 0;
};

inline std::size_t encodedSize(const PacketFrame& frame) {
    return 6U + frame.payloadLength + 2U;
}

inline bool isValidPayloadLength(std::uint8_t payloadLength) {
    return payloadLength <= kMaxPayloadBytes;
}

inline std::uint16_t calculateCrc(const PacketFrame& frame) {
    std::array<std::uint8_t, 6U + kMaxPayloadBytes> crcBuffer{};
    std::size_t offset = 0;

    crcBuffer[offset++] = frame.header.sync;
    crcBuffer[offset++] = frame.header.type;
    crcBuffer[offset++] = frame.header.version;
    crcBuffer[offset++] = frame.header.sequence;
    crcBuffer[offset++] = frame.header.flags;
    crcBuffer[offset++] = frame.payloadLength;

    for (std::size_t index = 0; index < frame.payloadLength; ++index) {
        crcBuffer[offset++] = frame.payload[index];
    }

    return crc16(crcBuffer.data(), offset);
}

inline bool encodePacket(const PacketFrame& frame, std::uint8_t* buffer, std::size_t bufferSize, std::size_t& writtenBytes) {
    if (!buffer || !isValidPayloadLength(frame.payloadLength)) {
        writtenBytes = 0;
        return false;
    }

    const std::size_t requiredSize = encodedSize(frame);
    if (bufferSize < requiredSize) {
        writtenBytes = 0;
        return false;
    }

    std::size_t offset = 0;
    buffer[offset++] = frame.header.sync;
    buffer[offset++] = frame.header.type;
    buffer[offset++] = frame.header.version;
    buffer[offset++] = frame.header.sequence;
    buffer[offset++] = frame.header.flags;
    buffer[offset++] = frame.payloadLength;

    for (std::size_t index = 0; index < frame.payloadLength; ++index) {
        buffer[offset++] = frame.payload[index];
    }

    const std::uint16_t crc = calculateCrc(frame);
    buffer[offset++] = static_cast<std::uint8_t>((crc >> 8) & 0xFFU);
    buffer[offset++] = static_cast<std::uint8_t>(crc & 0xFFU);

    writtenBytes = offset;
    return true;
}

inline bool decodePacket(const std::uint8_t* buffer, std::size_t bufferSize, PacketFrame& frame) {
    if (!buffer || bufferSize < 8U) {
        return false;
    }

    std::size_t offset = 0;
    frame.header.sync = buffer[offset++];
    frame.header.type = buffer[offset++];
    frame.header.version = buffer[offset++];
    frame.header.sequence = buffer[offset++];
    frame.header.flags = buffer[offset++];
    frame.payloadLength = buffer[offset++];

    if (frame.header.sync != kSyncByte || frame.header.version != kVersion || !isValidPayloadLength(frame.payloadLength)) {
        return false;
    }

    const std::size_t expectedSize = 6U + frame.payloadLength + 2U;
    if (bufferSize < expectedSize) {
        return false;
    }

    for (std::size_t index = 0; index < frame.payloadLength; ++index) {
        frame.payload[index] = buffer[offset++];
    }

    frame.crc = static_cast<std::uint16_t>(buffer[offset++] << 8);
    frame.crc |= static_cast<std::uint16_t>(buffer[offset++]);

    return frame.crc == calculateCrc(frame);
}

}  // namespace protocol