#include <iostream>
#include <sstream>
#include <cpr/cpr.h>

int main() {
    std::string input = "";
    std::string name = "";
    std::string city = "";
    std::string phone = "";

    std::cout << "Commands:\n";
    std::cout << "\"get\"\n";
    std::cout << "\"post\"\n";
    std::cout << "\"put\"\n";
    std::cout << "\"patch\"\n";
    std::cout << "\"delete\"\n";
    std::cout << "\"exit\"  - Exit.\n";
    while (true) {
        std::cout << "Command > ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }
        else if (input == "get") {
            cpr::Response r = cpr::Get(cpr::Url("http:/httpbin.org/get"));
            std::cout << r.text << std::endl;
        }
        else if (input == "post") {
            std::cout << "Name: ";
            std::getline(std::cin, name);
            std::cout << "City: ";
            std::getline(std::cin, city);
            std::cout << "Phone: ";
            std::getline(std::cin, phone);
            cpr::Response r = cpr::Post(cpr::Url("http:/httpbin.org/post"),
                              cpr::Payload({ {"name", name.c_str()}, {"city", city.c_str()},
                                             {"phone", phone.c_str()} }));
            std::cout << r.text << std::endl;

        }
        else if (input == "put") {
            std::cout << "Name: ";
            std::getline(std::cin, name);
            std::cout << "City: ";
            std::getline(std::cin, city);
            std::cout << "Phone: ";
            std::getline(std::cin, phone);

            cpr::Response r = cpr::Put(cpr::Url("http:/httpbin.org/put"),
                              cpr::Payload({ {"name", name.c_str()}, {"city", city.c_str()},
                                             {"phone", phone.c_str()} }));
            std::cout << r.text << std::endl;
        }
        else if (input == "patch") {
            std::cout << "Name: ";
            std::getline(std::cin, name);
            std::cout << "City: ";
            std::getline(std::cin, city);
            std::cout << "Phone: ";
            std::getline(std::cin, phone);

            cpr::Response r = cpr::Patch(cpr::Url("http:/httpbin.org/patch"),
                              cpr::Payload({ {"name", name.c_str()}, {"city", city.c_str()},
                                             {"phone", phone.c_str()} }));
            std::cout << r.text << std::endl;
        }
        else if (input == "delete") {
            cpr::Response r = cpr::Delete(cpr::Url("http:/httpbin.org/delete"));
            std::cout << r.text << std::endl;
        }
    }

	return 0;
}
