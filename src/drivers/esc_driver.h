#pragma once

#include <cstdint>

class EscDriver {
public:
    void begin();
    void writeThrottle(int throttle);
    int lastThrottle() const;
    std::uint16_t lastPulseUs() const;

private:
    static int clamp(int value, int minValue, int maxValue);
    static std::uint16_t throttleToPulseUs(int throttle);

    int lastThrottle_ = 0;
    std::uint16_t lastPulseUs_ = 0;
};