#pragma once

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

private:
    void initializeCore();
    void initializeServices();
    void initializeApplication();
    void runCoreTick();
    void runServicesTick();
    void runApplicationTick();

    RadioService radioService_;
    bool started_ = false;
    StartupPhase startupPhase_ = StartupPhase::Idle;
    unsigned long updateCount_ = 0;
};
