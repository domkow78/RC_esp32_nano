# Firmware Development Guidelines

## 1. Purpose

This document defines the coding standards, architectural principles and
development workflow for the Caterpillar Car firmware.

Goals:

-   Maintainable code
-   Consistent architecture
-   Easy code reviews
-   Testability
-   Long-term scalability

------------------------------------------------------------------------

# 2. Programming Language

-   C++17
-   PlatformIO
-   Arduino Nano ESP32
-   FreeRTOS

------------------------------------------------------------------------

# 3. General Principles

-   Single Responsibility Principle (SRP)
-   Separation of hardware and business logic
-   Dependency Inversion where practical
-   Composition over inheritance
-   Deterministic execution for real-time tasks

------------------------------------------------------------------------

# 4. Layered Architecture

``` text
Application
    │
Controllers
    │
Services
    │
Protocol
    │
Drivers
    │
HAL
```

Each layer communicates only with the layer directly below it.

------------------------------------------------------------------------

# 5. Project Structure

``` text
src/
├── app/
├── controllers/
├── services/
├── protocol/
├── drivers/
├── hal/
├── models/
├── config/
├── utils/
├── web/
└── main.cpp
```

------------------------------------------------------------------------

# 6. Naming Convention

Classes: - PascalCase - Example: RadioDriver

Methods: - camelCase - Example: sendPacket()

Variables: - camelCase - Example: batteryVoltage

Constants: - UPPER_CASE - Example: MAX_SPEED

Files: - snake_case - Example: radio_driver.cpp

------------------------------------------------------------------------

# 7. Source Files

Each module should contain:

-   \*.h
-   \*.cpp

Public interface in the header. Implementation in the source file.

------------------------------------------------------------------------

# 8. Documentation

Every public class and method should contain Doxygen comments.

Example:

``` cpp
/**
 * @brief Sends a protocol packet.
 * @param packet Packet to transmit.
 * @return Result code.
 */
```

------------------------------------------------------------------------

# 9. Error Handling

Use strongly typed result codes.

Example:

``` cpp
enum class Result
{
    Ok,
    Timeout,
    InvalidPacket,
    HardwareError,
    InternalError
};
```

Avoid hidden failures.

------------------------------------------------------------------------

# 10. Logging

Levels:

-   DEBUG
-   INFO
-   WARNING
-   ERROR
-   FATAL

Logging must never block real-time execution.

------------------------------------------------------------------------

# 11. FreeRTOS Rules

Recommended tasks:

-   Radio RX
-   Radio TX
-   Drive
-   IMU
-   LiDAR
-   Battery
-   Display
-   WiFi
-   Logger

Use: - Queues - Event Groups - Mutexes - Software Timers

Avoid busy waiting.

------------------------------------------------------------------------

# 12. Configuration

Store all configurable values in:

``` text
config/
```

Avoid magic numbers in implementation files.

------------------------------------------------------------------------

# 13. State Machines

Subsystems should use explicit state machines.

Example:

``` text
BOOT
INIT
READY
RUN
ERROR
SAFE_STOP
```

------------------------------------------------------------------------

# 14. Unit Testing

Business logic should be independent of hardware where possible.

Test: - packet parsing - protocol validation - filters - state
machines - calculations

------------------------------------------------------------------------

# 15. Code Review Checklist

-   Builds without warnings
-   Naming follows convention
-   No duplicated logic
-   No dynamic allocation in critical paths
-   Public API documented
-   Error handling implemented
-   Logging appropriate
-   Tests updated

------------------------------------------------------------------------

# 16. Git Workflow

Recommended branches:

-   main
-   develop
-   feature/\*
-   bugfix/\*
-   release/\*

Each feature should be developed in its own branch.

------------------------------------------------------------------------

# 17. Commit Messages

Examples:

-   Add LiDAR driver
-   Fix battery voltage scaling
-   Refactor protocol manager
-   Improve failsafe logic

Use imperative mood and concise descriptions.

------------------------------------------------------------------------

# 18. Performance Guidelines

-   Avoid unnecessary heap allocation.
-   Minimize copying of packet buffers.
-   Keep interrupt routines short.
-   Prioritize deterministic timing over micro-optimizations.

------------------------------------------------------------------------

# 19. Security and Reliability

-   Validate all external inputs.
-   Verify CRC before processing packets.
-   Handle invalid states safely.
-   Default to SAFE_STOP on critical communication failures.

------------------------------------------------------------------------

# 20. Long-Term Goals

The firmware architecture shall support:

-   OTA updates
-   Additional transports
-   Autonomous navigation
-   AI-assisted perception
-   Advanced telemetry
-   Future hardware revisions

The architecture should remain modular, testable and
transport-independent.
