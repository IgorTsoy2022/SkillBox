﻿#include "m25surgery.h"

void incision(const COORD& begin, const COORD& end) {
	std::cout << "The incision is made." << std::endl;
	std::cout << "Start coordinates: x = " << begin.x
		      << ", y = " << begin.y << std::endl;
	std::cout << "End coordinates:   x = " << end.x
		      << ", y = " << end.y << std::endl;
}

void hemostat(const COORD& point) {
	std::cout << "Used a hemostat to clamp the blood vessels at point with\n";
	std::cout << "coordinates: x = " << point.x
		      << ", y = " << point.y << std::endl;
}

void tweezers(const COORD& point) {
	std::cout << "Used tweezers at point with coordinates: x = " << point.x
		      << ", y = " << point.y << std::endl;
}

bool suture(const COORD& incision_begin, const COORD& incision_end,
	const COORD& suture_begin, const COORD& suture_end) {
    const double epsilon = 0.001;
	std::cout << "Suture is placed on the incision with coordinates of\n";
	std::cout << "the beginning: x = " << suture_begin.x
		      << ", y = " << suture_begin.y << std::endl;
	std::cout << "and the end: x = " << suture_end.x
		      << ", y = " << suture_end.y << std::endl;

	std::cout << "The incision coordinates: the beginning: x = "
		      << incision_begin.x
		      << ", y = " << incision_begin.y << std::endl;
	std::cout << "the end: x = " << incision_end.x
		      << ", y = " << incision_end.y << std::endl;

	return (std::abs(incision_begin.x - suture_begin.x) < epsilon) &&
		   (std::abs(incision_begin.y - suture_begin.y) < epsilon) &&
		   (std::abs(incision_end.x - suture_end.x) < epsilon) &&
		   (std::abs(incision_end.y - suture_end.y) < epsilon);
}

void surgery() {
    std::cout << "Commands for surgery:\n";
    std::cout << "\"scalpel\"  - Make an incision.\n";
    std::cout << "\"hemostat\" - Apply a hemostatic clamp to the point.\n";
    std::cout << "\"tweezers\" - Clamp with tweezers at the point.\n";
    std::cout << "\"suture\"   - Stitch between the points.\n";
    std::cout << "\"exit\"     - Quit.\n";
    bool correct = false;
    COORD incision_begin{}, incision_end{};
    std::string input = "";
    std::cout << "Command > ";
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            break;
        }

        std::string str1 = "";
        std::string str2 = "";
        if (input == "scalpel") {
            std::cout << "    Enter the coordinates for the start and end of the incision:\n";
            std::cout << "    Start coordinates (x y): ";
            while (std::getline(std::cin, input)) {
                if (input == "exit") {
                    break;
                }
                std::stringstream input_stream(input);
                input_stream >> str1 >> str2;
                correct = is_number(str1) && is_number(str2);
                if (correct) {
                    incision_begin.x = std::stod(str1);
                    incision_begin.y = std::stod(str2);
                    break;
                }
                else {
                    std::cout << "    Incorrect number!\n";
                }
                std::cout << "    Start coordinates (x y): ";
            }
            if (correct) {
                std::cout << "    End coordinates (x y): ";
                while (std::getline(std::cin, input)) {
                    if (input == "exit") {
                        break;
                    }
                    std::stringstream input_stream(input);
                    input_stream >> str1 >> str2;
                    correct = is_number(str1) && is_number(str2);
                    if (correct) {
                        incision_end.x = std::stod(str1);
                        incision_end.y = std::stod(str2);
                        break;
                    }
                    else {
                        std::cout << "    Incorrect number!\n";
                    }
                    std::cout << "    End coordinates (x y): ";
                }
            }
            if (correct) {
                incision(incision_begin, incision_end);
            }
        }

        COORD point{}, suture_begin{}, suture_end{};
        if (correct) {
            if (input == "hemostat") {
                std::cout << "    Enter the coordinates of the clamping point\n";
                std::cout << "    (x, y): ";
                while (std::getline(std::cin, input)) {
                    if (input == "exit") {
                        break;
                    }
                    std::stringstream input_stream(input);
                    input_stream >> str1 >> str2;
                    if (is_number(str1) && is_number(str2)) {
                        point.x = std::stod(str1);
                        point.y = std::stod(str2);
                        hemostat(point);
                        break;
                    }
                    else {
                        std::cout << "    Incorrect number!\n";
                    }

                    std::cout << "    Enter the coordinates of the clamping point\n";
                    std::cout << "    (x, y): ";
                }
            }

            if (input == "tweezers") {
                std::cout << "    Enter the coordinates of the clamping point\n";
                std::cout << "    with tweezers (x, y): ";
                while (std::getline(std::cin, input)) {
                    if (input == "exit") {
                        break;
                    }
                    std::stringstream input_stream(input);
                    input_stream >> str1 >> str2;
                    if (is_number(str1) && is_number(str2)) {
                        point.x = std::stod(str1);
                        point.y = std::stod(str2);
                        tweezers(point);
                        break;
                    }
                    else {
                        std::cout << "    Incorrect number!\n";
                    }

                    std::cout << "    Enter the coordinates of the clamping point\n";
                    std::cout << "    with tweezers (x, y): ";
                }
            }

            if (input == "suture") {
                std::cout << "    Enter the coordinates for the start and end of the suture:\n";
                std::cout << "    Start coordinates (x y): ";
                while (std::getline(std::cin, input)) {
                    if (input == "exit") {
                        break;
                    }
                    std::stringstream input_stream(input);
                    input_stream >> str1 >> str2;
                    correct = is_number(str1) && is_number(str2);
                    if (correct) {
                        suture_begin.x = std::stod(str1);
                        suture_begin.y = std::stod(str2);
                        break;
                    }
                    else {
                        std::cout << "    Incorrect number!\n";
                    }
                    std::cout << "    Start coordinates (x y): ";
                }
                if (correct) {
                    std::cout << "    End coordinates (x y): ";
                    while (std::getline(std::cin, input)) {
                        if (input == "exit") {
                            break;
                        }
                        std::stringstream input_stream(input);
                        input_stream >> str1 >> str2;
                        correct = is_number(str1) && is_number(str2);
                        if (correct) {
                            suture_end.x = std::stod(str1);
                            suture_end.y = std::stod(str2);
                            break;
                        }
                        else {
                            std::cout << "    Incorrect number!\n";
                        }
                        std::cout << "    End coordinates (x y): ";
                    }
                }
                if (correct) {
                    if (suture(incision_begin, incision_end,
                        suture_begin, suture_end)) {
                        std::cout << "The operation finished." << std::endl;
                        break;
                    }
                    else {
                        std::cout << "The suture has not been placed over"
                                  << " the entire incision." << std::endl;
                    }
                }
            }
        }
        else {
            std::cout << "The incision hasn't been made. Without an incision\n";
            std::cout << "there is no manipulation.\n";
        }

        std::cout << "Command > ";
    }

}