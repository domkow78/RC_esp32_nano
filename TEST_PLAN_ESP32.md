# Test plan: minimum do pierwszego testu na Nano ESP32

## Cel

Zweryfikować, że firmware jest gotowy do podstawowego testu na docelowym module Arduino Nano ESP32, bez pełnej integracji wszystkich sensorów i funkcji V3.

## Zakres minimalny

### 1. Konfiguracja builda
- [ ] dodać poprawny projekt PlatformIO dla Nano ESP32
- [ ] ustawić `board = arduino-nano-esp32`
- [ ] ustawić `framework = arduino`
- [ ] dodać zależności:
  - `RF24`
  - `ESPAsyncWebServer`
- [ ] sprawdzić, że build przechodzi bez błędów środowiskowych

### 2. Weryfikacja kompilacji
- [ ] uruchomić kompilację lokalnie
- [ ] poprawić ewentualne błędy include / API / typów
- [ ] potwierdzić, że firmware bootuje na ESP32

### 3. Prawidłowa konfiguracja pinów
- [ ] zweryfikować mapę pinów dla:
  - ESC
  - serwo
  - nRF24
  - Wi‑Fi / debug serial
- [ ] sprawdzić, czy nie ma konfliktów GPIO
- [ ] potwierdzić kompatybilność z Arduino Nano ESP32

### 4. Podstawowa inicjalizacja systemu
- [ ] uruchomić `SystemManager`
- [ ] sprawdzić przejście przez `Init -> Ready -> Manual`
- [ ] sprawdzić działanie `failsafe`
- [ ] zweryfikować logi systemowe

### 5. Radio nRF24
- [ ] podłączyć moduł nRF24L01 do docelowego ESP32
- [ ] sprawdzić odbiór pakietów
- [ ] sprawdzić poprawność `HEARTBEAT`
- [ ] zbadać timeout i `SAFE_STOP`
- [ ] zweryfikować brak zawieszania po utracie sygnału

### 6. Sterowanie napędem
- [ ] podłączyć ESC i serwo na stanowisku testowym
- [ ] sprawdzić neutral / brake / reverse
- [ ] sprawdzić limitowanie sterowania według trybu
- [ ] potwierdzić brak przypadkowych skoków PWM

### 7. Wi‑Fi i diagnostyka
- [ ] uruchomić AP Wi‑Fi
- [ ] sprawdzić dostęp do panelu WWW
- [ ] sprawdzić WebSocket telemetry
- [ ] potwierdzić, że stan pojazdu jest widoczny w czasie rzeczywistym

### 8. Testy bezpieczeństwa
- [ ] test braku sygnału radio
- [ ] test `SAFE_STOP`
- [ ] test resetu systemu
- [ ] sprawdzić, czy ESC wyłącza się w bezpieczny sposób

### 9. Testy na stole / laboratorium
- [ ] uruchomienie bez pełnego pojazdu
- [ ] testy z zewnętrznym obciążeniem i bez obciążenia
- [ ] obserwacja temperatur / odczytów / logów

### 10. Wykluczenie z pierwszego etapu
- [ ] TF-Luna
- [ ] MPU6050
- [ ] SSD1306
- [ ] WS2812B
- [ ] pomiar baterii
- [ ] testy EMC
- [ ] testy terenowe pojazdu

## Kryterium wejścia do testu

Pierwszy test na ESP32 powinien przejść dopiero wtedy, gdy:
- build jest poprawny,
- system startuje bez resetów,
- radio działa,
- failsafe działa,
- ESC / servo są kontrolowane,
- telemetry jest widoczna.

## Uwaga

To jest minimalny test MVP dla docelowego hardware, a nie pełne testy V3. Wszelkie czujniki zewnętrzne i integracje dodatkowe powinny zostać dodane dopiero po potwierdzeniu stabilności podstawowego sterowania.
