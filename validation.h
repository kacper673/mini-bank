#pragma once
#include <string>
#include <cctype>
#include <ctime>


//validation funcitions used across project


inline bool isPeselValid(const std::string& pesel) {
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


inline bool isValidAccountNumber(long long number) {
	return number >= 1'000'000'000LL && number <= 9'999'999'999LL;
}


//helper func for date validation
inline static bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

//helper func for date validation
inline static int daysInMonth(int month, int year) {
    static const int days[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && isLeapYear(year)) return 29;
    return days[month];
}


inline bool isDateOfBirthValid(const std::string& date) {

    if (date.size() != 10)         return false;
    if (date[2] != '.' || date[5] != '.') return false;

    for (int i : {0, 1, 3, 4, 6, 7, 8, 9}) {
        if (!std::isdigit(static_cast<unsigned char>(date[i]))) return false;
    }
    int day = std::stoi(date.substr(0, 2));
    int month = std::stoi(date.substr(3, 2));
    int year = std::stoi(date.substr(6, 4));

	std::time_t now = std::time(nullptr);
	std::tm timeInfo{};
	localtime_s(&timeInfo, &now);
	int currentYear = 1900 + timeInfo.tm_year;

    if (year < 1900 || year > currentYear) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > daysInMonth(month, year)) return false;

    return true;
}



