#include <iostream>
#include <fstream>
#include <sstream>

// Task 3. Payroll.
static std::pair<std::string, double>
highest_payout(const std::string & filename, double & total, int & status) {
    std::pair<std::string, double> result = { "", 0 };
    std::ifstream fs;
    fs.open(filename, std::ios::binary);
    if (fs.is_open()) {
        status = -1;
        std::string buffer = "";
        std::string first_name = "";
        std::string surname = "";
        double wage = 0;
        std::string date = "";
        while (std::getline(fs, buffer)) {
            std::stringstream buffer_stream(buffer);
            buffer_stream >> first_name >> surname >> wage >> date;
            total += wage;
            if (wage > result.second) {
                status = 1;
                result.second = wage;
                result.first = first_name + " " + surname;
            }
        }
    }
    else {
        status = 0;
    }
    fs.close();

    return result;
}

int main() {
    std::cout << "Task 3. Payroll.\n";

    std::string filename = "Payroll.txt";
    std::cout << "The payroll filename to find the largest payout\n";
    std::cout << "and the total payout: \"" << filename << "\"\n";

    int status = 0;
    double total = 0;
    auto result = highest_payout(filename, total, status);

    if (status > 0) {
        std::cout << "The total amount of payments on the payroll was $" << total << ".\n";
        std::cout << "The largest payout of $" << result.second
                  << " was received by " << result.first << ".\n";
    }
    else if (status == -1) {
        std::cout << "Incorrect data format!\n";
    }
    else {
        std::cout << "File not found!\n";
    }

    return 0;
}