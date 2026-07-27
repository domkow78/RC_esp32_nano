# Vehicle Communication Protocol (VCP) Specification v1.0

## 1. Scope

This document defines the binary communication protocol used between the
Remote Controller and the Vehicle. The protocol is transport
independent.

Supported transports: - nRF24L01 - ESP-NOW - Wi-Fi - Ethernet (test)

------------------------------------------------------------------------

# 2. Packet Layout

        Byte Field           Size Description
  ---------- --------- ---------- --------------------------------
           0 SYNC               1 Start marker (0x55)
           1 TYPE               1 Packet type
           2 VERSION            1 Protocol version
           3 FLAGS              1 Status flags
           4 SEQ                1 Sequence number
           5 LENGTH             1 Payload length
        6..N PAYLOAD     Variable Packet data
    N+1..N+2 CRC16              2 CRC-16 over header and payload

Maximum payload: 24 bytes (fits comfortably within nRF24L01 32-byte
payload).

------------------------------------------------------------------------

# 3. Packet Types

      ID Name
  ------ --------------
    0x01 CONTROL
    0x02 TELEMETRY
    0x03 HEARTBEAT
    0x04 STATUS
    0x05 CONFIG_GET
    0x06 CONFIG_SET
    0x07 CONFIG_REPLY
    0x08 DEBUG
    0x09 BOOT_BEGIN
    0x0A BOOT_DATA
    0x0B BOOT_VERIFY
    0x0C BOOT_END

------------------------------------------------------------------------

# 4. Flags

    Bit Meaning
  ----- -----------------------
      0 ACK required
      1 ACK packet
      2 Emergency Stop
      3 Configuration changed
      4 Failsafe active
      5 Reserved
      6 Reserved
      7 Reserved

------------------------------------------------------------------------

# 5. CONTROL Payload

    Byte Type    Description
  ------ ------- ------------------------
       0 int16   Throttle (-1000..1000)
       2 int16   Steering (-1000..1000)
       4 uint8   Drive mode
       5 uint8   Buttons bitmap
       6 uint8   Light mode
       7 uint8   Reserved

Recommended rate: **50 Hz**

------------------------------------------------------------------------

# 6. TELEMETRY Payload

  Field             Type
  ----------------- -------------
  Battery voltage   uint16 (mV)
  Battery current   int16 (mA)
  Battery level     uint8 (%)
  RSSI              int8
  Packet loss       uint8 (%)
  LiDAR distance    uint16 (mm)
  Vehicle mode      uint8
  Error flags       uint16

Recommended rate: **10 Hz**

------------------------------------------------------------------------

# 7. HEARTBEAT

Payload:

    Byte Meaning
  ------ ------------
       0 Node state
       1 Reserved

Rate: **20 Hz**

Timeout policy: - 100 ms: communication warning - 200 ms: hold last
command - 300 ms: neutral / brake - 500 ms: SAFE_STOP

------------------------------------------------------------------------

# 8. STATUS Packet

Contains:

-   firmware version
-   hardware revision
-   protocol version
-   uptime
-   reset reason

Rate: 2 Hz.

------------------------------------------------------------------------

# 9. CONFIG Packets

Supported operations: - GET - SET - SAVE - LOAD_DEFAULT

Each parameter uses:

  Field
  --------------
  Parameter ID
  Type
  Length
  Value

------------------------------------------------------------------------

# 10. Error Codes

    Code Description
  ------ ---------------------
       0 OK
       1 Battery Low
       2 Radio Timeout
       3 IMU Failure
       4 LiDAR Failure
       5 ESC Failure
       6 Servo Failure
       7 Configuration Error
       8 Internal Error

------------------------------------------------------------------------

# 11. State Machine

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

# 12. Reliability Rules

-   Sequence number checked for every packet.
-   Invalid CRC packets are discarded.
-   Duplicate sequence numbers are ignored.
-   CONTROL packets have highest priority.
-   HEARTBEAT is mandatory.
-   Configuration packets require ACK.

------------------------------------------------------------------------

# 13. Timing Requirements

  Function               Requirement
  -------------------- -------------
  Control latency            \<20 ms
  Heartbeat period             50 ms
  Failsafe detection        \<200 ms
  SAFE_STOP                 \<500 ms

------------------------------------------------------------------------

# 14. Versioning

Major version changes may break compatibility. Minor version changes
shall remain backward compatible.

------------------------------------------------------------------------

# 15. Future Extensions

Reserved for: - GPS/RTK - Camera metadata - AI events - OTA update -
Telemetry streaming - Autonomous navigation
