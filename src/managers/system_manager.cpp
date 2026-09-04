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

void SystemManager::initializeCore() {
}

void SystemManager::initializeServices() {
	radioService_.begin();
}

void SystemManager::initializeApplication() {
}

void SystemManager::runCoreTick() {
}

void SystemManager::runServicesTick() {
	radioService_.update();
}

void SystemManager::runApplicationTick() {
}