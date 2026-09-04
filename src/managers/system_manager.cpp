#include "system_manager.h"

void SystemManager::begin() {
	if (started_) {
		return;
	}

	initializeCore();
	initializeServices();
	initializeApplication();

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

void SystemManager::initializeCore() {
}

void SystemManager::initializeServices() {
}

void SystemManager::initializeApplication() {
}

void SystemManager::runCoreTick() {
}

void SystemManager::runServicesTick() {
}

void SystemManager::runApplicationTick() {
}