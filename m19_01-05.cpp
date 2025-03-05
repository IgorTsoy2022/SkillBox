#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

template<typename A>
void print(const A & arr, size_t from, size_t to) {
    std::cout << "[";
    if (from > to) {
        for (size_t i = from; i > to; --i) {
            std::cout << arr[i] << " ";
        }
    }
    else {
        for (size_t i = from; i < to; ++i) {
	        std::cout << arr[i] << " ";
	    }
    }
    std::cout << arr[to] << "]\n";
}

void toupper(std::string& text) {
    for (auto& c : text) {
        if (c >= 'a' && c <= 'z') {
            c = c - 32;
        }
    }
}

void tolower(std::string& text) {
    for (auto& c : text) {
        if (c >= 'A' && c <= 'Z') {
            c = c + 32;
        }
    }
}

// Task 1. Search word

int search_in_file(const std::string& filename, std::string& search_word) {
    std::fstream fs;
    fs.open(filename);

    int size = search_word.size();
    int count = 0;
    toupper(search_word);
    std::string word = "";
    if (fs.is_open()) {
        while (fs >> word) {
            if (word.size() >= size) {
                toupper(word);
                if (word.substr(0, size) == search_word) {
                    ++count;
                }
            }
        }
    }
    else {
        std::cout << "File " << filename << " not found.\n";
    }

    fs.close();

    return count;
}

// Task 2. Text viewer.

bool text_view(const std::string & filename) {
    bool isOk = false;
    std::ifstream fs;
    fs.open(filename, std::ios::binary);
    if (fs.is_open()) {
        char buffer [33];
        while (!fs.eof()) {
            fs.read(buffer, 32);
            buffer[fs.gcount()] = 0;
            std::cout << buffer;
        }
        isOk = true;
    }
    fs.close();
    return isOk;
}

