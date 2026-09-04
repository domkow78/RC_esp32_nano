#include "system_manager.h"

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
}

void SystemManager::initializeServices() {
	radioService_.begin();
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
}

void SystemManager::runApplicationTick() {
	syncVehicleStateFromRadio();
	updateOperatingMode();
	missionController_.update(vehicleState_);
	driveController_.update(vehicleState_);
	syncVehicleStateFromDrive();
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