#pragma once

#include "../../models/vehicle_state.h"

class WebSocketPublisher {
public:
	void begin();
	void publishTelemetry(const VehicleState& vehicleState);

	const char* lastPayload() const;
	unsigned long publishCount() const;

private:
	char lastPayload_[512] = {0};
	unsigned long publishCount_ = 0;
};
