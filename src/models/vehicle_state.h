#pragma once

#include "battery_state.h"
#include "configuration.h"
#include "imu_data.h"
#include "lidar_data.h"
#include "radio_packet.h"

enum class OperatingMode {
	Boot,
	Init,
	Ready,
	Manual,
	SemiAuto,
	Auto,
	Error,
	SafeStop
};

struct VehicleState {
	BatteryState battery;
	Configuration configuration;
	ImuData imu;
	LidarData lidar;
	RadioPacket radio;
	int driveThrottleCommand = 0;
	int driveSteeringCommand = 0;
	bool driveBrakeCommand = false;
	OperatingMode mode = OperatingMode::Boot;
	bool failsafeActive = false;
};
