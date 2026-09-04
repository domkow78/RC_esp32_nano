#pragma once

namespace config {

inline constexpr unsigned long kControlLoopPeriodMs = 20;
inline constexpr unsigned long kHeartbeatPeriodMs = 50;
inline constexpr unsigned long kTelemetryPeriodMs = 100;
inline constexpr unsigned long kStatusPeriodMs = 500;
inline constexpr unsigned long kFailsafeTimeoutMs = 500;
inline constexpr unsigned long kLinkLossWarningMs = 100;
inline constexpr unsigned long kLinkHoldMs = 200;
inline constexpr unsigned long kLinkNeutralMs = 300;

inline constexpr int kRadioPayloadMaxBytes = 32;
inline constexpr int kControlDeadzone = 8;
inline constexpr int kThrottleMin = 0;
inline constexpr int kThrottleMax = 255;
inline constexpr int kSteeringMin = -100;
inline constexpr int kSteeringMax = 100;

inline constexpr int kPwmFrequencyHz = 50;
inline constexpr int kPwmResolutionBits = 16;
inline constexpr int kEscPulseMinUs = 1000;
inline constexpr int kEscPulseMaxUs = 2000;
inline constexpr int kEscPulseNeutralUs = 1500;
inline constexpr int kServoPulseMinUs = 1000;
inline constexpr int kServoPulseMaxUs = 2000;
inline constexpr int kServoPulseCenterUs = 1500;
inline constexpr int kEscPwmChannel = 0;
inline constexpr int kServoPwmChannel = 1;

}  // namespace config
