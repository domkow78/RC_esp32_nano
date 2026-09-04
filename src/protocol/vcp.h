#pragma once

#include <cstdint>

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

}  // namespace protocol