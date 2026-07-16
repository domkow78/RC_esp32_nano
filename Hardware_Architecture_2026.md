# Hardware Architecture Specification — Caterpillar Car 2026

## Project Overview

The controller is based on **Arduino Nano ESP32** and emphasizes:

- EMC robustness
- Reliable power architecture
- Modular software design
- Future expandability
- Separation of logic, RF and power electronics

---

## Main Hardware

| Component | Interface |
|---|---|
| Arduino Nano ESP32 | — |
| nRF24L01 | SPI |
| MPU6050 | I²C |
| TF-Luna | UART1 |
| SSD1306 | I²C |
| WS2812B (3 channels) | GPIO |
| ESC | PWM |
| Steering Servo | PWM |

---

## Power Architecture

- **LiPo 3S** — main supply
- **Buck LOGIC** 6 V → Arduino Nano ESP32 (VIN)
- **Buck AUX** 5 V → Servo, TF-Luna, WS2812
- **Dedicated RF supply:**
  - Buck LOGIC → LDO 3.3 V → Ferrite → 100 µF + 100 nF → nRF24L01
- **Arduino 3.3 V rail:**
  - MPU6050
  - SSD1306

---

## Ground

- Star topology
- Separate branches for ESC, Buck LOGIC and Buck AUX
- Every external signal routed together with GND

---

## Signal Conditioning

### PWM

| Signal | Path |
|---|---|
| ESC | ESP32 → SN74LVC2G17 (3.3 V) → 47 Ω → ESC |
| Servo | ESP32 → SN74LVC2G17 (3.3 V) → 47 Ω → Servo |

### WS2812

Three independent channels:

- ESP32 → SN74AHCT1G125 (5 V) → 330 Ω → LED
- OE = GND
- 100 nF per buffer
- 1000 µF at LED supply input

---

## Battery Monitor

| Element | Value |
|---|---|
| Voltage divider | 33 kΩ / 10 kΩ |
| Series resistor | 100 Ω |
| Filter capacitor | 100 nF |
| ADC pin | A0 |

---

## Interfaces

| Bus | Device(s) |
|---|---|
| SPI | nRF24L01 |
| I²C | MPU6050, SSD1306 |
| UART0 | Programming / Debug |
| UART1 | TF-Luna |
| UART2 | Reserved |

---

## Pin Assignment

| Pin | Function |
|---|---|
| D2 | nRF24L01 IRQ |
| D3 | MPU6050 INT |
| D5 | ESC PWM |
| D6 | Servo PWM |
| D7 | UART1 RX ← TF-Luna TX |
| D8 | UART1 TX → TF-Luna RX |
| D9 | nRF24L01 CE |
| D10 | nRF24L01 CSN |
| D11 | SPI MOSI |
| D12 | SPI MISO |
| D13 | SPI SCK |
| SDA | MPU6050 + SSD1306 |
| SCL | MPU6050 + SSD1306 |
| A0 | Battery ADC |
| A1 | WS2812 Channel 1 |
| A2 | WS2812 Channel 2 |
| A3 | WS2812 Channel 3 |

---

## Module Connections

### nRF24L01

- D11 — MOSI
- D12 — MISO
- D13 — SCK
- D10 — CSN
- D9 — CE
- D2 — IRQ
- Dedicated 3.3 V LDO supply

### MPU6050

- SDA, SCL
- D3 — INT
- AD0 → GND
- XDA / XCL → NC

### SSD1306

- SDA, SCL
- 3.3 V supply

### TF-Luna

- D7 — RX
- D8 — TX
- 5 V AUX supply
- CINP → NC
- MOUT → NC

---

## EMC

| Measure | Detail |
|---|---|
| Star ground | Separate ESC / LOGIC / AUX branches |
| Ferrite (input) | Before Buck converters |
| Ferrite (RF) | After RF LDO |
| Buffered PWM | SN74LVC2G17 |
| SPI series resistors | 22–33 Ω near ESP32 |
| PWM series resistors | 47 Ω near buffer |
| GND plane | Continuous, uninterrupted |

---

## Software Modules

| Module | Responsibility |
|---|---|
| Power Manager | Supply sequencing and monitoring |
| Battery Monitor | ADC sampling, voltage calculation |
| Radio Driver | nRF24L01 SPI communication |
| LiDAR Driver | TF-Luna UART framing |
| IMU Driver | MPU6050 data acquisition |
| OLED Driver | SSD1306 rendering |
| RGB Driver | WS2812B pixel control |
| Drive Controller | ESC + Servo coordination |
| WiFi Manager | AP / STA management |
| Mission Controller | Main state machine |
| Failsafe | Watchdog and safe-stop logic |
| Web Interface | REST API + WebSocket |
