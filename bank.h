#pragma once
#include <random>


class Bank {

public:
	static long long generateAccountNumber() {
		static std::mt19937_64 engine{ std::random_device{}() };
		static std::uniform_int_distribution<long long> dist(1'000'000'000LL,
			9'999'999'999LL);
		return dist(engine);
	}

};

