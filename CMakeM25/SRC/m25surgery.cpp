#include "m25surgery.h"

void greeting(const std::string& txt) {
	std::cout << txt << std::endl;
}

void incision(const COORD& begin, const COORD& end) {
	std::cout << "The incision is made." << std::endl;
	std::cout << "Start coordinates: x = " << begin.x << ", y = " << begin.y << std::endl;
	std::cout << "End coordinates:   x = " << end.x << ", y = " << end.y << std::endl;
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
