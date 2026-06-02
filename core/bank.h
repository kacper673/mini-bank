#pragma once
#include <random>
#include <map>
#include <stdexcept>
#include <set>

#include "account.h"
#include "validation.h"

class Bank {
public:
    long long createAccount(const std::string& name,
        const std::string& pesel,
        const std::string& dateOfBirth) {
        if (!isPeselValid(pesel))
            throw std::invalid_argument("Nieprawidlowy PESEL.");
        if (isPeselRegistered(pesel))
            throw std::invalid_argument("Konto o tym numerze PESEL juz istnieje");
        if (!isDateOfBirthValid(dateOfBirth))
            throw std::invalid_argument("Nieprawidlowa data urodzenia.");

        long long number = generateUniqueAccountNumber();
        accounts.emplace(number, Account(name, pesel, dateOfBirth, number));
        registeredPesels.insert(pesel);
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

     // const overload for QT UI
     const Account& getAccount(long long number) const {
         auto it = accounts.find(number);
         if (it == accounts.end())
             throw std::runtime_error("Konto o podanym numerze nie istnieje.");
         return it->second;
     }

    // getter for UI
        std::vector<long long> getAccountNumbers() const {
        std::vector<long long> nums;
        for (const auto& [num, acc] : accounts)
            nums.push_back(num);
        return nums;
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

    void printAllAccounts() {
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

    //optimal way of keeping registered pesels, finding in O(logn) time
    std::set<std::string> registeredPesels;

    bool isPeselRegistered(const std::string& pesel) const {
        if (registeredPesels.find(pesel) != registeredPesels.end()) return true;
        return false;
    }

    long long generateUniqueAccountNumber() {
        long long number;
        do {
            number = generateAccountNumber();
        } while (accounts.count(number) > 0);
        return number;
    }

};