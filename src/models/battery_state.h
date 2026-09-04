#pragma once

struct BatteryState {
    float voltage = 0.0f;
    float current = 0.0f;
    float percentage = 0.0f;
    bool lowVoltage = false;
    bool criticalVoltage = false;
};