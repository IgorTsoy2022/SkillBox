#pragma once

#include <iostream>
#include <string>

struct COORD {
	double x = 0;
	double y = 0;
};

void greeting(const std::string& txt);

void incision([[maybe_unused]] const COORD& begin, [[maybe_unused]] const COORD& end);

void haemostat([[maybe_unused]] const COORD& point);

void tweezers([[maybe_unused]] const COORD& point);

void suture([[maybe_unused]] const COORD& begin, [[maybe_unused]] const COORD& end);