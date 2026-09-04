# TODO - RC_esp32_nano

Dokument do śledzenia realizacji projektu, priorytetów i stanu wdrożenia.

## Legenda statusów

- [ ] do zrobienia
- [~] w toku
- [x] gotowe
- [!] do weryfikacji lub poprawy

## 1. Stan projektu

- [x] Zdefiniowana wizja projektu i podział na wersje V1, V2, V3
- [x] Opisana architektura systemu i warstwy oprogramowania
- [x] Zdefiniowany protokół komunikacyjny VCP
- [ ] Opracowany i wdrożony firmware V3
- [ ] Zintegrowane testy terenowe i stabilnościowe

## 2. Priorytety wdrożenia V3

- [~] Uruchomienie rdzenia firmware na Arduino Nano ESP32
	- [x] Spięcie `main()` z `SystemManager`
	- [x] Dodanie inicjalizacji warstw startowych
	- [x] Dodanie podstawowego cyklu pracy firmware
	- [x] Uporządkowanie faz startowych `SystemManager`
- [ ] Integracja nRF24L01 z obsługą IRQ i heartbeat
- [ ] Integracja TF-Luna jako czujnika przeszkód z przodu
- [ ] Integracja MPU6050 dla podstawowej stabilizacji kierunku
- [ ] Integracja SSD1306 jako panelu statusu
- [ ] Integracja 3 kanałów WS2812B
- [ ] Integracja monitoringu napięcia akumulatora
- [ ] Uruchomienie Wi-Fi AP i panelu WWW do diagnostyki
- [ ] Implementacja telemetrii WebSocket
- [ ] Implementacja failsafe i SAFE_STOP
- [ ] Weryfikacja EMC i odporności na zakłócenia

## 3. Plan prac

### Etap A - Fundamenty

- [ ] Potwierdzić mapę modułów i odpowiedzialności warstw
- [ ] Uzupełnić main.cpp o inicjalizację SystemManager
- [x] Uporządkować konfigurację w katalogu config/
- [x] Zdefiniować wspólne modele danych w models/

### Etap B - Komunikacja i sterowanie

- [ ] Dokończyć implementację VCP po stronie pojazdu
- [ ] Obsłużyć pakiety CONTROL, HEARTBEAT, STATUS i TELEMETRY
- [ ] Dodać CRC i numer sekwencyjny do obsługi ramek
- [ ] Zaimplementować timeout i przejście do SAFE_STOP
- [x] Zdefiniować bazowe typy protokołu VCP

### Etap C - Czujniki i telemetryka

- [ ] Dodać obsługę MPU6050
- [ ] Dodać obsługę TF-Luna
- [ ] Dodać odczyt baterii przez ADC
- [ ] Dodać emisję danych telemetrycznych

### Etap D - Interfejs i diagnostyka

- [ ] Uruchomić Wi-Fi AP na ESP32
- [ ] Zbudować prosty dashboard WWW
- [ ] Dodać panel diagnostyczny i status systemu
- [ ] Dodać logi i kody błędów

### Etap E - Bezpieczeństwo i testy

- [ ] Przetestować reakcję na utratę łączności
- [ ] Przetestować reakcję na błąd czujnika
- [ ] Przetestować przejście do SAFE_STOP
- [ ] Przetestować stabilność sterowania pod obciążeniem
- [ ] Przeprowadzić testy długotrwałe

## 4. Zadania otwarte

- [ ] Ustalić finalny zakres V3 MVP
- [ ] Ustalić kolejność integracji modułów
- [ ] Spisać kryteria akceptacji dla każdego etapu
- [ ] Oznaczyć zadania zależne od hardware
- [ ] Oznaczyć zadania zależne od gotowości protokołu

## 5. Ryzyka

- [ ] Ryzyko przeciążenia pętli sterowania przez WWW i telemetrykę
- [ ] Ryzyko zakłóceń EMC przy PWM, RF i zasilaniu
- [ ] Ryzyko błędów integracji czujników I2C/UART/SPI
- [ ] Ryzyko zbyt późnego przejścia do SAFE_STOP

## 6. Mierniki postępu

- Liczba ukończonych zadań w Etapie A-E
- Stabilność połączenia radiowego
- Czas reakcji na failsafe
- Liczba błędów krytycznych w testach terenowych
- Liczba ukończonych integracji sprzętowych

## 7. Notatki bieżące

- Projekt jest obecnie na etapie architektury i szkieletu kodu.
- Dokumentacja opisuje docelowy stan V3, a nie w pełni gotowe wdrożenie.
- TODO należy aktualizować po każdej większej integracji lub teście.
