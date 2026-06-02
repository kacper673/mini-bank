#pragma once
#include <random>
#include <map>
#include <stdexcept>

#include "account.h"
#include "validation.h"

class Bank {
public:
    long long createAccount(const std::string& name,
                            const std::string& pesel,
                            const std::string& dateOfBirth) {
        if (!isPeselValid(pesel))
            throw std::invalid_argument("Nieprawidlowy PESEL.");
        if (!isDateOfBirthValid(dateOfBirth))
            throw std::invalid_argument("Nieprawidlowa data urodzenia.");

        long long number = generateUniqueAccountNumber();
        accounts.emplace(number, Account(name, pesel, dateOfBirth, number));
        return number;
    }

    Account& getAccount(long long number) {
        if (!isValidAccountNumber(number))
            throw std::invalid_argument("Nieprawidlowy numer konta");

        auto it = accounts.find(number);
        if (it == accounts.end()) {
            throw std::runtime_error("Konto o podanym numerze nie istnieje.");
        }
        return it->second;
    }

    void deposit(long long number, long long amountGr) {
        getAccount(number).deposit(amountGr);
    }

    void withdraw(long long number, long long amountGr) {
        getAccount(number).withdraw(amountGr);
    }

    void printAccountInfo(long long number) {
        getAccount(number).printInfo();
    }

    void printAllAccounts(){
        if (accounts.empty()) {
            std::cout << "Brak kont w systemie.\n";
            return;
        }
        for (auto& pair : accounts) {
            pair.second.printInfo();
        }
    }

    static long long generateAccountNumber() {
        static std::mt19937_64 engine{ std::random_device{}() };
        static std::uniform_int_distribution<long long> dist(1'000'000'000LL,
                                                             9'999'999'999LL);
        return dist(engine);
    }

private:
    std::map<long long, Account> accounts;

    long long generateUniqueAccountNumber() {
        long long number;
        do {
            number = generateAccountNumber();
        } while (accounts.count(number) > 0);
        return number;
    }
};