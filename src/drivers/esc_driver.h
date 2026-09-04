#pragma once

class EscDriver {
public:
    void begin();
    void writeThrottle(int throttle);
    int lastThrottle() const;

private:
    int lastThrottle_ = 0;
};