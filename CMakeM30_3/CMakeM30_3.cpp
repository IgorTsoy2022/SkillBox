#include <iostream>
#include <sstream>
#include <unordered_map>
#include <cpr/cpr.h>

template<typename C>
void print(const C& container) {
    for (const auto& [key, value] : container) {
        std::cout << "[" << key << "] = " << value << std::endl;
    }
}

template<typename C>
std::string makeURLRequest(const C& container) {
    int i = 1;
    int size = container.size();
    std::string result = "?";
    for (const auto& [key, value] : container) {
        result += key + "=";
        result += value;
        if (i++ == size) {
            break;
        }
        result += "&";
    }
    return result;
}

template<typename C>
cpr::Payload makeArray(const C& container) {
    cpr::Payload result{};
    for (const auto& [key, value] : container) {
        result.Add(cpr::Pair(std::string(key), std::string(value)));
    }
    return result;
}

int main() {
	std::string input = "";
    std::string value = "";
    std::unordered_map<std::string, std::string> args;

    std::cout << "Enter arguments : key / value\n";
    std::cout << "or enter one of the following commands:\n";
    std::cout << "\"get\"   - Get request.\n";
    std::cout << "\"post\"  - Post arguments.\n";
    std::cout << "\"args\"  - Show arguments.\n";
    std::cout << "\"exit\"  - Exit.\n";
    while (true) {
        std::cout << "Command > ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }
        else if (input == "get") {
            std::string request = "http://httpbin.org/get" + makeURLRequest(args);
            cpr::Response r = cpr::Get(cpr::Url(request));
            std::cout << "URL request: " << request << std::endl;
            std::cout << "Respond:" << std::endl;
            std::cout << r.text << std::endl;
            continue;
        }
        else if (input == "post") {
            cpr::Response r = cpr::Post(cpr::Url("http:/httpbin.org/post"),
                                        cpr::Payload(makeArray(args)));
            std::cout << "POST request" << std::endl;
            std::cout << "Respond:" << std::endl;
            std::cout << r.text << std::endl;
            continue;
        }
        else if (input == "args") {
            print(args);
            continue;
        }
        else if (input == "") {
            continue;
        }

        while (true) {
            std::cout << "Enter the value for [" << input << "] = ";
            std::getline(std::cin, value);
            if (value.size() > 0) {
                break;
            }
        }
        args[input] = value;
    }

	return 0;
}
