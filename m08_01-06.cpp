#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <typeinfo>

//  1    - integer (4 bytes) 10 digits (-2 147 483 648 to  +2 147 483 647)
//  1    - long    (4 bytes) 19 digits (-9 223 372 036 854 775 808 to +9 223 372 036 854 775 807)
//  1.0  - double  (8 bytes) 16 digits (-1.7E-308      to 1.7E+308)
//  1.0f - float   (4 bytes)  7 digits (-3.4E-38       to 3.4E+38)
//  1.0l - long double (not less than double up to 18-19 digits)

template<typename T>
bool isNumber(const std::string & str, T & number, int & maxDigits, double & maxMantissa) {
    auto size = str.size();
    if (size == 0) {
        return false;
    }

    int maxExponentaDigits = 1;

    switch (*typeid(number).name()) {
    case 'i':
        // std::cout << "integer\n";
        maxMantissa = 2.1;
        maxDigits = 10;
        maxExponentaDigits = 1;
        break;
    case 'l':
        // std::cout << "long\n";
        maxMantissa = 2.1; // 9.2;
        maxDigits = 10; // 19;
        maxExponentaDigits = 1; // 2;
        break;
    case 'f':
        // std::cout << "float\n";
        maxMantissa = 3.4;
        maxDigits = 38;
        maxExponentaDigits = 2;
        break;
    case 'd':
        // std::cout << "double\n";
        maxMantissa = 1.7;
        maxDigits = 308;
        maxExponentaDigits = 3;
        break;
    default :
        maxDigits = 0;
        maxMantissa = 0;
        return false;
    }

    bool numberIsNegative = false;
    bool signHasAlreadyBeen = false;
    size_t start = 0;
    char c = '0';
    for (size_t i = 0; i < size; ++i) {
        c = str[i];
        if (c == '0' || c == ' ') {
                ++start;
        }
        else if (c == '+' || c == '-') {
            if (signHasAlreadyBeen) {
                std::cout << "Incorrect number format!\n";
                return false;
            }
            ++start;
            signHasAlreadyBeen = true;
            numberIsNegative = (c == '-');
        }
        else {
            break;
        }
    }

    bool hasFractions = false;
    bool completeFractionZeros = false;
    int digits = 0;
    int intDigits = 0;
    int fractionZeros = 0;
    int symbols = 0;
    int startExponenta = 0;
    for (size_t i = start; i < size; ++i) {
    	c = str[i];
        if (std::isdigit(c)) {
            ++digits;
            if (!hasFractions && digits > maxDigits) {
                std::cout << "Too big number (digits)!\n";
                return false;
            }
            if (hasFractions && !completeFractionZeros) {
                if (c == '0') {
                    ++fractionZeros;
                }
                else {
                    completeFractionZeros = true;
                }
            }
        }
        else if (c == '.') {
            if (hasFractions) {
                std::cout << "Too many decimal separators!\n";
                return false;
            }
            intDigits = digits;
            ++symbols;
            hasFractions = true;
        }
        else if (c == 'e' || c == 'E') {
            ++symbols;
            startExponenta = start + digits + symbols;
            break;
        }
        else {
            break;
        }
    }

    if (digits == 0) {
        if (start > 0 || symbols > 0) {
            number = 0;
            return true;
        }
        return false;
    }

    if (!hasFractions) {
        intDigits = digits;
    }

/*
    std::cout << "\nstart = " << start << " digits = " << digits << " symbols = " << symbols << "\n";
    std::cout << "substr(start, digits + symbols) = " << str.substr(start, digits + symbols) << "\n";
    std::cout << "intDigits      = " << intDigits << "\n";
    std::cout << "startExponenta = " << startExponenta << " e='" << str.substr(startExponenta, size - startExponenta) << "'\n";
    std::cout << "fractionZeros  = " << fractionZeros << "\n\n";
*/

    bool exponentaIsNegative = false;
    int exponentaDigits = 0;
    int exponenta = 0;
    if (startExponenta > 0) {
        signHasAlreadyBeen = false;
        for (size_t i = startExponenta; i < size; ++i) {
            char c = str[i];
            if (c == '0') {
                ++startExponenta;
            }
            else if (c == '+' || c == '-') {
                if (signHasAlreadyBeen) {
                    break;
                }
                ++startExponenta;
                signHasAlreadyBeen = true;
                exponentaIsNegative = (c == '-');
            }
            else {
                break;
            }
        }

        for (size_t i = startExponenta; i < size; ++i) {
            if (std::isdigit(str[i])) {
                if (exponentaDigits > maxExponentaDigits) {
                    if (exponentaIsNegative) {
                        break;
                    }
                    std::cout << "Too big the power of the number!\n";
                    return false;
                }
                ++exponentaDigits;
            }
            else if (str[i] == '.') {
                std::cout << "Incorrect power of number!\n";
                return false;
            } 
            else {
                break;
            }
        }

        if (exponentaDigits > 0) {
            exponenta = std::stoi(str.substr(startExponenta, exponentaDigits));
            exponenta = exponentaIsNegative ? -exponenta : exponenta;
        }
    }

    int powerNumber = 0;
    if (intDigits > 0) {
        powerNumber = intDigits + exponenta - 1;
    }
    else {
        powerNumber = exponenta - fractionZeros - 1;
    }

/*
    std::cout << "exponenta = " << exponenta << "\n";
    std::cout << "maxDigits = " << maxDigits << "\n";
    std::cout << "powerNumber = " << powerNumber << "\n";
*/

    if (powerNumber > maxDigits) {
        std::cout << "Too big number (power)!\n";
    	return false;
    }

    double doubleNumber = std::stod(str.substr(start, digits + symbols));
    if (exponenta != 0) {
        doubleNumber *= std::pow(10, exponenta);
    }
    double mantissa = doubleNumber / std::pow(10, powerNumber);

    if (powerNumber == maxDigits && mantissa > maxMantissa) {
        std::cout << "Too big number (mantissa)!\n";
        return false;
    }

    number = numberIsNegative ? -doubleNumber : doubleNumber;

    return true;
}

