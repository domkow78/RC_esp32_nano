# System Runtime Description (Code-Based)

This document describes how the system currently works based on the implementation in src/.
It reflects actual runtime behavior in code, not only target architecture assumptions.

## 1. Entry Point and Main Loop

File: src/main.cpp

- The firmware creates a SystemManager instance.
- It calls begin() once.
- It then runs an infinite loop calling update().

Runtime pattern:

1. startup initialization
2. cyclic service processing
3. cyclic application processing

## 2. System Manager Lifecycle

Files:
- src/managers/system_manager.h
- src/managers/system_manager.cpp

SystemManager has startup phases:
- Idle
- Core
- Services
- Application
- Running

begin() sequence:
1. initializeCore()
2. initializeServices()
3. initializeApplication()
4. switch to Running

update() sequence:
1. runCoreTick()
2. runServicesTick()
3. runApplicationTick()

## 3. Core State Object

File: src/models/vehicle_state.h

VehicleState is the shared runtime snapshot. It currently contains:

- vehicle mode and failsafe status
- radio input data
- drive command values
- applied drive outputs
- ESC arming and brake status
- Wi-Fi AP diagnostics
- system error code
- lightweight log state (counter + last log string)

This object is synchronized in SystemManager from multiple subsystems.

## 4. Service Layer Behavior

### 4.1 RadioService

Files:
- src/services/radio_service.h
- src/services/radio_service.cpp

Responsibilities:
- build outgoing VCP frames (Status/Telemetry)
- encode/decode packets
- monitor heartbeat timeout
- set failsafeActive when link supervision fails

Cycle behavior:
- increments update counter
- every telemetry period sends Telemetry frame
- in other ticks sends Status frame
- writes frame through Nrf24Driver
- on received frame interrupt, decodes and updates latestPacket

Fail-safe behavior:
- if no valid RX for configured timeout, marks heartbeatTimedOut and failsafeActive

Telemetry payload currently includes:
- battery, imu, lidar placeholders
- radio quality fields
- applied drive outputs
- ESC armed/brake flags

### 4.2 WifiManager

Files:
- src/managers/wifi_manager.h
- src/managers/wifi_manager.cpp

Responsibilities:
- start Wi-Fi AP on ESP32 (if enabled)
- monitor connected station count

When built for ESP32:
- AP mode is enabled via WiFi.softAP(...)
- AP status and client count are updated each cycle

On non-ESP32 builds:
- AP remains inactive as fallback behavior

## 5. Application Layer Behavior

### 5.1 Mode Decision

File: src/managers/system_manager.cpp

SystemManager decides operation mode using current state:
- enters SafeStop if failsafe is active or emergencyStop is received
- transitions Boot/Init/Ready/SafeStop to Manual when safe

### 5.2 MissionController

Files:
- src/controllers/mission_controller.h
- src/controllers/mission_controller.cpp

Responsibilities:
- convert mode + radio inputs into high-level drive commands

Current behavior:
- SafeStop mode: throttle=0, steering=0, brake=true
- otherwise Manual mapping from radio throttle/steering

### 5.3 DriveController

Files:
- src/controllers/drive_controller.h
- src/controllers/drive_controller.cpp

Responsibilities:
- apply mode-based output limits
- forward commands to ESC and Servo drivers

Current behavior:
- throttle and steering are clamped by mode limits
- brake command is passed to ESC driver
- exposes applied output values for diagnostics

## 6. Driver Layer Behavior

### 6.1 Nrf24Driver

Files:
- src/drivers/nrf24_driver.h
- src/drivers/nrf24_driver.cpp

Current implementation is an adapter skeleton with loopback support:
- supports begin/write/read/available
- supports IRQ-like pending interrupt state
- can emulate RX from TX in loopback mode

This is suitable for architecture integration and testing flow, but not yet full hardware RF24 integration.

### 6.2 EscDriver

Files:
- src/drivers/esc_driver.h
- src/drivers/esc_driver.cpp

Behavior:
- non-blocking arming period at startup
- command interface supports throttle and brake
- signed throttle mapping to pulse range (reverse/neutral/forward)
- PWM output via ESP32 LEDC when ARDUINO_ARCH_ESP32 is defined

### 6.3 ServoDriver

Files:
- src/drivers/servo_driver.h
- src/drivers/servo_driver.cpp

Behavior:
- steering command mapped to servo pulse range
- PWM output via ESP32 LEDC when ARDUINO_ARCH_ESP32 is defined

## 7. Web/API Runtime Path

Files:
- src/web/api/api.h
- src/web/api/api.cpp
- src/web/websocket/websocket.h
- src/web/websocket/websocket.cpp

Runtime flow:
1. SystemManager updates VehicleState
2. ApiService receives VehicleState
3. WebSocketPublisher serializes JSON telemetry
4. if ESPAsyncWebServer is available, payload is broadcast to /ws clients

Additional endpoint:
- /health returns "ok"

Fallback:
- without ESPAsyncWebServer, publisher still stores payload locally but does not open real WS transport

## 8. Frontend Runtime Path

Files:
- src/web/html/index.html
- src/web/css/style.css
- src/web/js/app.js

Behavior:
- browser connects to ws://<host>/ws (or wss)
- auto-reconnect on disconnect
- parses telemetry JSON and updates dashboard values:
  - mode, failsafe, battery, lidar, imu
  - command and output values
  - ESC armed/brake
  - Wi-Fi AP status and clients
  - error code and event log counters

## 9. Diagnostics and Logging

Current diagnostics in runtime state:
- systemErrorCode
- logCount
- lastLog

SystemManager updates these when mode/failsafe/error state changes.

The current logging strategy is lightweight and state-based (not yet persistent).

## 10. Current Practical Status

Implemented and operational in code:
- end-to-end control loop skeleton
- mode and failsafe transitions
- drive command pipeline
- ESC/servo output pipeline
- telemetry JSON publication path
- WebSocket dashboard client
- Wi-Fi AP manager integration

Still at scaffold level for full production behavior:
- real RF24 hardware transport (beyond adapter loopback)
- full sensor acquisition drivers (MPU6050, TF-Luna, battery ADC)
- expanded autonomous logic beyond manual/failsafe base
