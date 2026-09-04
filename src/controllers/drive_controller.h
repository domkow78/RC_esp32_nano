#pragma once

#include "../drivers/esc_driver.h"
#include "../drivers/servo_driver.h"
#include "../models/vehicle_state.h"

class DriveController {
public:
    void begin();
    void update(const VehicleState& vehicleState);
    int appliedThrottle() const;
    int appliedSteering() const;
    bool brakeActive() const;
    bool escArmed() const;

private:
    static int clamp(int value, int minValue, int maxValue);
    static int modeThrottleAbsLimit(OperatingMode mode);
    static int modeSteeringAbsLimit(OperatingMode mode);

    EscDriver escDriver_;
    ServoDriver servoDriver_;
    bool brakeActive_ = false;
};