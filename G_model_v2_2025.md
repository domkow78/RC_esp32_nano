# Analiza pliku receiver_nano.ino - Receiver 02 (ATmega Nano)

## 📋 Przegląd ogólny
- **Mikrokontroler**: ATmega Nano
- **Funkcja**: Odbiornik sygnałów RF24L01 oraz sterowanie silnikiem i serwa
- **Główne moduły**: Radio nRF24, Servo, FastLED (WS2812)

---

## 🔌 Konfiguracja pinów
| Komponent | Pin |
|-----------|-----|
| LED przód | 7 |
| LED lewo | 8 |
| LED prawo | 9 |
| Radio (CE) | 3 |
| Radio (CSN) | 2 |
| ESC (silnik) | 10 |
| Servo (sterowanie) | 4 |

---

## 📡 Komunikacja RF24
- **Adres**: `"00001"`
- **Szybkość**: 250 kbps (niskoenergowe, długi zasięg)
- **Moc**: PA_LOW (niska moc)
- **AutoAck**: Wyłączony (dla lepszej wydajności)
- **Timeout**: 1 sekunda - brak sygnału → resetowanie danych

---

## 🎮 Struktura danych transmitowanych
```cpp
struct Data_Package {
  byte j1PotX, j1PotY, j1Button;    // Joystick 1 (gaz/hamulec)
  byte j2PotX, j2PotY, j2Button;    // Joystick 2 (sterowanie)
  byte pot1, pot2;                  // Potencjometry
  byte tSwitch1, tSwitch2;          // Toggle switche
  byte button1, button2, button3, button4;  // Przyciski
};
```

---

## ⚙️ Sterowanie silnikiem (ESC)

### Dwa tryby pracy:
1. **Crawl Mode** (leży) - `pot2 < 117`
   - Impulsowe sterowanie (300ms cykl, 100ms impuls)
   - Mniejsza prędkość, większa kontrola
   - LEDy: **zielone** na bokach

2. **Speed Mode** (normalny) - `pot2 > 137`
   - Ciągłe sterowanie z rampingiem (5µs na krok)
   - Pełna prędkość dostępna
   - LEDy: **czerwone** na bokach

### Zakresy PWM:
- `escStop`: 1500µs (neutral)
- `escMinForward`: 1600µs | `escMaxForward`: 1640µs
- `escMinBackward`: 1390µs | `escMaxBackward`: 1370µs
- `escDeadzone`: 5 jednostek (martwa strefa)

---

## 🔄 Sterowanie servo (kierownica)
- **Zakres**: 40-140° (100° amplitudy)
- **Trim**: -8° (kalibracja)
- **Martwą strefę**: ±8 wokół centrali (127)
- **Mapowanie**: Mapuje wartość joysticka na kąt servo

---

## 💡 System LED WS2812
- **3 pasy**: Przód (biały), Lewo, Prawo
- **5 diód na pasie**
- Jasność: 120/255

### Logika oświetlenia:
- `tSwitch2 = 0` → wyłączone
- Kolor podstawowy: biały (przód) + zielony/czerwony (boki, zależy od trybu)
- **Miganie pomarańczowe** na boku podczas skrętu > 15° (sygnalizacja)

---

## 🛡️ Zabezpieczenia
1. **Watchdog Timer** (1 sekunda) - reset mikrokontrolera w razie zawieszenia
2. **Timeout komunikacji** (1s) - powrót do stanu bezpiecznego bez sygnału
3. **Soft deadzone** - ignorowanie małych ruchów joysticka
4. **Ramping ESC** - stopniowa zmiana prędkości (unikanie szarpaków)

---

## ⚠️ Potencjalne problemy/uwagi
1. ⚠️ Brak zabezpieczenia przed przepięciami na pinach RF24
2. ⚠️ Zmienna `speedMode` nigdy nie jest używana w kodzie
3. ⚠️ `pot1` jest odbierany, ale nigdy nie wykorzystywany
4. ✓ Implementacja crawl mode jest ciekawa - impulsowe sterowanie dla niskich prędkości
5. ✓ Układ LED do sygnalizacji kierunku dobrze zaprojektowany
