#include "websocket.h"

#include <cstdio>

void WebSocketPublisher::begin() {
    lastPayload_[0] = '\0';
    publishCount_ = 0;
}

void WebSocketPublisher::publishTelemetry(const VehicleState& vehicleState) {
    const int mode = static_cast<int>(vehicleState.mode);
    const int failsafe = vehicleState.failsafeActive ? 1 : 0;
    const int escArmed = vehicleState.escArmed ? 1 : 0;
    const int escBrake = vehicleState.escBrakeActive ? 1 : 0;

    std::snprintf(
        lastPayload_,
        sizeof(lastPayload_),
        "{\"mode\":%d,\"failsafe\":%d,\"batteryV\":%.2f,\"imuHeading\":%.2f,\"lidarCm\":%.2f,\"throttleCmd\":%d,\"steeringCmd\":%d,\"escThrottle\":%d,\"servoSteering\":%d,\"escArmed\":%d,\"escBrake\":%d}",
        mode,
        failsafe,
        static_cast<double>(vehicleState.battery.voltage),
        static_cast<double>(vehicleState.imu.heading),
        static_cast<double>(vehicleState.lidar.distanceCm),
        vehicleState.driveThrottleCommand,
        vehicleState.driveSteeringCommand,
        vehicleState.escAppliedThrottle,
        vehicleState.servoAppliedSteering,
        escArmed,
        escBrake);

    ++publishCount_;
}

const char* WebSocketPublisher::lastPayload() const {
    return lastPayload_;
}

unsigned long WebSocketPublisher::publishCount() const {
    return publishCount_;
}