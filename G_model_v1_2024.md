# Analiza pliku AP_TCP_Server_RC_Car.ino - Receiver 01 (ATmega UNO + ESP8266)

## 📋 Przegląd ogólny
- **Mikrokontroler**: Arduino ATmega UNO
- **Moduł WiFi**: ESP8266 (tryb Access Point)
- **Funkcja**: Serwer TCP odbierający komendy z sieci WiFi i sterujący 4 silnikami DC
- **Komunikacja**: WiFi TCP/IP (Port: 8081)

---

## 🌐 Konfiguracja WiFi

### Parametry AP (Access Point):
```
SSID: esp8266
Hasło: 12345678
Port TCP: 8081
Max klientów: 180 (multi-connect)
```

### Cechy komunikacji:
- Moduł ESP8266 nawiązuje połączenie z ATmega przez SoftwareSerial
- Serial (HW): Komunikacja z ESP8266 (115200 baud)
- SoftwareSerial (A0, A1): Debug output

---

## 🔌 Konfiguracja pinów silników

### Kontrola 4 silników DC (2 kanały PWM):

| Silnik | Enable | In1 | In2 | Funkcja |
|--------|--------|-----|-----|---------|
| Motor A | Pin 2 | Pin 4 | Pin 5 | Przód-lewo |
| Motor B | Pin 3 | Pin 6 | Pin 7 | Przód-prawo |
| Motor C | Pin 8 | Pin 10 | Pin 11 | Tył-lewo |
| Motor D | Pin 9 | Pin 12 | Pin 13 | Tył-prawo |

**Wszystkie Enable (PWM) ustawione na 255 (100% moc)**

---

## 🎮 Komendy sterowania

| Komenda | Działanie | Czas trwania |
|---------|-----------|-------------|
| `F` | Forward (naprzód) | 200ms |
| `B` | Backward (wstecz) | 200ms |
| `L` | Left (skręt lewo) | 200ms |
| `R` | Right (skręt prawo) | 200ms |
| `S` | Stop (zatrzymanie) | - |

### Logika ruchu:
- Każda komenda (F/B/L/R) wykonuje się przez 200ms, następnie `stop()`
- Komendy są wysyłane przez TCP z urządzenia klienckiego

---

## ⚙️ Logika sterowania silnikami

### Forward (Naprzód):
```
Motor A: HIGH, LOW   (przód)
Motor B: HIGH, LOW   (przód)
Motor C: HIGH, LOW   (przód)
Motor D: HIGH, LOW   (przód)
```
→ Wszystkie silniki w jedną stronę

### Backward (Wstecz):
```
Motor A: LOW, HIGH   (tył)
Motor B: LOW, HIGH   (tył)
Motor C: LOW, HIGH   (tył)
Motor D: LOW, HIGH   (tył)
```

### Left (Skręt w lewo):
```
Motor A: HIGH, LOW   (przód)
Motor B: HIGH, LOW   (przód)
Motor C: LOW, HIGH   (tył) ← Różnica!
Motor D: LOW, HIGH   (tył) ← Różnica!
```
→ Lewe koła tył, prawe koła przód = skręt

### Right (Skręt w prawo):
```
Motor A: LOW, HIGH   (tył) ← Różnica!
Motor B: LOW, HIGH   (tył) ← Różnica!
Motor C: HIGH, LOW   (przód)
Motor D: HIGH, LOW   (przód)
```
→ Lewe koła przód, prawe koła tył = skręt

### Stop (Zatrzymanie):
```
Wszystkie In piny: LOW
```

---

## 📊 Architektura komunikacji

```
Urządzenie klienckie
    ↓ (TCP/IP, WiFi)
ESP8266 (Access Point)
    ↓ (SoftwareSerial 115200)
Arduino ATmega UNO
    ↓ (GPIO pins)
4 × Motor Driver → Silniki DC
```

---

## 🛡️ Zabezpieczenia i cechy

1. ✓ **Multi-connect** - obsługuje wielu klientów jednocześnie
2. ✓ **Debug output** - komunikaty diagnostyczne na SoftwareSerial
3. ✓ **Timeout komendy** - każda komenda auto-resetuje się po 200ms
4. ⚠️ Brak potwierdzeń (ACK) z serwera
5. ⚠️ Brak timeout'u komunikacji WiFi
6. ⚠️ Brak Authentication na poziomie aplikacji (tylko WPA2)

---

## ⚠️ Potencjalne problemy/uwagi

1. ⚠️ **Niski PWM (255 = max)** - brak regulacji prędkości, zawsze pełna moc
2. ⚠️ **Brak ramping'u** - zmiana kierunku bez przejścia przez neutral (może obciążyć silniki)
3. ⚠️ **Ograniczona logika** - tylko dyskretne komendy (F/B/L/R/S), brak analogu
4. ⚠️ **Brak feedbacku** - mikrokontroler nie wysyła informacji o stanie
5. ⚠️ **Inicjalizacja SoftwareSerial** - pinów A0, A1 nie ma zwykle dla I/O na ATmega UNO
6. ✓ Czysty model sterowania dla prostego RC samochodu z 4 silnikami
7. ✓ Dobrze wykorzystane WiFi dla bezprzewodowego sterowania

---

## 💡 Spostrzeżenia

- **Wersja 1 (ATmega + ESP8266)**: WiFi TCP, sterowanie dyskretne, pełna moc
- **Wersja 2 (ATmega Nano + nRF24)**: RF24 (bezprzewodowe), sterowanie analogowe, regulacja prędkości
- Wersja 2 jest bardziej zaawansowana z lepszą kontrolą i mniejszą latencją
