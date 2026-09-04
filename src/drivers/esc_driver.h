#pragma once

#include <cstdint>

class EscDriver {
public:
    void begin();
    void writeCommand(int throttle, bool brake);
    void writeThrottle(int throttle);
    void writeBrake();
    int lastThrottle() const;
    std::uint16_t lastPulseUs() const;
    bool brakeActive() const;

private:
    static int clamp(int value, int minValue, int maxValue);
    static std::uint16_t throttleToPulseUs(int throttle);

    int lastThrottle_ = 0;
    std::uint16_t lastPulseUs_ = 0;
    bool brakeActive_ = false;
};