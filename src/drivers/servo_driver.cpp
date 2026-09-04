#include "servo_driver.h"

void ServoDriver::begin() {
    lastSteering_ = 0;
}

void ServoDriver::writeSteering(int steering) {
    lastSteering_ = steering;
}

int ServoDriver::lastSteering() const {
    return lastSteering_;
}