#include <iostream>
#include <limits>
#include <string>
#include "bank.h"


void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

long long readAmount() {
    long long zlote = 0;
    long long grosze = 0;

    std::cout << "  Zlote  : ";
    std::cin >> zlote;

    std::cout << "  Grosze : ";
    std::cin >> grosze;

    clearInputBuffer();

    if (zlote < 0 || grosze < 0 || grosze > 99)
        throw std::invalid_argument("Nieprawidlowa kwota.");

    return zlote * 100 + grosze;
}

long long readAccountNumber() {
    long long number;
    std::cout << "Numer konta: ";
    std::cin >> number;
    clearInputBuffer();
    return number;
}


void printMenu() {
    std::cout << "\n===========================\n"
              << "        MINI BANK\n"
              << "===========================\n"
              << " 1. Nowe konto\n"
              << " 2. Wplata\n"
              << " 3. Wyplata\n"
              << " 4. Info o koncie\n"
              << " 5. Lista kont\n"
              << " 0. Wyjscie\n"
              << "===========================\n"
              << "Wybor: ";
}


int main() {
    Bank bank;
    int choice = -1;

    while (choice != 0) {
        printMenu();

        if (!(std::cin >> choice)) {
            std::cin.clear();
            clearInputBuffer();
            std::cout << "[BLAD] Podaj liczbe z zakresu 0-5.\n";
            continue;
        }
        clearInputBuffer();

        try {
            switch (choice) {

            case 1: {
                std::string name, pesel, dateOfBirth;

                std::cout << "Imie i nazwisko            : ";
                std::getline(std::cin, name);

                std::cout << "PESEL                      : ";
                std::getline(std::cin, pesel);

                std::cout << "Data urodzenia (DD.MM.YYYY): ";
                std::getline(std::cin, dateOfBirth);

                long long number = bank.createAccount(name, pesel, dateOfBirth);
                std::cout << "Konto utworzone pomyslnie.\n"
                          << "Numer konta: " << number << "\n";
                break;
            }

            case 2: {
                long long number = readAccountNumber();
                std::cout << "Kwota wplaty:\n";
                long long amount = readAmount();
                bank.deposit(number, amount);
                std::cout << "Wplata zrealizowana.\n";
                bank.printAccountInfo(number);
                break;
            }

            case 3: {
                long long number = readAccountNumber();
                std::cout << "Kwota wyplaty:\n";
                long long amount = readAmount();
                bank.withdraw(number, amount);
                std::cout << "Wyplata zrealizowana.\n";
                bank.printAccountInfo(number);
                break;
            }

            case 4: {
                long long number = readAccountNumber();
                bank.printAccountInfo(number);
                break;
            }

            case 5: {
                bank.printAllAccounts();
                break;
            }

            case 0:
                std::cout << "Do widzenia!\n";
                break;

            default:
                std::cout << "[BLAD] Nieznana opcja. Wybierz 0-5.\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "[BLAD] " << e.what() << "\n";
        }

        if (choice != 0) {
            std::cout << "\nNacisnij Enter aby kontynuowac...";
            std::cin.get();
        }
    }

    return 0;
}
