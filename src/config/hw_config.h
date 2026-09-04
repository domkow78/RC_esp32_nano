#pragma once

namespace config {

inline constexpr bool kUseEsp32Nano = true;
inline constexpr bool kUseNrf24 = true;
inline constexpr bool kUseMpu6050 = true;
inline constexpr bool kUseTfLuna = true;
inline constexpr bool kUseSsd1306 = true;
inline constexpr bool kUseWs2812 = true;
inline constexpr bool kUseWifiAp = true;

inline constexpr unsigned long kSerialBaudRate = 115200;

inline constexpr const char* kWifiApSsid = "RC_ESP32_NANO_V3";
inline constexpr const char* kWifiApPassword = "rcnano2026";
inline constexpr int kWifiApChannel = 6;
inline constexpr bool kWifiApHidden = false;
inline constexpr int kWifiApMaxClients = 4;

}  // namespace config
