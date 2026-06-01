#pragma once
#include <random>
#include <ctime>
#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>

#include "bank.h"



struct OwnerData {
	OwnerData() = default;
	OwnerData(std::string name_, std::string pesel_, std::string date_of_birth_){
		if (!isPeselValid(pesel_)) throw std::invalid_argument("Nieprawidlowy PESEL.");
		name = name_; pesel = pesel_; date_of_birth = date_of_birth_;
	}

	std::string name;
	std::string pesel;
	std::string date_of_birth;

private:
	bool isPeselValid(const std::string& pesel);
};


class Account {
private:
	long long money_gr;
	long long number;

public:

	Account(std::string name_, std::string pesel_,
		std::string date_of_birth_, long long number_)
		: money_gr(0), number(number_) {   
		owner.name = name_;
		owner.pesel = pesel_;
		owner.date_of_birth = date_of_birth_;
	}

	//preventing creating "empty account"
	Account() = delete;

	OwnerData owner;

	      
	void deposit(long long amountGr);  
	void withdraw(long long amountGr);  

	void printSaldo();
	void printInfo();
};



void Account::withdraw(long long amount_gr) {
	if (amount_gr <= 0) {
		throw std::invalid_argument("Kwota wyplaty musi byc dodatnia.");
	}
	if (amount_gr > money_gr) {
		throw std::runtime_error("Niewystarczajace srodki na koncie.");
	}
	money_gr -= amount_gr;
}


void Account::deposit(long long amount_gr) {
	if (amount_gr <= 0) {
		throw std::invalid_argument("Kwota wplaty musi byc dodatnia.");
	}
	money_gr += amount_gr;
}

void Account::printSaldo() {
	long long zlote = money_gr / 100;
	long long grosze = money_gr % 100;

	std::cout << "Saldo konta nr " << number << ": "
		<< zlote << "."
		<< std::setw(2) << std::setfill('0') << grosze
		<< " PLN\n";
}


void Account::printInfo() {
	std::cout << "================================\n"
		<< "Numer konta : " << number << "\n"
		<< "Wlasciciel  : " << owner.name << "\n"
		<< "PESEL       : " << owner.pesel << "\n"
		<< "Data ur.    : " << owner.date_of_birth << "\n"
		<< "Saldo       : " << money_gr / 100 << "."
		<< std::setw(2) << std::setfill('0') << money_gr % 100
		<< " PLN\n"
		<< "================================\n";
}


bool OwnerData::isPeselValid(const std::string& pesel) {
	if (pesel.size() != 11) {
		return false;
	}
	for (char c : pesel) {
		if (!std::isdigit(static_cast<unsigned char>(c))) {
			return false;
		}
	}

	static const int weights[10] = { 1, 3, 7, 9, 1, 3, 7, 9, 1, 3 };

	int sum = 0;
	for (int i = 0; i < 10; ++i) {
		sum += (pesel[i] - '0') * weights[i];  
	}

	int control = (10 - (sum % 10)) % 10;  
	int lastDigit = pesel[10] - '0';

	return control == lastDigit;
}