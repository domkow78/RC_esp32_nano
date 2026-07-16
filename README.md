# 🚗 RC_esp32_nano

A remotely controlled RC car project developed across multiple hardware and software versions.

## 📖 Overview

This repository contains two main development lines:

- **Version 1 (2024)** — control via **WiFi TCP/IP** (ATmega UNO + ESP8266)
- **Version 2 (2025)** — control via **nRF24L01** with optional **MPU6050 IMU** (ATmega Nano)
- **Version 3 (2026)** — control via **nRF24L01**, full sensor suite, FreeRTOS (**Arduino Nano ESP32**)

The shared goal of all versions is RC vehicle control (drive + steering) with an emphasis on low latency and operational safety.

---

## 🧩 Project Structure

- Receivers: [src/Receiver](src/Receiver)
  - [01_rx_atmega_uno](src/Receiver/01_rx_atmega_uno/AP_TCP_Server_RC_Car.ino) — WiFi TCP server (V1)
  - [02_rx_atmega_nano](src/Receiver/02_rx_atmega_nano/receiver_nano.ino) — nRF24 receiver (V2)
  - [03_rx_esp32_nano](src/Receiver/03_rx_esp32_nano) — ESP32 receiver (V3, in progress)
- Transmitters: [src/Transmitter](src/Transmitter)
  - [02_tx_nano](src/Transmitter/02_tx_nano/transmitter_nano.ino) — nRF24 transmitter (V2)
  - [03_tx_nano](src/Transmitter/03_tx_nano) — transmitter (V3, in progress)
- Documentation and assets: [doc](doc)

---

## 🌐 Version 1 (2024) — WiFi TCP

### Architecture

A client (phone or PC) sends discrete commands (`F/B/L/R/S`) over TCP to an ESP8266 acting as an Access Point.
The ESP8266 forwards them to an ATmega UNO, which drives 4 DC motors via direction pins.

### Features

- Simple implementation, easy network testing
- Discrete commands (no smooth throttle control)
- Good educational base for WiFi + Arduino communication

---

## ⚡ Version 2 (2025) — nRF24 + IMU

### Architecture

The transmitter (Nano) reads joysticks, buttons and potentiometers (and optionally the MPU6050), packs the data and sends it via nRF24.
The receiver (Nano) decodes the packet and controls:

- **ESC** (drive)
- **Servo** (steering)
- **WS2812 LEDs** (status and turn indication)

### Features

- Low latency, true radio-style control
- Two drive modes (crawl / speed)
- Deadzone, ramping, watchdog and signal timeout
- Optional IMU-based controller tilt mode

---

## 🛰️ Version 3 (2026) — Arduino Nano ESP32

### Architecture

Full redesign based on **Arduino Nano ESP32** with FreeRTOS task architecture.

Key additions over V2:

| Feature | Detail |
|---|---|
| RF | nRF24L01 with IRQ |
| LiDAR | TF-Luna (UART1) |
| IMU | MPU6050 (I²C) |
| Display | SSD1306 OLED (I²C) |
| LEDs | 3 × WS2812B channels |
| Connectivity | WiFi AP + WebSocket telemetry |
| Power | 3S LiPo, dual Buck, RF LDO |
| EMC | Buffered PWM, ferrites, star ground |

### Architecture Documents

- [Hardware Architecture](Hardware_Architecture_2026.md)
- [Hardware Design Rules](Hardware_Design_Rules_2026.md)
- [Software Architecture](Software_Architecture_2026.md)
- [Objectives for V3](Objectives_for_V3.md)

---

## 📡 Data & Communication

In V2 a compact `Data_Package` struct (14 bytes) is used, carrying joystick axes, button and switch states.
This fits safely within the nRF24 buffer limit (32 bytes).

In V3 the packet structure is extended to support telemetry, mode flags and failsafe state.

---

## 🛡️ Safety & Reliability

Mechanisms implemented across versions:

- Signal loss timeout
- Hardware watchdog (V2 receiver, V3)
- Neutral / stop as the default safe state
- Control limits and deadzone filtering
- Failsafe controller with `SAFE_STOP` mode (V3)

---

## 📚 Analysis Documents

| Document | Content |
|---|---|
| [G_model_v1_2024.md](G_model_v1_2024.md) | Receiver V1 analysis |
| [G_model_v2_2025.md](G_model_v2_2025.md) | Receiver V2 analysis |
| [G_model_v3_2026.md](G_model_v3_2026.md) | Receiver V3 analysis |
| [P_model_v2_2025.md](P_model_v2_2025.md) | Transmitter V2 analysis |

---

## 🔧 Libraries

| Library | Purpose |
|---|---|
| `RF24` | nRF24L01 radio |
| `FastLED` | WS2812B LEDs |
| `Servo` | Servo PWM |
| `Wire` | I²C (MPU6050, SSD1306) |
| `avr/wdt` | Hardware watchdog |
| `FreeRTOS` | Task scheduling (V3) |
| `ESPAsyncWebServer` | Web interface (V3) |

---

## 🚀 Roadmap

- [x] V1 — WiFi TCP control (2024)
- [x] V2 — nRF24 + IMU control (2025)
- [ ] V3 — ESP32, FreeRTOS, full sensor suite (2026)
- [ ] V3 — Autonomous obstacle avoidance
- [ ] V3 — WebSocket live telemetry dashboard
- [ ] V3 — OTA firmware update

---

*Last README update: July 2026.*
