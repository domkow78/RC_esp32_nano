# Caterpillar Car

# Communication Software Architecture

## Purpose

This document defines the software architecture of the Vehicle
Communication Protocol (VCP) implementation.

The communication stack is independent from the physical transport
layer.

------------------------------------------------------------------------

# Layered Architecture

``` text
Application
    │
Protocol Manager
    │
Packet Dispatcher
    │
Encoder / Decoder
    │
Transport Adapter
    │
Radio Driver
    │
HAL
```

------------------------------------------------------------------------

# Module Responsibilities

## HAL

Responsibilities: - SPI - GPIO - Timer - CRC support

No protocol logic.

------------------------------------------------------------------------

## Radio Driver

Responsibilities: - Initialize radio - Send raw frames - Receive raw
frames - Report RSSI and link quality

Public API:

``` cpp
bool begin();
bool send(const uint8_t* data, size_t len);
bool available();
bool receive(uint8_t* data, size_t* len);
```

------------------------------------------------------------------------

## Transport Adapter

Purpose: - Hide differences between nRF24L01, ESP-NOW and future
transports.

API:

``` cpp
class ITransport
{
    virtual bool send(...) = 0;
    virtual bool receive(...) = 0;
};
```

------------------------------------------------------------------------

## VcpEncoder

Responsibilities: - Build packet header - Serialize payload - Append
CRC - Produce binary frame

------------------------------------------------------------------------

## VcpDecoder

Responsibilities: - Validate header - Verify CRC - Deserialize payload -
Reject invalid packets

------------------------------------------------------------------------

## PacketValidator

Checks: - Protocol version - Payload length - CRC - Sequence number -
Packet type - Reserved bits

Returns a validation result for the dispatcher.

------------------------------------------------------------------------

## PacketDispatcher

Routes packets according to TYPE.

``` text
CONTROL     → ControlHandler
TELEMETRY   → TelemetryHandler
STATUS      → StatusHandler
CONFIG      → ConfigHandler
DEBUG       → DebugHandler
BOOT        → BootHandler
```

Dispatcher never contains business logic.

------------------------------------------------------------------------

## Protocol Manager

Central coordinator.

Responsibilities: - Maintain connection state - Generate heartbeat -
Track sequence numbers - Manage ACK/Retry - Detect link timeout -
Activate failsafe

------------------------------------------------------------------------

# Packet Handlers

## ControlHandler

Updates: - throttle - steering - lights - mode - auxiliary outputs

------------------------------------------------------------------------

## TelemetryHandler

Collects: - battery - IMU - LiDAR - radio statistics - ESC status

Produces telemetry packets.

------------------------------------------------------------------------

## ConfigHandler

Supports: - GET - SET - SAVE - LOAD_DEFAULT

------------------------------------------------------------------------

## BootHandler

Firmware update state machine.

States:

``` text
BEGIN
 ↓
DATA
 ↓
VERIFY
 ↓
END
```

------------------------------------------------------------------------

# Data Model

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

Each module updates only its own section.

------------------------------------------------------------------------

# FreeRTOS Tasks

  Task         Priority
  ---------- ----------
  Drive            High
  Radio RX         High
  Radio TX         High
  IMU            Medium
  LiDAR          Medium
  Battery        Medium
  Display           Low
  WiFi              Low
  Logger         Lowest

------------------------------------------------------------------------

# Inter-task Communication

Use: - Queues for packets - Event Groups for state changes - Mutexes for
shared resources - Timers for heartbeat

Avoid global variables except shared state objects.

------------------------------------------------------------------------

# Error Handling

Every module returns a status code.

Example:

``` cpp
enum class Result
{
    Ok,
    Timeout,
    InvalidPacket,
    CrcError,
    UnsupportedVersion,
    InternalError
};
```

------------------------------------------------------------------------

# Logging

Recommended levels:

-   DEBUG
-   INFO
-   WARNING
-   ERROR
-   FATAL

Logs should never block real-time tasks.

------------------------------------------------------------------------

# Design Rules

-   Single Responsibility Principle
-   Dependency inversion through interfaces
-   No hardware access above Driver layer
-   No application logic inside drivers
-   All packet parsing centralized
-   Deterministic execution
-   Unit-testable modules
-   Transport-independent protocol

------------------------------------------------------------------------

# Suggested Project Structure

``` text
src/
├── app/
├── controllers/
├── protocol/
│   ├── protocol_manager
│   ├── packet_dispatcher
│   ├── packet_validator
│   ├── encoder
│   ├── decoder
│   ├── handlers/
│   └── transport/
├── drivers/
├── services/
├── hal/
├── models/
├── config/
├── utils/
└── main.cpp
```

------------------------------------------------------------------------

# Future Extensions

Prepared for: - OTA updates - Multiple transports - Encryption -
Authentication - Message compression - Autonomous fleet communication
