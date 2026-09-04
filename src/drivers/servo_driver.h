#pragma once

#include <cstdint>

class ServoDriver {
public:
    void begin();
    void writeSteering(int steering);
    int lastSteering() const;
    std::uint16_t lastPulseUs() const;

private:
    static int clamp(int value, int minValue, int maxValue);
    static std::uint16_t steeringToPulseUs(int steering);

    int lastSteering_ = 0;
    std::uint16_t lastPulseUs_ = 0;
};