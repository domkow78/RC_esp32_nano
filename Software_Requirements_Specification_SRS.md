# Software Requirements Specification (SRS)

**Version:** 1.0

## 1. Purpose

This document defines the functional and non-functional requirements for
the RC Car platform. It serves as the primary reference for
firmware, hardware and verification activities.

------------------------------------------------------------------------

# 2. System Overview

The Car is a tracked robotic platform capable of:

-   Manual remote control
-   Semi-autonomous operation
-   Autonomous operation (future)
-   Real-time telemetry
-   Wireless configuration
-   Sensor fusion
-   Future AI-assisted navigation

------------------------------------------------------------------------

# 3. Functional Requirements

## FR-001 Manual Driving

The operator shall control throttle and steering using a remote
controller.

## FR-002 Emergency Stop

The vehicle shall immediately enter SAFE_STOP after an emergency stop
command.

## FR-003 Failsafe

Loss of communication shall automatically trigger the configured
failsafe sequence.

## FR-004 Telemetry

The vehicle shall periodically transmit: - battery status - IMU data -
LiDAR distance - radio statistics - operating mode - error flags

## FR-005 Configuration

Configuration parameters shall be readable and writable without
recompiling firmware.

## FR-006 Diagnostics

The firmware shall expose diagnostic information including reset reason,
task status and communication statistics.

## FR-007 Logging

The system shall provide configurable log levels for development and
troubleshooting.

## FR-008 Web Interface

The vehicle shall provide a browser-based interface for monitoring and
configuration over Wi-Fi.

## FR-009 OTA Ready

The architecture shall support future firmware updates over the air.

------------------------------------------------------------------------

# 4. Operating Modes

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

------------------------------------------------------------------------

# 5. Non-Functional Requirements

## Performance

-   Control latency \< 20 ms
-   Heartbeat period 50 ms
-   SAFE_STOP activation \< 500 ms

## Reliability

-   CRC validation on every packet
-   Sequence number validation
-   Automatic recovery after transient communication errors where
    possible

## Maintainability

-   Modular architecture
-   Documented public APIs
-   Unit-testable business logic

## Portability

The protocol shall remain independent of the transport layer.

------------------------------------------------------------------------

# 6. Safety Requirements

-   Invalid packets shall be ignored.
-   Critical failures shall result in SAFE_STOP.
-   Watchdog shall supervise firmware execution.
-   ESC output shall default to neutral after failsafe timeout.

------------------------------------------------------------------------

# 7. Hardware Requirements

Supported modules:

-   Arduino Nano ESP32
-   nRF24L01
-   MPU6050
-   TF-Luna
-   SSD1306
-   WS2812
-   ESC
-   Steering Servo

------------------------------------------------------------------------

# 8. Communication Requirements

-   Vehicle Communication Protocol (VCP)
-   Bidirectional communication
-   Heartbeat supervision
-   ACK for configuration commands
-   CRC16 packet protection

------------------------------------------------------------------------

# 9. Software Architecture Requirements

The firmware shall follow a layered architecture:

``` text
Application
Controllers
Services
Protocol
Drivers
HAL
```

Business logic shall never directly access hardware.

------------------------------------------------------------------------

# 10. User Interface Requirements

Remote controller: - Driving - Mode selection - Emergency stop

Web interface: - Live telemetry - Configuration - Diagnostics - Firmware
information

OLED: - Battery - Mode - Radio status - Errors

------------------------------------------------------------------------

# 11. Verification

The project shall include:

-   Unit tests
-   Integration tests
-   Hardware bring-up tests
-   Communication tests
-   Long-duration stability tests
-   Failsafe verification

------------------------------------------------------------------------

# 12. Acceptance Criteria

The system shall be accepted when:

-   Manual driving is stable.
-   Telemetry operates without packet corruption.
-   Failsafe activates within specification.
-   All sensors operate correctly.
-   Communication remains stable under expected operating conditions.
-   Firmware passes regression testing.

------------------------------------------------------------------------

# 13. Future Roadmap

Planned capabilities:

-   GPS / RTK
-   SLAM
-   Autonomous navigation
-   AI vision
-   Camera streaming
-   Fleet communication
-   Cloud telemetry
-   Mission scripting

------------------------------------------------------------------------

# 14. Traceability

Each implementation task, test case and design document should reference
the corresponding requirement identifier (FR-xxx) to maintain end-to-end
traceability.
