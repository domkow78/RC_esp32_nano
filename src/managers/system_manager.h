#pragma once

class SystemManager {
public:
    void begin();
    void update();

private:
    void initializeCore();
    void initializeServices();
    void initializeApplication();

    bool started_ = false;
};
