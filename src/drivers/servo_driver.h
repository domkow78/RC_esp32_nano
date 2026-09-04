#pragma once

class ServoDriver {
public:
    void begin();
    void writeSteering(int steering);
    int lastSteering() const;

private:
    int lastSteering_ = 0;
};