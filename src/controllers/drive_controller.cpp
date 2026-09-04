#include "drive_controller.h"

#include "../config/constants.h"

void DriveController::begin() {
    escDriver_.begin();
    servoDriver_.begin();
    brakeActive_ = false;
}

void DriveController::update(const VehicleState& vehicleState) {
    const int throttle = vehicleState.driveBrakeCommand
        ? 0
        : clamp(vehicleState.driveThrottleCommand, config::kThrottleMin, config::kThrottleMax);
    const int steering = clamp(vehicleState.driveSteeringCommand, config::kSteeringMin, config::kSteeringMax);

    escDriver_.writeThrottle(throttle);
    servoDriver_.writeSteering(steering);
    brakeActive_ = vehicleState.driveBrakeCommand;
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