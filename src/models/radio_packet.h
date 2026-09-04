#pragma once

struct RadioPacket {
    unsigned char sequence = 0;
    unsigned char flags = 0;
    int throttle = 0;
    int steering = 0;
    unsigned short heartbeatUptimeMs = 0;
    unsigned char heartbeatLinkState = 0;
    unsigned char heartbeatErrorCode = 0;
    unsigned char statusCode = 0;
    unsigned char statusFlags = 0;
    float telemetryBatteryVoltage = 0.0f;
    float telemetryBatteryCurrent = 0.0f;
    float telemetryImuHeading = 0.0f;
    float telemetryLidarDistanceCm = 0.0f;
    short telemetryRssi = 0;
    unsigned char telemetryPacketLoss = 0;
    unsigned char telemetryErrorFlags = 0;
    bool lights = false;
    bool emergencyStop = false;
    bool valid = false;
};