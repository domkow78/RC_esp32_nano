# Objectives for V3

## 1) Założenia bazowe V3

- Wersja V3 bazuje na **G_model_v2_2025** (bez zmiany podstawowego modelu sterowania RC).
- Sterowanie pojazdem pozostaje z **pilota (manual RC)** jako tryb domyślny.
- Implementacja ma być zgodna z **Architecture_v2_2026** (podział zasilania, diagnostyka, rozbudowa o WWW).

## 2) Główne cele funkcjonalne

### 2.1 Tryb DEBUG przez WWW (ESP32 AP)

ESP32 uruchamia własny punkt dostępowy Wi-Fi (AP) i serwer WWW do diagnostyki on-line.

Zakres podglądu w panelu debug:
- podstawowe parametry systemu (stan połączenia, tryb pracy, failsafe),
- sygnały odbierane przez odbiornik RF (kanały/manetki/switche),
- wykres napięcia akumulatora w czasie (spadki napięcia pod obciążeniem),
- temperatury układów (diagnostyka termiczna),
- podstawowe zdarzenia i alarmy (np. niski poziom napięcia, utrata sygnału).

### 2.2 Autonomiczne hamowanie awaryjne

- Dodanie funkcji bezpieczeństwa: **automatyczne hamowanie** po wykryciu przeszkody.
- Czujnik początkowy: **1× czujnik ultradźwiękowy z przodu**.
- Cel: redukcja ryzyka kolizji przy jeździe do przodu.
- Priorytet bezpieczeństwa: hamowanie autonomiczne ma nadrzędność nad gazem z pilota.

### 2.3 Tryb „jazda na wprost” (MPU6050)

- Dodanie trybu wspomagania jazdy do przodu opartego o **MPU6050**.
- W trybie aktywnym system utrzymuje kierunek jazdy „na wprost”.
- Utrzymanie kierunku trwa do momentu poruszenia manetki kierunku przez operatora.
- Ręczna korekta kierunku z pilota natychmiast wyłącza wymuszenie toru prostego.

## 3) Wymagania niefunkcjonalne

- Zachowanie responsywności sterowania RC mimo równoległego działania WWW/diagnostyki.
- Bezpieczne stany przejściowe i failsafe przy zaniku RF lub błędzie czujników.
- Czytelny interfejs diagnostyczny (dashboard mobilny/desktop).
- Logika alarmów bez blokowania pętli sterowania czasu rzeczywistego.

## 4) Kryteria akceptacji V3

Projekt V3 uznaje się za spełniony, gdy:

1. Pojazd działa w trybie manualnym RC jak w V2.
2. Serwer WWW na AP pokazuje kluczowe dane systemowe, RF, napięcie i temperatury.
3. Widoczny jest trend/wykres napięcia akumulatora podczas pracy.
4. Wykrycie przeszkody z przodu powoduje skuteczne hamowanie awaryjne.
5. Tryb „jazda na wprost” stabilizuje kierunek, a ruch manetki kierunku oddaje pełną kontrolę operatorowi.

## 5) Zakres pierwszej iteracji wdrożenia

- Integracja AP + WWW debug (MVP dashboard).
- Integracja 1 czujnika ultradźwiękowego (przód) + próg hamowania.
- Integracja MPU6050 + prosty algorytm utrzymania kierunku.
- Testy terenowe: manual RC, hamowanie awaryjne, stabilizacja jazdy na wprost.

