#include "CMakeM25.h"

void greeting(const std::string& txt) {
	std::cout << txt << std::endl;
}

void incision([[maybe_unused]] const COORD& begin, [[maybe_unused]] const COORD& end) {
	std::cout << "The incision is made." << std::endl;
}

void haemostat([[maybe_unused]] const COORD& point) {
	std::cout << "Used a haemostat to clamp the blood vessels." << std::endl;
}

void tweezers([[maybe_unused]] const COORD& point) {
	std::cout << "Used tweezers." << std::endl;
}

void suture([[maybe_unused]] const COORD& begin, [[maybe_unused]] const COORD& end) {
	std::cout << "Suture." << std::endl;
}
