#pragma once

#include "../models/vehicle_state.h"
#include "../services/radio_service.h"

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
    void updateOperatingMode();

    RadioService radioService_;
    VehicleState vehicleState_;
    bool started_ = false;
    StartupPhase startupPhase_ = StartupPhase::Idle;
    unsigned long updateCount_ = 0;
};
