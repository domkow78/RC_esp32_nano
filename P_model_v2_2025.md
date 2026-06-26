# Analiza pliku transmitter_nano.ino - Transmitter 02 (ATmega Nano + nRF24 + MPU6050)

## 📋 Przegląd ogólny
- **Mikrokontroler**: Arduino ATmega Nano
- **Komunikacja RF**: nRF24L01 (250 kbps, PA_LOW)
- **Czujnik**: MPU6050 (akcelerometr + żyroskop, I2C)
- **Funkcja**: Nadajnik z obsługą joystickowych i czujników IMU
- **Adres RF24**: `"00001"`

---

## 🔌 Konfiguracja pinów

### RF24L01:
- **CE**: Pin 5
- **CSN**: Pin 6

### MPU6050 (I2C):
- **SDA**: Pin A4 (I2C)
- **SCL**: Pin A5 (I2C)
- **Adres I2C**: 0x68

### Joystick & Przyciski:
| Funkcja | Pin | Typ |
|---------|-----|-----|
| Joystick 1 Button | 1 | Digital (INPUT_PULLUP) |
| Joystick 2 Button | 0 | Digital (INPUT_PULLUP) |
| Toggle Switch 1 | 7 | Digital (INPUT_PULLUP) |
| Toggle Switch 2 | 4 | Digital (INPUT_PULLUP) |
| Button 1 | 8 | Digital (INPUT_PULLUP) |
| Button 2 | 9 | Digital (INPUT_PULLUP) |
| Button 3 | 2 | Digital (INPUT_PULLUP) |
| Button 4 | 3 | Digital (INPUT_PULLUP) |

### Wejścia Analogowe:
| Funkcja | Pin |
|---------|-----|
| Joystick 1 X | A1 |
| Joystick 1 Y | A0 |
| Joystick 2 X | A2 |
| Joystick 2 Y | A3 |
| Potencjometr 1 | A7 |
| Potencjometr 2 | A6 |

---

## 📡 Komunikacja RF24

### Parametry radia:
```
Adres: "00001"
Szybkość: 250 kbps (niskoenergowe)
Moc: PA_LOW (niska)
AutoAck: Wyłączony
Buffer: Max 32 bajty (limit NRF24)
```

---

## 🎮 Struktura danych transmitowanych

```cpp
struct Data_Package {
  byte j1PotX;      // Joystick 1 X (0-255)
  byte j1PotY;      // Joystick 1 Y (0-255)
  byte j1Button;    // Joystick 1 Button (0/1)
  byte j2PotX;      // Joystick 2 X (0-255) / MPU Angle X
  byte j2PotY;      // Joystick 2 Y (0-255) / MPU Angle Y
  byte j2Button;    // Joystick 2 Button (0/1)
  byte pot1;        // Potencjometr 1 (0-255)
  byte pot2;        // Potencjometr 2 (0-255)
  byte tSwitch1;    // Toggle Switch 1 (0/1) - IMU Mode
  byte tSwitch2;    // Toggle Switch 2 (0/1)
  byte button1;     // Button 1 (0/1)
  byte button2;     // Button 2 (0/1)
  byte button3;     // Button 3 (0/1)
  byte button4;     // Button 4 (0/1)
};
```

**Rozmiar**: 14 bajtów (poniżej limitu 32 bajtów nRF24)

---

## 🧭 MPU6050 - Czujnik Ruchu 6-DOF

### Inicjalizacja:
- **Akcelerometr**: ±8g full scale
- **Żyroskop**: ±1000 dps full scale
- **I2C Frequency**: 100 kHz (domyślna)

### Kalibracja (opcjonalna):
Funkcja `calculate_IMU_error()` kalibruje czujnik poprzez:
- 200 odczytów akcelerometru → błędy AccErrorX, AccErrorY
- 200 odczytów żyroskopu → błędy GyroErrorX, GyroErrorY
- Czujnik musi być nieruchomy!

### Błędy kalibracji (stałe w kodzie):
```
AccErrorX: +1.15°
AccErrorY: -0.52°
GyroErrorX: -1.85°/s
GyroErrorY: +0.15°/s
```

---

## 📊 Przetwarzanie danych IMU

