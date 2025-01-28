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

bool isDigitAlpha(const char & c) {
    return ((c >= '0' && c <= '9')
             || (c >= 'A' && c <= 'Z')
             || (c >= 'a' && c <= 'z'));
}

bool isSpecChar(const char & c) {
    if (c == '!') {
        //   !    33
        return true;
    }
    if (c >= '#' && c <= '\'') {
        //   #$%&''  35 - 39
        return true;
    }
    if (c >= '^' && c <= '`') {
        //   ^_`      94 - 96
        return true;
    }
    if (c >= '{' && c <= '~') {
        //   {|}~      123 - 126
        return true;
    }
    return false;
}

std::string symb = "!#$%&'*+-/=?^_`{|}~";

int main () {
    std::cout << std::boolalpha;
 
    std::vector<std::pair<std::string, int>> texts = { { "aBxZ*", 67 },
        { "The quick brown fox jumps over the lazy dog", 3 } };

    for (const auto & p : texts) {
        std::cout << encrypt_caesar(p.first, p.second) << "\n";
    }

   
    std::cout << (int) '0' << " - " << (int) '9' << "\n";
    std::cout << (int) 'A' << " - "  << (int) 'Z' << "\n";
    std::cout << (int) 'a' << " - "  << (int) 'z' << "\n";
    std::cout << "\"-\" = " << (int) '-' << "\".\" = " << (int) '.' << "\n";

    for (const auto & c : symb) {
        std::cout << c << " = " << (int) c << "\n";
    }

    char c;
    do {
    	std::cin >> c;
    	std::cout << isDigitAlpha(c) << "\n";
    } while (c != '\'');


    return 0;
}