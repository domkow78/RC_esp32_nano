# Vehicle Communication Protocol (VCP)

## Purpose

VCP defines communication between the remote controller and the vehicle.
It is independent of the transport layer (nRF24L01, ESP-NOW, Wi-Fi,
Ethernet).

## Goals

-   Low latency
-   High reliability
-   Bidirectional communication
-   Extensible protocol
-   Backward compatibility

## Architecture

``` text
Remote Controller
      |
   VCP Layer
      |
 Transport
      |
============
      |
 Transport
      |
   VCP Layer
      |
Mission Controller
```

## Packet Types

  Type         Description
  ------------ ----------------------
  CONTROL      Vehicle control
  TELEMETRY    Vehicle status
  HEARTBEAT    Link supervision
  STATUS       System status
  CONFIG_GET   Read configuration
  CONFIG_SET   Update configuration
  DEBUG        Diagnostics
  BOOTLOADER   Firmware update

## Generic Packet

``` text
SYNC | TYPE | VERSION | SEQ | FLAGS | PAYLOAD | CRC
```

## Sequence Number

-   8-bit (0-255)
-   Detect lost/duplicate packets

## CRC

-   CRC8 for short frames
-   CRC16 for larger payloads

## CONTROL Payload

-   Throttle
-   Steering
-   Lights
-   Mode
-   AUX buttons
-   Emergency Stop

## TELEMETRY Payload

-   Battery
-   RSSI
-   Packet loss
-   IMU
-   LiDAR
-   ESC status
-   Servo status
-   Error flags

## HEARTBEAT

-   20 Hz
-   100 ms: warning
-   200 ms: hold command
-   300 ms: neutral/brake
-   500 ms: SAFE_STOP

## Configuration

Supported: - GET - SET - SAVE - LOAD_DEFAULT

## Firmware Update

Reserved messages: - BEGIN - DATA - VERIFY - END

## Diagnostics

-   CPU load
-   Heap
-   Stack
-   Reset reason
-   Task timing
-   Radio statistics

## Error Codes

  Code   Meaning
  ------ -------------
  0      OK
  1      Battery Low
  2      Radio Lost
  3      IMU Error
  4      LiDAR Error
  5      ESC Error
  6      Servo Error

## Priorities

  Priority   Packets
  ---------- -------------------
  Critical   CONTROL, E-STOP
  High       HEARTBEAT, STATUS
  Medium     TELEMETRY
  Low        CONFIG
  Lowest     DEBUG

## Recommended Rates

  Packet         Rate
  ----------- -------
  CONTROL       50 Hz
  HEARTBEAT     20 Hz
  TELEMETRY     10 Hz
  STATUS         2 Hz
  DEBUG          1 Hz

## Latency Targets

  Function        Target
  ----------- ----------
  Control        \<20 ms
  Link loss     \<200 ms
  SAFE_STOP     \<500 ms

## Design Principles

-   Transport independent
-   Deterministic timing
-   Safety first
-   Modular implementation
-   Backward compatible
-   Future-proof packet format
