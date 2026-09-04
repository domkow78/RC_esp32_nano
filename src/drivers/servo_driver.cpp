#include "servo_driver.h"

#include "../config/constants.h"
#include "../config/pins.h"

#if defined(ARDUINO_ARCH_ESP32)
#include <Arduino.h>
#endif

namespace {

std::uint32_t pulseToDuty(std::uint16_t pulseUs) {
    const std::uint32_t periodUs = 1000000UL / static_cast<std::uint32_t>(config::kPwmFrequencyHz);
    const std::uint32_t maxDuty = (1UL << config::kPwmResolutionBits) - 1UL;
    return (static_cast<std::uint32_t>(pulseUs) * maxDuty) / periodUs;
}

}  // namespace

void ServoDriver::begin() {
    lastSteering_ = 0;
    lastPulseUs_ = static_cast<std::uint16_t>(config::kServoPulseCenterUs);

#if defined(ARDUINO_ARCH_ESP32)
    ledcSetup(config::kServoPwmChannel, config::kPwmFrequencyHz, config::kPwmResolutionBits);
    ledcAttachPin(config::kPinServoPwm, config::kServoPwmChannel);
    ledcWrite(config::kServoPwmChannel, pulseToDuty(lastPulseUs_));
#endif
}

void ServoDriver::writeSteering(int steering) {
    lastSteering_ = clamp(steering, config::kSteeringMin, config::kSteeringMax);
    lastPulseUs_ = steeringToPulseUs(lastSteering_);

#if defined(ARDUINO_ARCH_ESP32)
    ledcWrite(config::kServoPwmChannel, pulseToDuty(lastPulseUs_));
#endif
}

int ServoDriver::lastSteering() const {
    return lastSteering_;
}

std::uint16_t ServoDriver::lastPulseUs() const {
    return lastPulseUs_;
}

int ServoDriver::clamp(int value, int minValue, int maxValue) {
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

std::uint16_t ServoDriver::steeringToPulseUs(int steering) {
    const int normalized = clamp(steering, config::kSteeringMin, config::kSteeringMax);
    const int span = config::kServoPulseMaxUs - config::kServoPulseCenterUs;
    const int scaled = config::kServoPulseCenterUs + ((normalized * span) / config::kSteeringMax);
    return static_cast<std::uint16_t>(scaled);
}