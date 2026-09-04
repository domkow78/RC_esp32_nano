#include "nrf24_driver.h"

bool Nrf24Driver::begin() {
    ready_ = true;
    rxLength_ = 0;
    return ready_;
}

bool Nrf24Driver::isReady() const {
    return ready_;
}

bool Nrf24Driver::write(const std::uint8_t* data, std::size_t length) {
    if (!ready_ || !data || length == 0 || length > rxBuffer_.size()) {
        return false;
    }

    if (loopbackEnabled_ && rxLength_ == 0) {
        for (std::size_t index = 0; index < length; ++index) {
            rxBuffer_[index] = data[index];
        }
        rxLength_ = length;
    }

    return true;
}

bool Nrf24Driver::read(std::uint8_t* data, std::size_t bufferSize, std::size_t& outLength) {
    outLength = 0;

    if (!ready_ || !data || rxLength_ == 0 || bufferSize < rxLength_) {
        return false;
    }

    for (std::size_t index = 0; index < rxLength_; ++index) {
        data[index] = rxBuffer_[index];
    }

    outLength = rxLength_;
    rxLength_ = 0;
    return true;
}

bool Nrf24Driver::available() const {
    return rxLength_ > 0;
}

void Nrf24Driver::setLoopbackEnabled(bool enabled) {
    loopbackEnabled_ = enabled;
}