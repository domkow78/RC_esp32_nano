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
	int escAppliedThrottle = 0;
	int servoAppliedSteering = 0;
	bool escBrakeActive = false;
	bool escArmed = false;
	bool webSocketServerActive = false;
	unsigned char systemErrorCode = 0;
	unsigned long logCount = 0;
	char lastLog[96] = "BOOT";
	OperatingMode mode = OperatingMode::Boot;
	bool failsafeActive = false;
};