template<typename T>
bool getNumber(const std::string& prompt, T& number) {
    std::string input = "";
    std::cout << prompt;
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            return false;
        }

        int maxDigits = 0;
        double maxMantissa = 0;
        if (isNumber(input, number, maxDigits, maxMantissa)) {
            break;
        }

        std::string range = "from -" + std::to_string(maxMantissa) + "*10^" + std::to_string(maxDigits);
                    range += " to " + std::to_string(maxMantissa) + "*10^" + std::to_string(maxDigits);
        std::cout << "Incorrect value!\n";
        std::cout << "Input a number in the range:\n";
        std::cout << range + ".\n";
        std::cout << "Or type 'exit' for exit.\n";
    }
    return true;
}

int main() {
    std::cout.precision(16); 
    std::cout << "Task 1. Space simulator.\n";
    float f = 0.0, m = 0.0, t = 0.0;
    float distance = 0.0;

    if (getNumber("Enter the force f (kg*m/sec^2): ", f)) {
        std::cout << "f=" << f << "\n";
        if (getNumber("Enter the mass m (kg): ", m)) {
            if (getNumber("Enter the time t (sec): ", t)) {
            	if (m > 0) {
            		distance = t * t * f / m / 2;
            	    std::cout << "The distance is " << distance << " m.\n";
            	}
            }
        }
    }

    std::cout << "\nTask 2. Immolate improved.\n";
    float health = 0.0;
    float magicResistance = 0.0;

    getNumber("Enter the health in range 0.0 - 1.0 : ", health);
    getNumber("Enter the magic resistance in range 0.0 - 1.0 : ", magicResistance);
    if (health > 0.0 && health <= 1.0 && magicResistance >= 0.0 && magicResistance <= 1.0) {
        if (std::abs(magicResistance - 1) < 0.001) {
        	std::cout << "The orc is immortal.\n";
        }
        else {
            const float forceReduction = 1.0f - magicResistance;
            float damage = 0;
            while (health > 0) {
                do {
                    if (getNumber("Enter the spell's power (0.0 - 1.0) : ", damage)) {
                        if (damage > 0 && damage <= 1) {
                            break;
                        }
                    }
                } while (true);
                damage *= forceReduction;
                std::cout << "   Caused damage = " << damage << "\n";
                health -= (health > damage ? damage : health);
                std::cout << "   Health = " << health << "\n";
            }
        }
    }

    std::cout << "\nTask 3. The toy story.\n";
    const int cubeEdge = 5;
    int cubes = 0;
    int bigCubeSize = 0;
    int maxSet = 0;
    float barX = 0.0;
    float barY = 0.0;
    float barZ = 0.0;

    std::cout << "Enter bar sizes (X, Y, Z). Minimum sizes in all dimensions are\n"
              << "not less than " << cubeEdge << " metric units.\n";
    getNumber("Enter the dimension X: ", barX);
    getNumber("Enter the dimension Y: ", barY);
    getNumber("Enter the dimension Z: ", barZ);
    if (barX >= cubeEdge && barY >= cubeEdge && barZ >= cubeEdge) {
        cubes = barX / cubeEdge;
        cubes *= (barY / cubeEdge);
        cubes *= (barZ / cubeEdge);
        bigCubeSize = std::cbrt(cubes);
        maxSet = std::pow(bigCubeSize, 3);
        std::cout << cubes << " cubes can be made from a "
                  << barX << "x" << barY << "x" << barZ << " metric units bar.\n";
        std::cout << "A set of " << maxSet << " cubes can be composed for a "
                  << bigCubeSize << "x" << bigCubeSize << "x" << bigCubeSize << " cubes big cube.\n";
    }

    std::cout << "\nTask 4. Progress bar.\n";
    float fileSize = 0.0;
    float transferRate = 0.0;
    getNumber("Specify the file size to download: ", fileSize);
    getNumber("Enter the data transfer rate: ", transferRate);
    if (fileSize > 0) {
        if (transferRate > 0) {
            int time = 0;
//            const float accuracy = 0.0001;
            float transfered = 0.0;
            int progress = 0;
            while (progress < 100) {
                ++time;
                transfered += transferRate;
                if (transfered > fileSize) {
                    transfered = fileSize;
                }
                progress = transfered * 100 / fileSize;
                std::cout << time << " sec. have passed. Downloaded ";
                std::cout << transfered << " out of " << fileSize << " mb ("
                          << progress << "%).\n";
            }
        }
        else {
            std::cout << "File could not be downloaded.\n";
        }
    }

    std::cout << "\nTask 5. Kenyan runner.\n";
    int run = 0;

    getNumber("How many kilometers did you run today?: ", run);
    if (run > 0) {
        int time = 0;
        int timeTotal = 0;
        for (int i = 0; i < run; ++i) {
            do {
                if (getNumber("How many seconds passed in the " + std::to_string(i + 1) + " km ? : ", time)) {
                    if (time > 0) {
                        break;
                    }
                }
            } while (true);
            timeTotal += time;
        }
        float average = std::round((float) timeTotal / run);
        int minutes = average / 60;
        int seconds = average - minutes * 60;
        std::cout << "Average pace: " << minutes << " minutes " << seconds << " seconds.\n";
    }

    std::cout << "\nTask 6. The pendulum.\n";
    float amplitude = 0;
    float minimum = 0;
    const float attenuation = 8.4;

    getNumber("Enter the amplitude: ", amplitude);
    getNumber("Enter the amplitude at wich we assume the oscillation has stoped: ", minimum);
    if (minimum > 0 && amplitude > minimum) {
    	int oscillations = 0;
    	float decreaseRate = (100 - attenuation) / 100;
    	do {
    	    ++oscillations;
    	    amplitude *= decreaseRate;
    	    std::cout << oscillations << ". amplitude = " << amplitude << "\n";
    	} while (amplitude > minimum);
        std::cout << "The amplitude will decrease to the minimum (" << minimum << " cm) after " << oscillations << " oscillations.\n";
    }

    return 0;
}
