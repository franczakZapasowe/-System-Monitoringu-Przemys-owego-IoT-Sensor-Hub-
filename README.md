# Industrial IoT Sensor Hub (Zdalny Hub Monitoringu)

Wielowątkowy system komunikacji sieciowej Klient-Serwer napisany w C++20, wykorzystujący surowe gniazda TCP (POSIX Sockets). Projekt symuluje system telemetrii przemysłowej czasu rzeczywistego, służący do monitorowania parametrów maszyn oraz zdalnego zarządzania ich stanem awaryjnym.

## ⚙️ Architektura i Założenia Projektowe

System został zaprojektowany z myślą o niskich opóźnieniach (low-latency) i wydajności operacyjnej, charakterystycznej dla środowisk produkcyjnych (np. ciągłość linii montażowych).

Zamiast zasobożernych formatów tekstowych (JSON/XML), aplikacja realizuje własny **protokół binarny**. Dane przesyłane są bezpośrednio jako zserializowane struktury C++ (C-structs), co minimalizuje narzut na sieć i gwarantuje przewidywalny układ w pamięci.

### Główne funkcjonalności:

* **Wielowątkowy Serwer (Multithreading):** Główny wątek asynchronicznie akceptuje nowe połączenia, podczas gdy procesy robocze (pracujące w trybie `detach()`) obsługują odrębne maszyny równolegle, zapobiegając blokowaniu systemu.
* **Ciągły Strumień Danych (Continuous Stream):** Klienci raportują swój stan (np. identyfikator, temperatura silnika) w nieprzerwanej pętli.
* **Komunikacja Dwukierunkowa (Zabezpieczenie przed Deadlockiem):** Wdrożono zsynchronizowany mechanizm odpowiedzi. Serwer weryfikuje każdą paczkę danych i natychmiast odsyła strukturę z komendą sterującą.
* **Zdalne Wyzwalanie Alarmów:** W przypadku wykrycia anomalii (np. przekroczenie krytycznego progu temperatury), serwer automatycznie wysyła sygnał przerwania (Emergency Stop), wymuszając bezpieczne zamknięcie obwodu przez klienta.

## 🛠️ Technologie

* **Język:** C++20
* **Sieć:** POSIX Sockets (`<sys/socket.h>`, `<netinet/in.h>`, `<arpa/inet.h>`)
* **Współbieżność:** Biblioteka standardowa C++ (`<thread>`, `<chrono>`)
* **Kompilator:** GCC / G++ (wsparcie dla flagi `-pthread`)

## 📁 Struktura Projektu

Zastosowano zasadę DRY (Don't Repeat Yourself), wydzielając wspólny protokół komunikacyjny do niezależnego pliku nagłówkowego.

* `serwer.cpp` - Główna logika centralki, zarządzanie portami i delegowanie wątków.
* `klient.cpp` - Symulator czujnika maszyny, generujący i wysyłający pakiety binarne.
* `Structures.h` - Definicje współdzielonych, binarnych struktur przesyłowych (`MachineState`, `ServerCommand`).

## 🚀 Kompilacja i Uruchomienie

System przeznaczony jest dla środowisk typu UNIX/Linux.

**1. Kompilacja plików:**
Otwórz terminal w katalogu z projektem i skompiluj kod używając poniższych komend:

```bash
g++ -std=c++20 serwer.cpp -o serwer -pthread
g++ -std=c++20 klient.cpp -o klient

```

**2. Uruchomienie Serwera (Centralki):**

```bash
./serwer

```

*Serwer rozpocznie nasłuchiwanie na porcie 5000.*

**3. Uruchomienie Maszyn (Klientów):**
Otwórz nowe okna terminala dla każdego kolejnego klienta (możesz uruchomić ich dowolną ilość równolegle):

```bash
./klient

```

---

