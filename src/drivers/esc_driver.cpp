#include "esc_driver.h"

void EscDriver::begin() {
    lastThrottle_ = 0;
}

void EscDriver::writeThrottle(int throttle) {
    lastThrottle_ = throttle;
}

int EscDriver::lastThrottle() const {
    return lastThrottle_;
}