#pragma once

struct LidarData {
    float distanceCm = 0.0f;
    float temperatureC = 0.0f;
    bool valid = false;
    bool obstacleDetected = false;
};