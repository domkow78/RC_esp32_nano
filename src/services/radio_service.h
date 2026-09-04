#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "../config/constants.h"
#include "../drivers/nrf24_driver.h"
#include "../models/radio_packet.h"
#include "../models/vehicle_state.h"
#include "../protocol/vcp.h"

class RadioService {
public:
    void begin();
    void update();
    void setTelemetryData(const VehicleState& vehicleState);

    const RadioPacket& latestPacket() const;
    protocol::PacketType currentPacketType() const;
    const protocol::PacketFrame& currentFrame() const;
    std::size_t encodedFrameSize() const;
    bool linkAlive() const;
    bool heartbeatTimedOut() const;
    bool failsafeActive() const;

private:
    static void writeUInt16Le(std::uint8_t* data, std::uint16_t value);
    static void writeInt16Le(std::uint8_t* data, std::int16_t value);
    static void writeFloatLe(std::uint8_t* data, float value);
    static std::int8_t clampInt8(int value, int minValue, int maxValue);

    static std::int16_t readInt16Le(const std::uint8_t* data);
    static std::uint16_t readUInt16Le(const std::uint8_t* data);
    static float readFloatLe(const std::uint8_t* data);

    void buildStatusFrame();
    void buildTelemetryFrame();
    void evaluateLinkState();
    void processIncomingFrame(const protocol::PacketFrame& frame);

    Nrf24Driver radioDriver_;
    RadioPacket latestPacket_;
    protocol::PacketFrame currentFrame_;
    protocol::PacketType currentPacketType_ = protocol::PacketType::Status;
    std::array<std::uint8_t, protocol::kMaxPacketBytes> encodedFrameBuffer_{};
    std::array<std::uint8_t, protocol::kMaxPacketBytes> rxBuffer_{};
    std::size_t encodedFrameSize_ = 0;

    float telemetryBatteryVoltage_ = 0.0f;
    float telemetryBatteryCurrent_ = 0.0f;
    float telemetryImuHeading_ = 0.0f;
    float telemetryLidarDistanceCm_ = 0.0f;
    std::int16_t telemetryRssi_ = 0;
    std::uint8_t telemetryPacketLoss_ = 0;
    std::uint8_t telemetryErrorFlags_ = 0;
    std::int16_t telemetryAppliedThrottle_ = 0;
    std::int8_t telemetryAppliedSteering_ = 0;
    bool telemetryEscBrake_ = false;
    bool telemetryEscArmed_ = false;

    bool linkAlive_ = false;
    bool heartbeatTimedOut_ = false;
    bool failsafeActive_ = false;
    std::uint8_t lastReceivedSequence_ = 0;
    unsigned long lastRxUpdateCount_ = 0;
    unsigned long updateCount_ = 0;
};
