#pragma once

#include "../models/vehicle_state.h"

class MissionController {
public:
    void begin();
    void update(VehicleState& vehicleState);
    bool safeStopOutputActive() const;

private:
    void applySafeStopOutputs(VehicleState& vehicleState);
    void applyManualOutputs(VehicleState& vehicleState);

    bool safeStopOutputActive_ = false;
};
