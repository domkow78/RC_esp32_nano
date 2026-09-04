#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

class Nrf24Driver {
public:
    bool begin();
    bool isReady() const;
    bool write(const std::uint8_t* data, std::size_t length);
    bool read(std::uint8_t* data, std::size_t bufferSize, std::size_t& outLength);
    bool available() const;
    void setLoopbackEnabled(bool enabled);

private:
    bool ready_ = false;
    bool loopbackEnabled_ = true;
    std::array<std::uint8_t, 32> rxBuffer_{};
    std::size_t rxLength_ = 0;
};
