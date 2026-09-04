#pragma once

namespace config {

inline constexpr float kBatteryDividerRatio = 4.3f;
inline constexpr float kBatteryAdcReferenceVoltage = 3.3f;
inline constexpr float kBatteryLowVoltage = 10.8f;
inline constexpr float kBatteryCriticalVoltage = 10.2f;
inline constexpr float kTfLunaBrakeDistanceCm = 80.0f;
inline constexpr float kTfLunaEmergencyDistanceCm = 40.0f;
inline constexpr float kImuHeadingHoldGain = 1.0f;

}  // namespace config
