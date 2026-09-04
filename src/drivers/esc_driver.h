#pragma once

#include <cstdint>

class EscDriver {
public:
    void begin();
    void update();
    void writeCommand(int throttle, bool brake);
    void writeThrottle(int throttle);
    void writeBrake();
    int lastThrottle() const;
    std::uint16_t lastPulseUs() const;
    bool brakeActive() const;
    bool armed() const;

private:
    void applyPulse(std::uint16_t pulseUs);
    static int clamp(int value, int minValue, int maxValue);
    static std::uint16_t throttleToPulseUs(int throttle);
    static unsigned long nowMs();

    int lastThrottle_ = 0;
    std::uint16_t lastPulseUs_ = 0;
    bool brakeActive_ = false;
    bool armed_ = false;
    unsigned long armStartMs_ = 0;
};