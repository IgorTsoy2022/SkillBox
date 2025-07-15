#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <map>
#include <regex>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
//#include <boost/regex.hpp>


static bool isCharacter(const char& c) {
    if (c >= '0' && c <= '9') return true;
    if (c >= 'A' && c <= 'Z') return true;
    if (c >= 'a' && c <= 'z') return true;
    for (const char& symbol : "#$.@_") {
        if (c == symbol) return true;
    }
    return false;
}

static bool isOperator(const char& c) {
    for (const char& symbol : "*+-/^") {
        if (c == symbol) return true;
    }
    return false;
}

static bool isUnary(const char& c) {
    return (c == '!') || (c == '~');
    // || (c == '�');
}

static bool isNumber(const std::string& word) {
    const std::regex pattern(R"(^^\s*[-+]?((\d+(\.\d+)?)|(\d+\.)|(\.\d+))([eE][-+]?\d+)?\s*$)");
    return std::regex_match(word, pattern);
}

static bool IsFunction(std::string& word) {
    std::transform(word.begin(), word.end(), word.begin(), ::toupper);
    if (word == "ABS") return true;
    if (word == "MIN") return true;
    if (word == "MAX") return true;
    if (word == "MOD") return true;
    if (word == "ROUND") return true;
    if (word == "EXP") return true;
    if (word == "SQRT") return true;
    if (word == "LOG") return true;
    if (word == "LN") return true;
    if (word == "SIN") return true;
    if (word == "COS") return true;
    if (word == "TAN") return true;
    if (word == "ASIN") return true;
    if (word == "ACOS") return true;
    if (word == "ATAN") return true;
}

std::unordered_map<std::string, int> precedence;
void init_precedence() {
    precedence["="] = 1;
    precedence["!="] = 1;
    precedence["<>"] = 1;
    precedence["||"] = 2;
    precedence["&&"] = 3;
    precedence["+"] = 4;
    precedence["-"] = 4;
    precedence["`"] = 4;
    precedence["*"] = 5;
    precedence["/"] = 5;
    precedence["^"] = 6;
    precedence["!"] = 7;
    precedence["~"] = 7;
}

double applyOp(double a, double b, char op) {
    switch (op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;
    case '^':
        return std::pow(a, b);
    default:
        return 0;
    }
}

double arithmetic_operations(const double& op1,
    const double& op2,
    const char& operation) {
    double result = 0.0;
    switch (operation) {
    case ('+'): result = op1 + op2;
        break;
    case ('-'): result = op1 - op2;
        break;
    case ('*'): result = op1 * op2;
        break;
    case ('/'): result = op1 / op2;
        break;
    }
    return result;
}

std::string remove_unary_pluses(std::string& expression) {
//    const std::regex pattern(R"(^(\+)+)");
//    const std::regex pattern(R"(^\+|(?<=[(*\/+^-])(\+)|\+$)");
//    const std::regex pattern(R"((?<=^|[*/+^-])(\+))");
//    return std::regex_replace(expression, pattern, "");
    auto size = expression.size();
    size_t pos = 0;
    char previous = 0;
    char current = 0;
    char next = 0;
    std::string result = "";

    while (pos < size) {
        current = expression[pos];
        next = (pos < size - 1) ? expression[pos + 1] : 0;
        if (current == ' ') {
            ++pos;
            continue;
        }

        if (current == '+' && 
            (previous == 0 || previous == '(' || isOperator(previous) ||
                isOperator(next))) {
            previous = current;
            ++pos;
            continue;
        }

        result += current;
        previous = current;
        ++pos;
    }

    std::cout << result << std::endl;

    return result;
}

std::string replace_unary_minuses(std::string& expression) {
    auto size = expression.size();
    size_t pos = 0;
    char previous = 0;
    char current = 0;
    char next = 0;
    bool is_minus = false;
    std::string result = "";

    while (pos < size) {
        current = expression[pos];

        if (current == '-') {
            while(true) {
                is_minus = !is_minus;
                
                next = (pos < size - 1) ? expression[pos + 1] : 0;
                if (next == 0 || next != '-') {
                    break;
                }
                ++pos;
            }

            if (is_minus) {
                if (previous == 0 || previous == '(') {
                    current = '`';
                }
                else if (previous == '^') {
                    current = '~';
                }
                else {
                    current = '-';
                }
                is_minus = false;
            }
            else {
                if (previous == 0 || previous == '(' || previous == '^') {
                	previous = current;
                	++pos;
                	continue;
                }
                else {
                    current = '+';
                }
            }
        }

        result += current;
        previous = current;
        ++pos;
    }

    std::cout << result << std::endl;

    return result;
}



std::string to_postfix(std::string& expression) {
    std::string result = "";
    std::stack<std::string> operators;
    auto size = expression.size();
    size_t pos = 0;
    char previous = 0;
    char current = 0;

    while (pos < size) {
        current = expression[pos];
        if (current == ' ') {
            ++pos;
            continue;
        }
        if (current == '+') {
            std::cout << "'+' previous=" << previous << " current=" << current << std::endl;

            while (pos < size - 1) {
                if (!isOperator(expression[pos + 1])) {
                    break;
                }
                previous = current;
                current = expression[++pos];
            }
            if (previous == 0 || previous == '(' || isOperator(previous)) {
                previous = current;
                ++pos;
                continue;
            }

            while (operators.size() > 0 && 
                   precedence[operators.top()] >= precedence[std::to_string(current)]) {
                result += operators.top();
                operators.pop();
            }
            std::cout << "push+\n";
            operators.push("+");
        }
        else if (current == '-') {
            std::cout << "'-' previous=" << previous << " current=" << current << std::endl;
            if (previous == 0 || previous == '(' || isOperator(previous)) {
                previous = (previous == '-') ? '+' : '-';
                ++pos;
                continue;
            }
        }

        previous = current;
        ++pos;
    }
    std::cout << result << std::endl;

    return result;
}

int main() {


//    isOperator('_');

    std::string num = "012345678901234567890123456789012345678901234567890123456789.";
    std::cout << std::boolalpha << isNumber(num) << " " << std::stold(num) << std::endl;

    double x;
    std::stringstream(num) >> x;
//    std::cout << x << std::endl;



    init_precedence();
    for (const auto& [key, value] : precedence) {
//        std::cout << "[" << key << "] = " << value << std::endl;
    }

    std::string expr = " + +++- +++ -++- 566++--++ --+2 / ( 55 -+- -45 -+3+9) *---2^---2+---";
//    auto res = to_postfix(expr);
    auto res = remove_unary_pluses(expr);
    replace_unary_minuses(res);

    return 0;
}