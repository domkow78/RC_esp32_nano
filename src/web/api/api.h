#pragma once

#include "../websocket/websocket.h"

class ApiService {
public:
	void begin(WebSocketPublisher* webSocketPublisher);
	void publishTelemetry(const VehicleState& vehicleState);

private:
	WebSocketPublisher* webSocketPublisher_ = nullptr;
};
