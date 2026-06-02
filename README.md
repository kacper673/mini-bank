# Mini System Bankowy

Konsolowy + GUI system bankowy w C++

## Funkcje
- Tworzenie konta (walidacja PESEL, daty urodzenia, unikalności)
- Wpłata / wypłata środków
- Wyświetlanie informacji o koncie
- Blokada wypłaty ponad dostępne saldo
- Interfejs graficzny Qt (Qt Widgets)

## Technologie
- C++17
- Qt 6.10.1 (Widgets)
- CMake 3.16+
- MinGW 13.1.0

## Budowanie

### Wymagania
- Qt 6.x z komponentem MinGW 64-bit
- CMake 3.16+

### Kroki
```bash
mkdir build && cd build

set QTFRAMEWORK_BYPASS_LICENSE_CHECK=1
cmake .. -G "MinGW Makefiles" \
  -DCMAKE_PREFIX_PATH="C:/Qt/6.10.1/mingw_64" \
  -DCMAKE_C_COMPILER="C:/Qt/Tools/mingw1310_64/bin/gcc.exe" \
  -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1310_64/bin/g++.exe"

cmake --build .

# Kopiowanie bibliotek Qt (wymagane do uruchomienia)
C:\Qt\6.10.1\mingw_64\bin\windeployqt.exe bank.exe
```

## Struktura projektu
```
bank/
├── CMakeLists.txt
├── main.cpp
├── core/
│   ├── account.h     # klasa konta bankowego
│   ├── bank.h        # zarządzanie kontami
│   └── validation.h  # walidacja PESEL, daty, numeru konta
└── ui/
    ├── mainwindow.h
    └── mainwindow.cpp
```
