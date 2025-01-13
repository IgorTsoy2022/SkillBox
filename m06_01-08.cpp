#include <iostream>
#include <string>

bool isInteger(const std::string& str, int& number) {
    if (str.empty()) {
        return false;
    }

    auto size = str.size();

    size_t start = (str[0] == '-' || str[0] == '+') ? 1 : 0;
    size_t leadingZeros = 0;

    for (size_t i = start; i < size; ++i) {
        if (str[i] != '0') {
            break;
        }
        ++leadingZeros;
    }

    start += leadingZeros;

    size_t length = size > start + 9 ? start + 9 : size;

    if (!std::isdigit(str[start])) {
        if (leadingZeros > 0) {
            number = 0;
            return true;
        }
        return false;
    }

    ++start;

    for (size_t i = start; i < length; ++i) {
        if (!std::isdigit(str[i])) {
            number = std::stoi(str.substr(0, i - 1));
            return true;
        }
    }

    number = std::stoi(str.substr(0, length));
    return true;
}

bool getInteger(const std::string& prompt, int& number) {
    std::string input = "";
    std::cout << prompt;
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            return false;
        }

        if (isInteger(input, number)) {
            break;
        }

        std::cout << "Incorrect value!\n";
        std::cout << "Input a number in the range:\n";
        std::cout << "from -999 999 999 to 999 999 999.\n";
        std::cout << "Or type 'exit' for exit.\n";
    }
    return true;
}

int getFibonacci(int sequenceNo) {
	if (sequenceNo < 1) {
		return 0;
	}
	if (sequenceNo < 3) {
		return 1;
	}

	int maxInt = 2147483647;
	int previous = 1;
	int current = 1;
	int next = 0;

	for (int i = 2; i < sequenceNo; ++i) {
	    if (previous >= maxInt - current) {
		    return -1;
    	}

    	next  = previous + current;
    	previous = current;
    	current = next;
	}

	 return current;
}

int main() {
	std::cout << "Task 1. The Cuckoo.\n";
	int number = 0;
	std::cout << "What time is it?\n";
	while (getInteger("(Enter an integer number of hours): ", number)) {
		if (number >= 0 && number < 25) {
			if (number > 12) {
				number -= 12;
			}
			else if (number == 0) {
				number = 12;
			}
			for (int i = 0; i < number; ++i) {
			    std::cout << i + 1 << " cuckoo-cuckoo!\n";
			}
			break;
		}
	}
	
	std::cout << "\nTask 2. The number of digits.\n";	
	if (getInteger("Enter an integer number: ", number)) {
		number = number < 0 ? -number : number;
		std::string str = std::to_string(number);
		std::cout << "1. Convert to string, then count characters: " << str.size() << "\n";
		int digits = number > 0 ? 0 : 1;
		if (number > 0) {
			do {
			    number /= 10;
			    ++digits;
			} while (number > 0);
		}
		std::cout << "2. The arithmetic method: " << digits << "\n";
	}
	
	std::cout << "\nTask 3. Debt collector.\n";
    std::string name;
    int depo = 0;
	std::cout << "Enter debtor's name : ";
	std::getline(std::cin, name);
	if (getInteger("Enter the amount of debt: ", depo)) {
		if (depo < 0) {
			std::cout << "The amount of debt cannot be negative.\n";
		}
		else {
			int repayment = 0;
			depo = -depo;
			while (depo < 0) {
				std::cout << "The amount of dept is: " << -depo << "\n";
				if (!getInteger("Enter the repayment: ", repayment)) {
					break;
		    	}
		    	if (repayment > 0) {
		        	depo += repayment;
		    	}
	    	}
	    	if (depo > 0) {
	    		std::cout << "Accout balance: " << depo << "\n";
	    	}
    	}
	}

	std::cout << "\nTask 4. Lucky ticket.\n";
	int category = 6;
	int ticketNo = 0;
	if (getInteger("Enter ticket number: ", ticketNo)) {
		category /= 2;
		int digitSum = 0;
		int digit = 0;
		for (int i = 0; i < category; ++i) {
			digit = ticketNo % 10;
			ticketNo /= 10;
			digitSum += digit;
		}
		for (int i = 0; i < category; ++i) {
			digit = ticketNo % 10;
			ticketNo /= 10;
			digitSum -= digit;
		}
		if (digitSum == 0) {
			std::cout << "The ticket is lucky!\n";
		}
		else {
			std::cout << "The ticket is ordinary.\n";
		}
	}
	
	std::cout << "\nTask 5. Fibonacci numbers.\n";
	int sequenceNo = 0;
	if (getInteger("Enter sequence number: ", sequenceNo)) {
	    int fib = getFibonacci(sequenceNo);
	    if (fib < 0) {
	        std::cout << "The " << sequenceNo << " Fibonacci number causes an overflow.\n";
	    }
	    else {
	        std::cout << "Fibonacci number: " << fib << "\n";
	    }
	}
	
	std::cout << "\nTask 6. Deposits.\n";
	int deposit = 0;
	int interest = 0;
	int desirableSum = 0;
	int years = 0;
	if (getInteger("Enter deposit amount: ", deposit)) {
		if (getInteger("Enter interest rate in annual percentage: ", interest)) {
			if (getInteger("Enter the desired amount: ", desirableSum)) {

			}
		}
	}

	std::cout << "\nTask 7. .\n";
	
		
	std::cout << "\nTask 8. .\n";
	
	
	return 0;
}