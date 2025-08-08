#include <iostream>
#include <fstream>
#include <string>
#include <vector>

static void toupper(std::string& text) {
    for (auto& c : text) {
        if (c >= 'a' && c <= 'z') {
            c = c - 32;
        }
    }
}

static bool text_view(const std::string & filename) {
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

static bool is_number(const std::string& str) {
    for (const auto& c : str) {
        if (c >= '0' && c <= '9') {
            continue;
        }
        return false;
    }
    return true;
}

static int revolve(std::vector<bool> & drum, int shift) {
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

static void game_experts() {
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
    std::cout << "Task 2. What, where, when?\n";

    game_experts();

    return 0;
}