# 🚗 RC_esp32_nano

Projekt zdalnie sterowanego auta RC rozwijany w kilku wersjach sprzętowo-programowych.

## 📖 Opis ogólny

Repozytorium zawiera dwie główne linie rozwoju:

- **Wersja 1 (2024)** – sterowanie przez **WiFi TCP/IP** (ATmega UNO + ESP8266)
- **Wersja 2 (2025)** – sterowanie przez **nRF24L01** + opcjonalne **IMU MPU6050** (ATmega Nano)

Wspólnym celem obu wersji jest kontrola pojazdu RC (napęd + skręt), z naciskiem na niskie opóźnienia i bezpieczeństwo pracy.

---

## 🧩 Struktura projektu

- Odbiorniki: [src/Receiver](src/Receiver)
	- [src/Receiver/01_rx_atmega_uno/AP_TCP_Server_RC_Car.ino](src/Receiver/01_rx_atmega_uno/AP_TCP_Server_RC_Car.ino)
	- [src/Receiver/02_rx_atmega_nano/receiver_nano.ino](src/Receiver/02_rx_atmega_nano/receiver_nano.ino)
- Nadajniki: [src/Transmitter](src/Transmitter)
	- [src/Transmitter/02_tx_nano/transmitter_nano.ino](src/Transmitter/02_tx_nano/transmitter_nano.ino)
- Dokumentacja i materiały: [doc](doc)

---

## 🌐 Wersja 1 (2024) – WiFi TCP

### Architektura

Klient (telefon/PC) wysyła komendy `F/B/L/R/S` po TCP do ESP8266 działającego jako Access Point.
ESP8266 przekazuje je do ATmega UNO, który steruje 4 silnikami DC przez piny kierunku.

### Cechy

- Prosta implementacja i łatwe testy sieciowe
- Komendy dyskretne (bez płynnej regulacji)
- Dobra baza edukacyjna do nauki komunikacji WiFi + Arduino

---

## ⚡ Wersja 2 (2025) – nRF24 + IMU

### Architektura

Nadajnik na Nano odczytuje joysticki/przyciski/potencjometry (oraz opcjonalnie MPU6050), pakuje dane i wysyła je przez nRF24.
Odbiornik na Nano dekoduje pakiet i steruje:

- **ESC** (napęd)
- **Servo** (skręt)
- **LED WS2812** (sygnalizacja stanu/skrętu)

### Cechy

- Niska latencja i bardziej „radiowy” charakter sterowania
- Dwa tryby napędu (crawl/speed)
- Deadzone, ramping, watchdog i timeout sygnału
- Możliwość sterowania ruchem kontrolera (IMU mode)

---

## 📡 Dane i komunikacja

W V2 używana jest zwarta struktura `Data_Package` (14 bajtów), która zawiera m.in. osie joysticków, stany przycisków i przełączników.
To mieści się bezpiecznie w limicie bufora nRF24 (32 bajty).

---

## 🛡️ Bezpieczeństwo i niezawodność

W projekcie zastosowano m.in.:

- timeout utraty sygnału
- watchdog (receiver V2)
- neutral/stop jako stan bezpieczny
- ograniczenia i martwe strefy sterowania

---

## 📚 Dokumenty analityczne

- Odbiornik V1: [G_model_v1_2024.md](G_model_v1_2024.md)
- Odbiornik V2: [G_model_v2_2025.md](G_model_v2_2025.md)
- Nadajnik V2: [P_model_v2_2025.md](P_model_v2_2025.md)

---

## 🔧 Biblioteki

Najważniejsze biblioteki użyte w kodzie:

- `RF24` (nRF24L01)
- `FastLED` (WS2812)
- `Servo`
- `Wire` (I2C / MPU6050)
- `avr/wdt` (watchdog)

---

## 🚀 Kierunek rozwoju

Repozytorium zawiera również katalogi pod kolejne iteracje (Receiver/Transmitter `03_*`), co wskazuje na plan dalszej rozbudowy platformy.

---

Ostatnia aktualizacja README: czerwiec 2026.
