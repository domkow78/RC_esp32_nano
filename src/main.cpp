#include "managers/system_manager.h"

int main() {
    SystemManager systemManager;

    systemManager.begin();

    while (true) {
        systemManager.update();
    }

    return 0;
}