### 1. Odczyt akcelerometru:
```cpp
AccX, AccY, AccZ → podzielić przez 4096 (±8g range)
```

### 2. Obliczenie kąta z akcelerometru:
```cpp
accAngleX = atan(AccY / √(AccX² + AccZ²)) + 1.15°
accAngleY = atan(-AccX / √(AccY² + AccZ²)) - 0.52°
```

### 3. Odczyt żyroskopu:
```cpp
GyroX, GyroY → podzielić przez 32.8 (±1000 dps range)
GyroX += 1.85°/s (offset)
GyroY -= 0.15°/s (offset)
```

### 4. Całkowanie żyroskopu w czasie:
```cpp
gyroAngleX = GyroX × elapsedTime
gyroAngleY = GyroY × elapsedTime
```

### 5. Filtr komplementarny (Complementary Filter):
```cpp
angleX = 0.98 × (angleX + gyroAngleX) + 0.02 × accAngleX
angleY = 0.98 × (angleY + gyroAngleY) + 0.02 × accAngleY
```

**98% - żyroskop (szybki, dryfuje)**
**2% - akcelerometr (wolny, stabilny)**

### 6. Mapowanie na zakres Joystick (0-255):
```cpp
data.j1PotX = map(angleX, -90°, +90°, 255, 0)
data.j1PotY = map(angleY, -90°, +90°, 0, 255)
```

---

## 🎛️ Tryby sterowania

### Normalny (Toggle Switch 1 = 1):
- Joystick 1 X/Y: Zwykłe wejścia analogowe
- Joystick 2 X/Y: Zwykłe wejścia analogowe

### IMU Mode (Toggle Switch 1 = 0):
- Joystick 1 X/Y: Zamieniane na kąty z MPU6050
- **Tilt kontrola**: Przechylenie kontrolera = sterowanie
- Emuluje ruchy fizyczne czujnika jako joystick

---

## 🔄 Główna pętla

```
1. Odczyt joysticków (A0-A3) → mapowanie na 0-255
2. Odczyt potencjometrów (A6-A7) → mapowanie na 0-255
3. Odczyt przycisków cyfrowych (piny 0-9)
4. JEŚLI Toggle Switch 1 = ON:
      → read_IMU() - zamienia j1Pot na kąty z MPU6050
5. Wysłanie całej struktury przez RF24
6. Powtarzanie (bez delay = ~10-20 ms na loop)
```

---

## 📈 Charakterystyka czasowa

- **Częstotliwość odczytu**: ~50-100 Hz (bez delay w loop)
- **Elapsed Time (IMU)**: Mierzony między odczytami (`millis()`)
- **Serial Debug**: 9600 baud (opcjonalny)

---

## ⚠️ Potencjalne problemy/uwagi

1. ✓ **Filtr komplementarny** - dobrze zbilansowany 98/2 ratio
2. ✓ **Kalibracja offsetu** - uwzględniona w kodzie
3. ⚠️ **Zmienna `tSwitch1`** - nigdy nie jest wysyłana (zawsze 1 w strukturze)
4. ⚠️ **Dragi żyroskopu** - filtr komplementarny kompensuje, ale może być potrzebny low-pass filter
5. ⚠️ **Brak timeout'u** - MPU6050 zawsze czytany, nawet w normalnym trybie
6. ⚠️ **Magnetometr niedostępny** - MPU6050 ma tylko 6-DOF (brak Z rotation)
7. ⚠️ **Średni czas przetwarzania**: ~5-10ms na loop (может być wąskim gardłem)
8. ✓ Struktura danych zoptymalizowana pod limit 32-bajtowy nRF24

---

## 💡 Spostrzeżenia

### Wersja 2 vs Wersja 1:
- **V2 Nadajnik**: Dwa Joystick'i + IMU + potencjometry
- **V1 Nadajnik**: Najprawdopodobniej mniej czujników (brak IMU)
- IMU pozwala na przechylenie kontrolera jako sterowanie (motion control)

### Możliwe ulepszenia:
1. Dodać timeout w razie rozpędzenia żyroskopu
2. Implementować 9-DOF (dodać magnetometr HMC5883L)
3. Dodać battery monitoring (ADC na Nano może odczytać napięcie)
4. Implementować handshake/ACK z odbiornikiem
