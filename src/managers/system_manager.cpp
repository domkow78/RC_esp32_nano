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
}

void SystemManager::initializeCore() {
}

void SystemManager::initializeServices() {
}

void SystemManager::initializeApplication() {
}