// Task 3. Payroll.
std::pair<std::string, double> highest_payout(const std::string & filename, double & total, int & status) {
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

// Task 4. PNG file detector.

template<typename T>
void swap(T & a, T & b) {
    T tmp = a;
    a = b;
    b = tmp;
}

template<typename T>
void reverse(T & arr, size_t size) {
    for (size_t i = 0; i < size / 2; ++i) {
        swap(arr[i], arr[size - 1 - i]);
    }
}

std::string get_extension(const std::string & filename) {
    bool has_extension = false;
    int size = filename.size();
    int start = size - 1;
    while (start >= 0) {
        has_extension = (filename[start] == '.');
        if (has_extension) {
            break;
        }
        --start;
    }

    if (has_extension) {
        return filename.substr(start + 1, size - start - 1);
    }

    return "";
}

template<typename T1, typename T2>
bool operator==(const T1 & lhs, const T2 & rhs) {
    for (size_t i = 0; lhs[i] != '\0'; ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

void assign(char* lhs, const char* rhs) {
    size_t index = 0;
    for (; rhs[index] != '\0'; ++index) {
        *(lhs + index) = *(rhs + index);
    }
    *(lhs + index) = 0;
}

void check_png_file(const std::string & filename) {
    auto size = filename.size();
    bool extension_is_png = false;
    if (size > 4) {
        std::string extension = get_extension(filename);
        tolower(extension);
        extension_is_png =  (extension == "png");
    }

    std::ifstream fs;
    fs.open(filename, std::ios::binary);
    if (fs.is_open()) {
        if (extension_is_png) {
            std::cout << "The extension of the file is \"png\"\n";
        }
        else {
            std::cout << "The extension of the file is not \"png\"\n";
        }

        char buffer[4];	
        std::string comp = "0PNG";
        comp[0] = -119;

        fs.read(buffer, 4);
        if (comp == buffer) {
            std::cout << "First 4 bytes of the file indicate that it is probably a png-file.\n";
        }
    }
    else {
        std::cout << "File not found!\n";
     }
    fs.close();
}

// Task 5. What, where, when.

bool is_number(const std::string& str) {
    for (const auto& c : str) {
        if (c >= '0' && c <= '9') {
            continue;
        }
        return false;
    }
    return true;
}

int revolve(std::vector<bool> & drum, int shift) {
    static int current;
    auto size = drum.size();
    if (size < 1) {
        return -1;
    }
    current += shift;
    if (current > size - 1) {
        current %= size;
    }
    
    for (int i = 0; i < size; ++i, ++current) {
        if (current > size - 1) {
            current -= size;
        }

        if (drum[current]) {
            drum[current] = false;
            return current;
        }

    }
    return -1;
}

void game_experts() {
    const int sectors = 13;
    int experts = 0;
    int TVviewers = 0;

    std::vector<bool> drum(sectors, true);
    std::vector<std::string> questions = {
        "q01.txt", "q02.txt", "q03.txt",
        "q04.txt", "q05.txt", "q06.txt",
        "q07.txt", "q08.txt", "q09.txt",
        "q10.txt", "q11.txt", "q12.txt",
        "q13.txt"
    };
    std::vector<std::string> answers = {
        "a01.txt", "a02.txt", "a03.txt",
        "a04.txt", "a05.txt", "a06.txt",
        "a07.txt", "a08.txt", "a09.txt",
        "a10.txt", "a11.txt", "a12.txt",
        "a13.txt"
    };

    int index = 0;
    int winscore = sectors / 2;
    while (experts < winscore && TVviewers < winscore) {
        bool exit = false;
        std::cout << "\nCurrent sector is " << index + 1
                  << ". Spin the drum to get a question\n"
                  << "by entering a number (type \"exit\" for exit): ";
        int offset = 0;
        std::string input = "";
        while (std::getline(std::cin, input)) {
            if (input == "exit") {
                exit = true;
                break;
            }
            if (is_number(input)) {
                offset = std::stoi(input);
                break;
            }
            std::cout << "Incorrect number. Try again: ";
        }

        if (exit) {
            std::cout << "Game over!\n";
            break;
        }

        index = revolve(drum, offset);
        std::cout << "Current sector is " << index + 1
                  << ".\nAttention! The question:\n";
        
        if (text_view(questions[index])) {
            std::cout << "\n";
            std::string experts_answer = "";
            std::cout << "Type your answer (one word): ";
            std::getline(std::cin, experts_answer);
            toupper(experts_answer);

            std::string right_answer = "";
            std::fstream fs;
            fs.open(answers[index]);
            if (fs.is_open()) {
                if (std::getline(fs, right_answer)) {
                    toupper(right_answer);
                    if (experts_answer == right_answer) {
                        ++experts;
                        std::cout << "That's right! Current round won experts!\n";
                    }
                    else {
                        ++TVviewers;
                        std::cout << "You're wrong! The right answer is "
                                  << right_answer << "\n";
                        std::cout << "Current round won TV viewrs!\n";
                    }

                    std::cout << "Current score:\n";
                    std::cout << "Experts: " << experts << "\n";
                    std::cout << "TV viewers: " << TVviewers << "\n";
                }
                else {
           std::cout << "File with no answer!\n";
                }
            }
            else {
                std::cout << "File with answer not found!\n";
            }
        }
        else {
            std::cout << "File with question not found!\n";
        }
    }
    if (experts > TVviewers) {
        std::cout << "The experts won the game!";
    }
    else {
        std::cout << "The TV viewers won the game!\n";
    }
}

int main() {
    std::cout << "Task 1. Search word.\n";
    {
        std::string filename = "Hello darkness.txt";
        std::string word = "silence";
        
        std::cout << "Enter the filename to read: ";
        std::getline(std::cin, filename);
        std::cin.clear();

        bool filefound = false;
        std::fstream fs;
        fs.open(filename);
        filefound = fs.is_open();
        if (filefound) {
            std::cout << "File content:\n";
            std::string buffer = "";
            while (std::getline(fs, buffer)) {
                std::cout << buffer << "\n";
            }
        }
        fs.close();

        if (filefound) {
            std::cout << "\nFile: \"" << filename << "\".\n";
            std::cout << "Enter a word for search (\"exit\" for exit): ";
            while (std::getline(std::cin, word)) {
                auto word_count = search_in_file(filename, word);

                std::cout << "The word \"" << word << "\" was ";
                if (word_count > 0) {
                    std::cout << "found " << word_count << " times.\n";
                }
                else {
                    std::cout << "not found.\n";
                }

                if (word == "EXIT") {
                    break;
                }

                std::cout << "\nEnter word for search (\"exit\" for exit): ";
            }
        }
        else {
            std::cout << "\nFile: \"" << filename << "\" not found!\n";
        }
    }

    std::cout << "\nTask 2. Text viewer.\n";
    {
        std::string filename = "Hello darkness.txt";
        std::cout << "Enter the filename to read: ";
        std::getline(std::cin, filename);
        std::cin.clear();

        if (!text_view(filename)) {
            std::cout << "File not found!\n";
        }
    }

    std::cout << "\nTask 3. Payroll.\n";
    {
        std::string filename = "Payroll.txt";
        std::cout << "Enter the payroll filename to find the largest payout\n";
        std::cout << "and the total payout: ";
        std::getline(std::cin, filename);
        std::cin.clear();
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
    }

    std::cout << "\nTask 4. PNG file detector.\n";
    {
        std::string filename = "money.png";

/*
        std::ofstream fs;
        fs.open(filename, std::ios::binary);
        char buffer[5];

        if (fs.is_open()) {
            assign(buffer, "0PNG");
            buffer[0] = -119;
            fs.write(buffer, 4);
        }
        fs.close();
*/

        std::cout << "Enter the filename to check if it is a png file: ";
        std::getline(std::cin, filename);
        std::cin.clear();
        check_png_file(filename);
    }

    std::cout << "\nTask 5.  What, where, when.\n";

    game_experts();
    
    return 0;
}