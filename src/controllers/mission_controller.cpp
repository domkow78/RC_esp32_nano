#include "mission_controller.h"

void MissionController::begin() {
    safeStopOutputActive_ = false;
}

void MissionController::update(VehicleState& vehicleState) {
    if (vehicleState.mode == OperatingMode::SafeStop) {
        applySafeStopOutputs(vehicleState);
        return;
    }

    applyManualOutputs(vehicleState);
}

bool MissionController::safeStopOutputActive() const {
    return safeStopOutputActive_;
}

void MissionController::applySafeStopOutputs(VehicleState& vehicleState) {
    vehicleState.driveThrottleCommand = 0;
    vehicleState.driveSteeringCommand = 0;
    vehicleState.driveBrakeCommand = true;
    safeStopOutputActive_ = true;
}

void MissionController::applyManualOutputs(VehicleState& vehicleState) {
    vehicleState.driveThrottleCommand = vehicleState.radio.throttle;
    vehicleState.driveSteeringCommand = vehicleState.radio.steering;
    vehicleState.driveBrakeCommand = false;
    safeStopOutputActive_ = false;
}