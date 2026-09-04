#pragma once

class SystemManager {
public:
    void begin();
    void update();

private:
    void initializeCore();
    void initializeServices();
    void initializeApplication();
    void runCoreTick();
    void runServicesTick();
    void runApplicationTick();

    bool started_ = false;
    unsigned long updateCount_ = 0;
};
