# Software Architecture — Caterpillar Car 2026

## Goal

Create a modular, scalable and maintainable firmware architecture that supports manual, semi-autonomous and autonomous operation while remaining robust and easy to extend.

---

## High-Level Architecture

```
main()
  │
  └── System Manager
        │
        ├── Hardware Layer
        ├── Service Layer
        └── Application Layer
                │
          Mission Controller
```

---

## Hardware Layer

Responsible **only** for hardware access. No application logic.

```
drivers/
  ├── esc_driver
  ├── servo_driver
  ├── nrf24_driver
  ├── tf_luna_driver
  ├── mpu6050_driver
  ├── ssd1306_driver
  ├── ws2812_driver
  ├── battery_driver
  └── wifi_driver
```

> Drivers expose an API only and contain no application logic.

---

## HAL — Hardware Abstraction Layer

Only this layer directly depends on the Arduino framework.

```
hal/
  ├── gpio
  ├── spi
  ├── uart
  ├── adc
  ├── pwm
  └── timer
```

---

## Service Layer

Responsible for processing and validating hardware data.

```
services/
  ├── radio_service
  ├── lidar_service
  ├── imu_service
  ├── battery_service
  ├── led_service
  ├── telemetry_service
  └── power_service
```

---

## Application Layer

Implements vehicle behaviour.

```
controllers/
  ├── drive_controller
  ├── manual_controller
  ├── autonomous_controller
  ├── obstacle_controller
  ├── mission_controller
  └── failsafe_controller
```

> **Mission Controller** is the main state machine of the vehicle.

---

## Managers

Global system management.

```
managers/
  ├── system_manager
  ├── task_manager
  ├── power_manager
  ├── wifi_manager
  ├── config_manager
  └── log_manager
```

---

## Models

Shared data structures.

```
models/
  ├── vehicle_state
  ├── battery_state
  ├── imu_data
  ├── lidar_data
  ├── radio_packet
  └── configuration
```

---

## Configuration

```
config/
  ├── pins.h
  ├── hw_config.h
  ├── constants.h
  ├── calibration.h
  └── version.h
```

> No hardcoded values shall appear outside this directory.

---

## Utilities

```
utils/
  ├── crc
  ├── moving_average
  ├── median_filter
  ├── ring_buffer
  └── timer
```

---

## Web Interface

```
web/
  ├── api
  ├── websocket
  ├── html
  ├── css
  └── js
```

---

## FreeRTOS Tasks

| Task | Priority |
|---|---|
| Radio Task | High |
| Drive Task | High |
| LiDAR Task | Medium |
| IMU Task | Medium |
| Display Task | Low |
| WiFi Task | Low |
| Battery Task | Low |
| Logger Task | Idle |

---

## Data Flow

```
nRF24L01
  │
  └── Radio Driver
        │
        └── Radio Service
              │
              └── Mission Controller
                    │
                    └── Drive Controller
                          │
                          ├── ESC Driver
                          └── Servo Driver
```

---

## Vehicle State

Single shared state object. All modules update **only their own section**.

```cpp
struct VehicleState {
    BatteryState  battery;
    RadioPacket   radio;
    ImuData       imu;
    LidarData     lidar;
    EscState      esc;
    ServoState    servo;
    WifiState     wifi;
    FailsafeState failsafe;
    OperatingMode mode;
};
```

---

## Operating Modes

```
BOOT
 ↓
INIT
 ↓
READY
 ↓
MANUAL  ←→  SEMI_AUTO  ←→  AUTO
 ↓
ERROR
 ↓
SAFE_STOP
```

---

## Recommended Folder Structure

```
src/
  ├── app/
  ├── controllers/
  ├── services/
  ├── drivers/
  ├── hal/
  ├── managers/
  ├── models/
  ├── config/
  ├── utils/
  ├── web/
  └── main.cpp
```

---

## Design Principles

- **Single Responsibility Principle** — each module has one clearly defined purpose.
- Drivers **never** implement business logic.
- Controllers **never** access hardware directly.
- All configuration lives in `config/`.
- FreeRTOS tasks communicate through queues or the shared `VehicleState`.
