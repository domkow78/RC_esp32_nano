# Hardware Design Rules — PCB & EMC 2026

---

## 1. PCB Stackup

Recommended:

- 2-layer PCB (prototype) or 4-layer (future)
- Continuous GND plane wherever possible
- Minimize splits in the ground plane

---

## 2. Component Placement

### High-current section

Place close together:

- LiPo connector
- ESC connector
- Buck LOGIC
- Buck AUX

> Keep this area separated from RF and sensors.

### RF section

Place together:

- nRF24L01
- RF LDO
- Ferrite
- RF capacitors

Rules:

- Away from ESC
- Away from Buck converters
- Away from motor cables
- Keep antenna outside copper pour

### Logic section

Cluster together:

- Arduino Nano ESP32
- MPU6050
- SSD1306

> Keep I²C traces short.

---

## 3. Power Distribution

Use star topology:

```
LiPo
 ├── ESC
 ├── Buck LOGIC
 └── Buck AUX
```

- Do not daisy-chain power between loads.

---

## 4. Grounding

- Single star ground reference
- Separate return paths for ESC, LOGIC and AUX
- Route every external signal with a nearby GND return
- Avoid ground loops

---

## 5. Decoupling

| Location | Capacitors |
|---|---|
| Buck LOGIC input | 100 µF, Ferrite, 22 µF |
| Buck LOGIC output | 22 µF, 100 nF |
| RF LDO output | Ferrite, 100 µF, 100 nF |
| MPU6050 | 100 nF |
| SSD1306 | 100 nF |
| TF-Luna | 10 µF, 100 nF |
| Servo connector | 470–1000 µF, 100 nF |
| WS2812 strip input | 1000 µF |

---

## 6. Routing Rules

### SPI

- Keep traces short
- Route MOSI / MISO / SCK together
- 22–33 Ω series resistors near ESP32

### I²C

- Keep SDA / SCL together
- Avoid routing near Buck inductors

### UART

- Keep TX / RX together
- Optional 22 Ω series resistors

### PWM

- Buffer with SN74LVC2G17
- 47 Ω series resistor close to buffer output

### WS2812

- Buffer with SN74AHCT1G125
- 330 Ω resistor close to buffer output

---

## 7. EMC

- Keep Buck inductors away from RF section
- Keep RF away from motor wiring
- Use ferrites only where defined in the schematic
- Separate noisy and sensitive areas physically
- Maintain uninterrupted GND plane

---

## 8. Test Points

Provide accessible test points for:

| Signal | Type |
|---|---|
| LiPo | Voltage |
| 6 V (Buck LOGIC) | Voltage |
| 5 V AUX | Voltage |
| 3.3 V (Arduino) | Voltage |
| 3.3 V RF (LDO) | Voltage |
| GND | Reference |
| SPI (MOSI/MISO/SCK) | Digital |
| UART1 (TX/RX) | Digital |
| ADC (A0) | Analog |
| PWM ESC | Digital |
| PWM Servo | Digital |

---

## 9. DFM — Design for Manufacturability

- Use identical footprints where possible
- Place decoupling capacitors on same side as the IC
- Place series resistors close to drivers
- Leave optional 0 Ω footprints where tuning may be required

---

## 10. Design Philosophy

Priority order:

1. Reliable power
2. EMC robustness
3. Serviceability
4. Modular firmware support
5. Future expansion

> The PCB shall prioritize robustness over minimum size.
