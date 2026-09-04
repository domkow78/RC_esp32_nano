#include "esc_driver.h"

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

void EscDriver::begin() {
    lastThrottle_ = 0;
    lastPulseUs_ = static_cast<std::uint16_t>(config::kEscPulseNeutralUs);
    brakeActive_ = false;

#if defined(ARDUINO_ARCH_ESP32)
    ledcSetup(config::kEscPwmChannel, config::kPwmFrequencyHz, config::kPwmResolutionBits);
    ledcAttachPin(config::kPinEscPwm, config::kEscPwmChannel);
    ledcWrite(config::kEscPwmChannel, pulseToDuty(lastPulseUs_));
#endif
}

void EscDriver::writeCommand(int throttle, bool brake) {
    if (brake) {
        writeBrake();
        return;
    }

    writeThrottle(throttle);
}

void EscDriver::writeThrottle(int throttle) {
    lastThrottle_ = clamp(throttle, config::kThrottleMin, config::kThrottleMax);
    brakeActive_ = false;
    lastPulseUs_ = throttleToPulseUs(lastThrottle_);

#if defined(ARDUINO_ARCH_ESP32)
    ledcWrite(config::kEscPwmChannel, pulseToDuty(lastPulseUs_));
#endif
}

void EscDriver::writeBrake() {
    lastThrottle_ = 0;
    brakeActive_ = true;
    lastPulseUs_ = static_cast<std::uint16_t>(config::kEscPulseBrakeUs);

#if defined(ARDUINO_ARCH_ESP32)
    ledcWrite(config::kEscPwmChannel, pulseToDuty(lastPulseUs_));
#endif
}

int EscDriver::lastThrottle() const {
    return lastThrottle_;
}

std::uint16_t EscDriver::lastPulseUs() const {
    return lastPulseUs_;
}

bool EscDriver::brakeActive() const {
    return brakeActive_;
}

int EscDriver::clamp(int value, int minValue, int maxValue) {
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

std::uint16_t EscDriver::throttleToPulseUs(int throttle) {
    const int normalized = clamp(throttle, config::kThrottleMin, config::kThrottleMax);

    if (normalized >= 0) {
        const int spanForward = config::kEscPulseMaxUs - config::kEscPulseNeutralUs;
        const int scaledForward = config::kEscPulseNeutralUs + ((normalized * spanForward) / config::kThrottleMax);
        return static_cast<std::uint16_t>(scaledForward);
    }

    const int spanReverse = config::kEscPulseNeutralUs - config::kEscPulseMinUs;
    const int scaledReverse = config::kEscPulseNeutralUs + ((normalized * spanReverse) / (-config::kThrottleMin));
    const int clampedReverse = clamp(scaledReverse, config::kEscPulseMinUs, config::kEscPulseNeutralUs);
    const int clampedForward = clamp(clampedReverse, config::kEscPulseMinUs, config::kEscPulseMaxUs);
    const int scaled = clampedForward;
    return static_cast<std::uint16_t>(scaled);
}