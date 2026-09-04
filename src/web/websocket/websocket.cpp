#include "websocket.h"

#include <cstdio>

#if defined(ARDUINO_ARCH_ESP32) && __has_include(<ESPAsyncWebServer.h>)
#define RC_HAS_ASYNC_WS 1
#include <ESPAsyncWebServer.h>
#else
#define RC_HAS_ASYNC_WS 0
#endif

#if RC_HAS_ASYNC_WS
namespace {

AsyncWebServer g_server(80);
AsyncWebSocket g_webSocket("/ws");
bool g_serverInitialized = false;

void initializeServerOnce() {
    if (g_serverInitialized) {
        return;
    }

    g_server.on("/health", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(200, "text/plain", "ok");
    });
    g_server.addHandler(&g_webSocket);
    g_server.begin();
    g_serverInitialized = true;
}

}  // namespace
#endif

void WebSocketPublisher::begin() {
    lastPayload_[0] = '\0';
    publishCount_ = 0;

#if RC_HAS_ASYNC_WS
    initializeServerOnce();
    serverActive_ = true;
#else
    serverActive_ = false;
#endif
}

void WebSocketPublisher::publishTelemetry(const VehicleState& vehicleState) {
    const int mode = static_cast<int>(vehicleState.mode);
    const int failsafe = vehicleState.failsafeActive ? 1 : 0;
    const int escArmed = vehicleState.escArmed ? 1 : 0;
    const int escBrake = vehicleState.escBrakeActive ? 1 : 0;

    std::snprintf(
        lastPayload_,
        sizeof(lastPayload_),
        "{\"mode\":%d,\"failsafe\":%d,\"errorCode\":%u,\"logCount\":%lu,\"lastLog\":\"%s\",\"wifiApActive\":%d,\"wifiClients\":%d,\"batteryV\":%.2f,\"imuHeading\":%.2f,\"lidarCm\":%.2f,\"throttleCmd\":%d,\"steeringCmd\":%d,\"escThrottle\":%d,\"servoSteering\":%d,\"escArmed\":%d,\"escBrake\":%d}",
        mode,
        failsafe,
        static_cast<unsigned int>(vehicleState.systemErrorCode),
        vehicleState.logCount,
        vehicleState.lastLog,
        vehicleState.wifiApActive ? 1 : 0,
        vehicleState.wifiStationCount,
        static_cast<double>(vehicleState.battery.voltage),
        static_cast<double>(vehicleState.imu.heading),
        static_cast<double>(vehicleState.lidar.distanceCm),
        vehicleState.driveThrottleCommand,
        vehicleState.driveSteeringCommand,
        vehicleState.escAppliedThrottle,
        vehicleState.servoAppliedSteering,
        escArmed,
        escBrake);

#if RC_HAS_ASYNC_WS
    if (serverActive_) {
        g_webSocket.textAll(lastPayload_);
    }
#endif

    ++publishCount_;
}

const char* WebSocketPublisher::lastPayload() const {
    return lastPayload_;
}

unsigned long WebSocketPublisher::publishCount() const {
    return publishCount_;
}

bool WebSocketPublisher::serverActive() const {
    return serverActive_;
}