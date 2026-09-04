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

private:
    static int clamp(int value, int minValue, int maxValue);

    EscDriver escDriver_;
    ServoDriver servoDriver_;
    bool brakeActive_ = false;
};