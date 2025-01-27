#include <iostream>
#include <vector>

const int LETTERS = 26;

char encrypt_letter(const char & c, const int shift) {
    char firstLetter = 'A';
    if (c >= 'A' && c <= 'Z') {
    }
    else if (c >= 'a' && c <= 'z') {
        firstLetter = 'a';
    }
    else {
        return c;
    }
    return (char) (firstLetter + (c - firstLetter + shift) % LETTERS);
}

std::string encrypt_caesar(const std::string & text, int shift) {
    std::string out = "";
    for (const auto & c : text) {
        out += encrypt_letter(c, shift);
    }
    return out;
}

int main () {
    std::cout << std::boolalpha;
    std::cout << (int) 'a' << " "  << (int) 'A' << "\n";
    std::cout << (int) 'z' << " "  << (int) 'Z' << "\n";

    std::vector<std::pair<std::string, int>> texts = { { "aBxZ*", 67 },
        { "The quick brown fox jumps over the lazy dog", 3 } };

    for (const auto & p : texts) {
        std::cout << encrypt_caesar(p.first, p.second) << "\n";
    }

    return 0;
}