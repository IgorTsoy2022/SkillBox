#include <iostream>
#include <string>
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
    //   !#$%&'*+-./=?^_`{|}~
    switch (c) {
    case '!' :   // 33
    case '*' :   // 42
    case '+' :   // 43
    case '=' :   // 61
    case '?' :   // 63
        return true;
    default :
        if (c >= '#' && c <= '\'') {
            //   #, $, %, &, '    = 35 - 39
            return true;
        }
        if (c >= '-' && c <= '/') {
            //   -, ., /          = 45 - 47
            return true;
        }
        if (c >= '^' && c <= '`') {
            //   ^, _, `          = 94 - 96
            return true;
        }
        if (c >= '{' && c <= '~') {
            //   {, |, }, ~       = 123 - 126
            return true;
        }
        break;
    }
    return false;
}

bool is_address(std::string text) {
    bool atDetected = false;
    char prev_c = '0';
    int name_size = 0;
    int domen_size = 0;
    std::string out = "";
    for (const char & c : text) {
        if (atDetected) {
            if (c == '@') {
                return false;
            }
            if (domen_size > 0) {
                if (domen_size > 62) {
                    return false;
                }
            }
            else {
                if (prev_c == '.' || c == '.') {
                    return false;
                }
            }
            if (!isDigitAlpha(c) && c != '-' && c != '.') {
                return false;
            }
            ++domen_size;
        }
        else {
            if (c == '@') {
                if (name_size < 1) {
                    return false;
                }
                atDetected = true;
                continue;
            }
            if (name_size > 0) {
                if (name_size > 63) {
                    return false;
                }
            }
            else {
                if (c == '.') {
                    return false;
                }
            }
            if (!isDigitAlpha(c) && !isSpecChar(c)) {
                return false;
            }
            ++name_size;
        }
        if (prev_c == '.' && c == '.') {
            return false;
        }
        prev_c = c;
        out += c;
    }

    if (domen_size < 1 || prev_c == '.') {
        return false;
    }

    return true;
}

//std::string symb = "!#$%&'*+-/=?^_`{|}~";

std::vector<std::string> split(const std::string & sentence, const char delimeter = ' ') {
	std::vector<std::string> result;
	std::string word = "";
	for (const char & c : sentence) {
	    if (c == delimeter) {
	        if (word.size() > 0) {
	            result.push_back(word);
	            word.clear();
	        }
	    }
	    else {
	        word += c;
	    }
	}
	if (word.size() > 0) {
	    result.push_back(word);
	}
	return result;
}

std::string get_address_part(const std::string & address, int pos) {
    bool isWord = false;
    int octette_number = 0;
    int start = 0;
    int count = 0;

    for (int i = 0; i < address.size(); ++i) {
        if (address[i] == '.') {
            isWord = false;
            if (octette_number == pos) {
                return address.substr(start, count);
            }
            ++octette_number;
            start = i + 1;
	        count = 0;
        }
        else {
            isWord = true;
            if (octette_number == pos) {
                ++count;
            }
        }
    }
    if (isWord) {
        if (octette_number == pos) {
            return address.substr(start, count);
        }
    }
    return "";
}

bool isValidNumber(const std::string & input) {
    auto size = input.size();
    if (size < 1) {
        return false;
    }
    int number = 0;
    for (const char & c : input) {
        if (c < '0' || c > '9') {
            return false;
        }
        number *= 10;
        if (c == '0') {
            if (number < 1 && size > 1) {
                return false;
            }
        }
        else {
            number += c - '0';
        }
    }
    return number < 256;
}

bool isValidIP(const std::string & text) {
	auto size = text.size();
	if (size < 7) {
	    return false;
	}

    int start = 0;
    int count = 0;
    int octettes = 0;
	for (int i = 0; i < size; ++i) {
	    if (text[i] == '.') {
	        if (count < 1) {
	            return false;
	        }
	        if (!isValidNumber(text.substr(start, count))) {
	            return false;
	        }
	        ++octettes;
	        start = i + 1;
	        count = 0;
	    }
	    else {
	        ++count;
	    }
	}
	if (count > 0) {
	    ++octettes;
	    if (!isValidNumber(text.substr(start, count))) {
	        return false;
        }
        return octettes == 4;
	}
    else {
        return false;
    }
}

int main () {
    std::cout << std::boolalpha;
 
    std::cout << "Task 1. The Caesar Cipher.\n";
    std::vector<std::pair<std::string, int>> texts = { 
        { "aBxZ*", 67 },
        { "The quick brown fox jumps over the lazy dog", 3 }
    };

    for (const auto & p : texts) {
        std::cout << encrypt_caesar(p.first, p.second) << "\n";
    }

    int code = 0;
    std::string sentence = "";
    std::cout << "Enter a sentence: ";
    std::getline(std::cin, sentence);
    std::cout << "Code: ";
    std::cin >> code;
    std::cout << "Result: " << encrypt_caesar(sentence, code) << "\n";

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail(), '\n');

    std::cout << "\nTask 2. Checking email-address.\n";
    std::vector<std::string> addresses = {
        "simple@example.com",
        "very.common@example.com",
        "disposable.style.email.with+symbol@example.com",
        "other.email-with-hyphen@example.com",
        "fully-qualified-domain@example.com",
        "user.name+tag+sorting@example.com",
        "x@example.com",
        "example-indeed@strange-example.com",
        "admin@mailserver1",
        "example@s.example",
        "mailhost!username@example.org",
        "user%example.com@example.org",
        "is_this?@correct",
        "is_this?@correct?",
        "John..Doe@example.com",
        "Abc.example.com",
        "A@b@c@example.com",
        "ab\"(c)d,e:f;g<h>i[j\\k]l@example.com",
        "1234567890123456789012345678901234567890123456789012345678901234+x@example.com",
        "i_like_underscore@but_its_not_allow_in _this_part.example.com"
    };

    for (const std::string & text : addresses) {
        std::cout << text << " " << is_address(text) << "\n";
    }
    std::string e_mail = "";
    std::cout << "Enter an e-mail address: ";
    std::cin >> e_mail;
    std::cout << "e-mail address: " << e_mail << " is " << (is_address(e_mail) ? "" : "not ") << "correct.\n";

    std::cout << "\nTask 3. IP Address Validation.\n";

    std::vector<std::string> ips = {
        "127.0.0.1", "255.255.255.255", "1.2.3.4", "55.77.213.101",
        "255.256.257.258", "0.55.33.22.", "10.00.000.0", "23.055.255.033", "65.123..9", "a.b.c.d"
    };

    for (const std::string & ip : ips) {
        std::cout << ip << " is " << (isValidIP(ip) ? "valid." : "invalid.") << "\n";
    }
    std::string ip = "";
    std::cout << "Enter an ip address: ";
    std::cin >> ip;
    std::cout << "ip address: " << ip << " is " << (isValidIP(ip) ? "valid." : "invalid.") << "\n";
    std::cout << "get_address_part(\"" << ip << "\", 0) = \"" << get_address_part(ip, 0) << "\"\n";
    std::cout << "get_address_part(\"" << ip << "\", 1) = \"" << get_address_part(ip, 1) << "\"\n";
    std::cout << "get_address_part(\"" << ip << "\", 2) = \"" << get_address_part(ip, 2) << "\"\n";
    std::cout << "get_address_part(\"" << ip << "\", 3) = \"" << get_address_part(ip, 3) << "\"\n";
    std::cout << "get_address_part(\"" << ip << "\", 4) = \"" << get_address_part(ip, 4) << "\"\n";

    return 0;
}