# Software Design Description (SDD)

**Version:** 1.0

## 1. Purpose

This document describes the software architecture and internal design of
the Car firmware. It explains how the requirements defined
in the SRS are implemented.

------------------------------------------------------------------------

# 2. Architecture Overview

``` text
                main()
                  │
          System Manager
                  │
 ┌────────────────┼────────────────┐
 │                │                │
Controllers    Services       Infrastructure
 │                │                │
 └────────────────┼────────────────┘
                  │
             Protocol Layer
                  │
               Drivers
                  │
                 HAL
```

------------------------------------------------------------------------

# 3. Layer Responsibilities

## HAL

Provides hardware abstraction for GPIO, SPI, I²C, UART, PWM, ADC and
timers.

## Drivers

Implement hardware-specific communication with peripherals.

Modules: - RadioDriver - EscDriver - ServoDriver - Mpu6050Driver -
TfLunaDriver - OledDriver - Ws2812Driver - BatteryDriver

## Protocol

Implements the Vehicle Communication Protocol (VCP): - encoder -
decoder - validator - dispatcher - protocol manager

## Services

Convert raw hardware data into application-level information.

Examples: - BatteryService - ImuService - LidarService - RadioService -
TelemetryService

## Controllers

Implement system behaviour: - MissionController - DriveController -
ManualController - AutonomousController - FailsafeController

------------------------------------------------------------------------

# 4. Main Data Model

``` cpp
struct VehicleState
{
    BatteryState battery;
    RadioState radio;
    ImuState imu;
    LidarState lidar;
    DriveState drive;
    SystemState system;
};
```

Each subsystem owns and updates only its section.

------------------------------------------------------------------------

# 5. Task Architecture (FreeRTOS)

  Task         Priority Purpose
  ---------- ---------- --------------------------
  Radio RX         High Receive packets
  Radio TX         High Transmit packets
  Drive            High ESC and steering control
  IMU            Medium Sensor acquisition
  LiDAR          Medium Distance measurement
  Battery        Medium Voltage monitoring
  Display           Low OLED updates
  WiFi              Low Web interface
  Logger         Lowest Diagnostics

Communication mechanisms: - Queue - Event Group - Mutex - Software Timer

------------------------------------------------------------------------

# 6. Data Flow

``` text
Radio
  │
RadioDriver
  │
ProtocolManager
  │
PacketDispatcher
  │
MissionController
  │
DriveController
  │
EscDriver / ServoDriver
```

Telemetry follows the reverse path.

------------------------------------------------------------------------

# 7. State Machines

## System

``` text
BOOT
 ↓
INIT
 ↓
READY
 ↓
MANUAL
 ↓
SEMI_AUTO
 ↓
AUTO
 ↓
SAFE_STOP
```

## Communication

``` text
Disconnected
 ↓
Connecting
 ↓
Connected
 ↓
Heartbeat Lost
 ↓
Failsafe
```

------------------------------------------------------------------------

# 8. Error Handling

Every module returns a strongly typed result.

``` cpp
enum class Result
{
    Ok,
    Timeout,
    InvalidPacket,
    CrcError,
    HardwareError,
    InternalError
};
```

Critical errors activate SAFE_STOP.

------------------------------------------------------------------------

# 9. Configuration

All configurable parameters are centralized.

``` text
config/
    pins.h
    constants.h
    hw_config.h
    calibration.h
    version.h
```

No magic numbers in application code.

------------------------------------------------------------------------

# 10. Logging

Levels: - DEBUG - INFO - WARNING - ERROR - FATAL

Logs shall never block real-time tasks.

------------------------------------------------------------------------

# 11. Design Patterns

Recommended patterns: - State Machine - Strategy - Factory - Observer
(events) - Dependency Injection (interfaces) - Command (configuration
commands)

------------------------------------------------------------------------

# 12. Public APIs

Each module exposes: - initialize() - update() - getState() - reset()

Drivers additionally expose hardware-specific APIs.

------------------------------------------------------------------------

# 13. Testing Strategy

Unit Tests: - protocol - packet parser - filters - state machines

Integration Tests: - radio - sensors - drive system

System Tests: - complete vehicle - long-duration operation - failsafe
validation

------------------------------------------------------------------------

# 14. Directory Layout

``` text
src/
├── app/
├── controllers/
├── services/
├── protocol/
├── drivers/
├── hal/
├── managers/
├── models/
├── config/
├── utils/
├── web/
└── main.cpp
```

------------------------------------------------------------------------

# 15. Traceability

Each module should reference: - SRS requirement IDs (FR-xxx) - Test
cases - Related design documents

------------------------------------------------------------------------

# 16. Future Evolution

The design supports: - OTA updates - Multiple transport layers -
Autonomous navigation - SLAM - AI-assisted perception - Additional
sensors - Future hardware revisions

The architecture is intended to remain modular, testable and
transport-independent.
