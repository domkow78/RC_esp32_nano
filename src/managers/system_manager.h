#pragma once

#include "../controllers/drive_controller.h"
#include "../controllers/mission_controller.h"
#include "../models/vehicle_state.h"
#include "../services/radio_service.h"
#include "../managers/wifi_manager.h"
#include "../web/api/api.h"
#include "../web/websocket/websocket.h"

class SystemManager {
public:
    enum class StartupPhase {
        Idle,
        Core,
        Services,
        Application,
        Running
    };

    void begin();
    void update();
    bool isStarted() const;
    StartupPhase startupPhase() const;
    unsigned long updateCount() const;
    const VehicleState& vehicleState() const;

private:
    void initializeCore();
    void initializeServices();
    void initializeApplication();
    void runCoreTick();
    void runServicesTick();
    void runApplicationTick();
    void syncVehicleStateFromRadio();
    void syncVehicleStateFromDrive();
    void syncVehicleStateFromWifi();
    void updateOperatingMode();
    void updateDiagnostics();

    ApiService apiService_;
    DriveController driveController_;
    MissionController missionController_;
    RadioService radioService_;
    WifiManager wifiManager_;
    WebSocketPublisher webSocketPublisher_;
    VehicleState vehicleState_;
    OperatingMode lastLoggedMode_ = OperatingMode::Boot;
    bool lastLoggedFailsafe_ = false;
    unsigned char lastLoggedErrorCode_ = 0;
    bool started_ = false;
    StartupPhase startupPhase_ = StartupPhase::Idle;
    unsigned long updateCount_ = 0;
};
