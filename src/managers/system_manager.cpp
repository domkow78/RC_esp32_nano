#include "system_manager.h"

#include <cstdio>

void SystemManager::begin() {
	if (started_) {
		return;
	}

	startupPhase_ = StartupPhase::Core;
	initializeCore();
	startupPhase_ = StartupPhase::Services;
	initializeServices();
	startupPhase_ = StartupPhase::Application;
	initializeApplication();

	startupPhase_ = StartupPhase::Running;
	started_ = true;
}

void SystemManager::update() {
	if (!started_) {
		return;
	}

	++updateCount_;

	runCoreTick();
	runServicesTick();
	runApplicationTick();
}

bool SystemManager::isStarted() const {
	return started_;
}

SystemManager::StartupPhase SystemManager::startupPhase() const {
	return startupPhase_;
}

unsigned long SystemManager::updateCount() const {
	return updateCount_;
}

const VehicleState& SystemManager::vehicleState() const {
	return vehicleState_;
}

void SystemManager::initializeCore() {
	vehicleState_.mode = OperatingMode::Init;
	vehicleState_.systemErrorCode = 0;
	vehicleState_.logCount = 1;
	std::snprintf(vehicleState_.lastLog, sizeof(vehicleState_.lastLog), "INIT");
	lastLoggedMode_ = vehicleState_.mode;
	lastLoggedFailsafe_ = vehicleState_.failsafeActive;
	lastLoggedErrorCode_ = vehicleState_.systemErrorCode;
}

void SystemManager::initializeServices() {
	radioService_.begin();
	wifiManager_.begin();
	webSocketPublisher_.begin();
	apiService_.begin(&webSocketPublisher_);
}

void SystemManager::initializeApplication() {
	vehicleState_.mode = OperatingMode::Ready;
	missionController_.begin();
	driveController_.begin();
}

void SystemManager::runCoreTick() {
}

void SystemManager::runServicesTick() {
	radioService_.update();
	wifiManager_.update();
}

void SystemManager::runApplicationTick() {
	syncVehicleStateFromRadio();
	updateOperatingMode();
	missionController_.update(vehicleState_);
	driveController_.update(vehicleState_);
	syncVehicleStateFromDrive();
	syncVehicleStateFromWifi();
	updateDiagnostics();
	radioService_.setTelemetryData(vehicleState_);
	apiService_.publishTelemetry(vehicleState_);
}

void SystemManager::syncVehicleStateFromRadio() {
	vehicleState_.radio = radioService_.latestPacket();
	vehicleState_.failsafeActive = radioService_.failsafeActive();
}

void SystemManager::syncVehicleStateFromDrive() {
	vehicleState_.escAppliedThrottle = driveController_.appliedThrottle();
	vehicleState_.servoAppliedSteering = driveController_.appliedSteering();
	vehicleState_.escBrakeActive = driveController_.brakeActive();
	vehicleState_.escArmed = driveController_.escArmed();
	vehicleState_.webSocketServerActive = webSocketPublisher_.serverActive();
}

void SystemManager::syncVehicleStateFromWifi() {
	vehicleState_.wifiApActive = wifiManager_.apActive();
	vehicleState_.wifiStationCount = wifiManager_.stationCount();
}

void SystemManager::updateOperatingMode() {
	if (vehicleState_.failsafeActive || vehicleState_.radio.emergencyStop) {
		vehicleState_.mode = OperatingMode::SafeStop;
		return;
	}

	if (vehicleState_.mode == OperatingMode::Boot ||
		vehicleState_.mode == OperatingMode::Init ||
		vehicleState_.mode == OperatingMode::Ready ||
		vehicleState_.mode == OperatingMode::SafeStop) {
		vehicleState_.mode = OperatingMode::Manual;
	}
}

void SystemManager::updateDiagnostics() {
	unsigned char errorCode = 0;

	if (vehicleState_.failsafeActive) {
		errorCode = 2;
	} else if (vehicleState_.radio.heartbeatErrorCode != 0U) {
		errorCode = vehicleState_.radio.heartbeatErrorCode;
	} else if (vehicleState_.radio.statusCode != 0U) {
		errorCode = vehicleState_.radio.statusCode;
	}

	vehicleState_.systemErrorCode = errorCode;

	const bool modeChanged = vehicleState_.mode != lastLoggedMode_;
	const bool failsafeChanged = vehicleState_.failsafeActive != lastLoggedFailsafe_;
	const bool errorChanged = vehicleState_.systemErrorCode != lastLoggedErrorCode_;

	if (!modeChanged && !failsafeChanged && !errorChanged) {
		return;
	}

	vehicleState_.logCount += 1;
	std::snprintf(
		vehicleState_.lastLog,
		sizeof(vehicleState_.lastLog),
		"mode=%d failsafe=%d err=%u",
		static_cast<int>(vehicleState_.mode),
		vehicleState_.failsafeActive ? 1 : 0,
		static_cast<unsigned int>(vehicleState_.systemErrorCode));

	lastLoggedMode_ = vehicleState_.mode;
	lastLoggedFailsafe_ = vehicleState_.failsafeActive;
	lastLoggedErrorCode_ = vehicleState_.systemErrorCode;
}