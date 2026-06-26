# Architektura sterownika RC -- wersja robocza

## Cel projektu

Budowa odpornego na zakłócenia sterownika RC opartego na Arduino Nano
ESP32 z możliwością rozbudowy o WWW i telemetrię.

## Główne elementy

-   Arduino Nano ESP32
-   nRF24L01
-   1 × ESC do silnika BLDC
-   1 × serwo kierunku

## Architektura zasilania

Źródłem energii jest pakiet akumulatorów.

-   ESC zasilany bezpośrednio z pakietu.
-   Osobny Buck 5 V dla logiki.
-   Osobny Buck 6 V dla serwa.
-   BEC w ESC nie zasila logiki.

``` text
                Akumulator
                     │
      ┌──────────────┼──────────────┐
      │              │              │
     ESC         Buck 5 V       Buck 6 V
      │              │              │
   Silnik      ESP32+nRF24L01     Serwo
```

## Masa

Wszystkie masy są wspólne i połączone w topologii gwiazdy.

## Wyjścia PWM

Każde wyjście PWM przechodzi przez bufor SN74LVC2G17.

``` text
ESP32 GPIO
    │
SN74LVC2G17
    │
 100 Ω
    │
 Złącze ESC / Serwo
```

Planowana ochrona: - bufor Schmitt Trigger, - rezystor 47--100 Ω, -
opcjonalnie dioda TVS/ESD przy złączu.

## Filtracja

ESP32: - 100 nF - 10--22 µF - 100--220 µF

nRF24L01: - 100 nF - 47--100 µF

Serwo: - 470--1000 µF

## Komunikacja

-   SPI → nRF24L01
-   Wi-Fi AP
-   Serwer WWW
-   Konfiguracja przez przeglądarkę

## Funkcje planowane 

-   watchdog
-   failsafe
-   konfiguracja WWW
-   telemetria
-   zapis ustawień

## Założenia EMC

-   osobne zasilanie logiki,
-   osobne zasilanie serwa,
-   brak zasilania logiki z BEC ESC,
-   wspólna masa (gwiazda),
-   buforowanie PWM,
-   lokalna filtracja,
-   krótkie ścieżki sygnałowe,
-   oddalenie sygnałów od przewodów silnika.

## Dalsze kroki

1.  Schemat elektryczny.
2.  Dobór elementów.
3.  Projekt PCB.
4.  Implementacja oprogramowania.
5.  Testy EMC.
