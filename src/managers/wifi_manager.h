#pragma once

class WifiManager {
public:
    void begin();
    void update();

    bool apActive() const;
    int stationCount() const;

private:
    bool apActive_ = false;
    int stationCount_ = 0;
};