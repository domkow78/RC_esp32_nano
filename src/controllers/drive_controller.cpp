#include "drive_controller.h"

#include "../config/constants.h"

void DriveController::begin() {
    escDriver_.begin();
    servoDriver_.begin();
    brakeActive_ = false;
}

void DriveController::update(const VehicleState& vehicleState) {
    escDriver_.update();

    const int throttleLimit = modeThrottleAbsLimit(vehicleState.mode);
    const int steeringLimit = modeSteeringAbsLimit(vehicleState.mode);

    const int throttleMin = -throttleLimit;
    const int throttleMax = throttleLimit;
    const int steeringMin = -steeringLimit;
    const int steeringMax = steeringLimit;

    const int throttle = clamp(vehicleState.driveThrottleCommand, throttleMin, throttleMax);
    const int steering = clamp(vehicleState.driveSteeringCommand, steeringMin, steeringMax);

    escDriver_.writeCommand(throttle, vehicleState.driveBrakeCommand);
    servoDriver_.writeSteering(steering);
    brakeActive_ = escDriver_.brakeActive();
}

int DriveController::appliedThrottle() const {
    return escDriver_.lastThrottle();
}

int DriveController::appliedSteering() const {
    return servoDriver_.lastSteering();
}

bool DriveController::brakeActive() const {
    return brakeActive_;
}

int DriveController::clamp(int value, int minValue, int maxValue) {
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

int DriveController::modeThrottleAbsLimit(OperatingMode mode) {
    if (mode == OperatingMode::Auto) {
        return config::kAutoThrottleAbsMax;
    }
    if (mode == OperatingMode::SemiAuto) {
        return config::kSemiAutoThrottleAbsMax;
    }
    return config::kManualThrottleAbsMax;
}

int DriveController::modeSteeringAbsLimit(OperatingMode mode) {
    if (mode == OperatingMode::Auto) {
        return config::kAutoSteeringAbsMax;
    }
    if (mode == OperatingMode::SemiAuto) {
        return config::kSemiAutoSteeringAbsMax;
    }
    return config::kManualSteeringAbsMax;
}