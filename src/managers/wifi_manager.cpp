#include "wifi_manager.h"

#include "../config/hw_config.h"

#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif

void WifiManager::begin() {
#if defined(ARDUINO_ARCH_ESP32)
    if (!config::kUseWifiAp) {
        apActive_ = false;
        stationCount_ = 0;
        return;
    }

    WiFi.mode(WIFI_AP);
    apActive_ = WiFi.softAP(
        config::kWifiApSsid,
        config::kWifiApPassword,
        config::kWifiApChannel,
        config::kWifiApHidden,
        config::kWifiApMaxClients);
    stationCount_ = WiFi.softAPgetStationNum();
#else
    apActive_ = false;
    stationCount_ = 0;
#endif
}

void WifiManager::update() {
#if defined(ARDUINO_ARCH_ESP32)
    if (!apActive_) {
        stationCount_ = 0;
        return;
    }

    stationCount_ = WiFi.softAPgetStationNum();
#else
    stationCount_ = 0;
#endif
}

bool WifiManager::apActive() const {
    return apActive_;
}

int WifiManager::stationCount() const {
    return stationCount_;
}