#include "api.h"

void ApiService::begin(WebSocketPublisher* webSocketPublisher) {
    webSocketPublisher_ = webSocketPublisher;
}

void ApiService::publishTelemetry(const VehicleState& vehicleState) {
    if (!webSocketPublisher_) {
        return;
    }

    webSocketPublisher_->publishTelemetry(vehicleState);
}