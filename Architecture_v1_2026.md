# Architektura elektroniki modelu RC -- założenia

## Cel

Zwiększenie odporności układu na zakłócenia EMC oraz wyeliminowanie
zawieszania się mikrokontrolera.

## Główne założenia

-   Mikrokontroler: Arduino Nano ESP32.
-   Komunikacja radiowa: nRF24L01.
-   Sterowanie:
    -   1 × ESC (silnik BLDC),
    -   1 × serwo kierunku.

## Architektura zasilania

Nie korzystać z BEC wbudowanego w ESC do zasilania logiki.

Zastosować dwa niezależne przetworniki DC/DC (buck):

### 1. Buck dla logiki

Zasilany bezpośrednio z akumulatora.

Zasila: - Arduino Nano ESP32, - nRF24L01, - pozostałą elektronikę
logiczną.

### 2. Buck dla serwa

Również zasilany bezpośrednio z akumulatora.

Zasila wyłącznie serwo.

### ESC

ESC zasilany bezpośrednio z akumulatora.

BEC w ESC pozostaje nieużywany lub wykorzystywany jedynie pomocniczo po
testach.

## Połączenie mas

Wszystkie masy powinny być wspólne i połączone w jednym punkcie
(topologia gwiazdy):

-   akumulator,
-   ESC,
-   buck logiki,
-   buck serwa.

## Sygnały PWM

PWM z ESP32 do: - ESC, - serwa.

Planowane zastosowanie układu 74HCT14 jako bufora: - poprawa kształtu
zboczy, - większa odporność na zakłócenia, - ochrona wyjść
mikrokontrolera.

## Filtracja zasilania

### Przy ESP32

-   100 nF przy zasilaniu,
-   10--22 µF lokalnie,
-   100--220 µF przy wejściu zasilania płytki.

### Przy nRF24L01

-   100 nF,
-   47--100 µF możliwie blisko modułu.

### Przy serwie

-   470--1000 µF blisko złącza serwa.

## Komunikacja

-   nRF24L01 przez SPI.
-   W przyszłości możliwe uruchomienie punktu dostępowego Wi-Fi (AP)
    oraz serwera WWW do konfiguracji modelu.
-   W przypadku dodania drugiego mikrokontrolera preferowana komunikacja
    UART zamiast I²C.

## Dlaczego Arduino Nano ESP32

Zalety: - Wi-Fi, - możliwość uruchomienia AP i serwera WWW, - duża
wydajność, - obsługa FreeRTOS, - bogaty ekosystem bibliotek, -
wystarczająca moc do jednoczesnej obsługi radia, serwa, ESC oraz
interfejsu WWW.

## Oczekiwane korzyści

-   znacznie większa odporność na zakłócenia,
-   eliminacja resetów powodowanych spadkami napięcia,
-   stabilniejsza praca nRF24L01,
-   możliwość dalszej rozbudowy o konfigurację przez WWW i telemetrię.
