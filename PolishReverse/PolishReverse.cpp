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
        std::cout << symbol << std::endl;
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
    precedence["�"] = 4;
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
    const std::regex
    pattern(R"(^\+|(?<=[(*\/+^-])(\+)|\+$)");
//    pattern(R"((?<=^|[*/+^-])(\+))");
//    pattern(R"(^(\+)+)");
//    std::cout << 
    return std::regex_replace(expression, pattern, "");
}





std::string to_postfix(std::string& expr) {
    std::string result;
    std::stack<std::string> operators;
    auto size = expr.size();
    int pos = 0;
    char previous = 0;
    char current = 0;

    while (pos < size) {
        if (expr[pos] == ' ') {
            ++pos;
            continue;
        }
        if (expr[pos] == '+') {
            if (previous == 0 || previous == '(' || isOperator(previous)) {
                previous = expr[pos++];
                continue;
            }
            while (operators.size() > 0 && 
                   precedence[operators.top()] >= precedence[expr[pos]]) {

            }
        }
        previous = expr[pos++];
    }

    return result;
}

int main() {

    char c = 172;
    int i = c;
 //   std::cout << c << " = " << i << std::endl;

//    isOperator('_');

    std::string num = "012345678901234567890123456789012345678901234567890123456789.";
 //   std::cout << std::boolalpha << isNumber(num) << " " << std::stold(num) << std::endl;

    double x;
    std::stringstream(num) >> x;
//    std::cout << x << std::endl;



    init_precedence();

    std::string expr = "+++++566+++2";
    auto res = to_postfix(expr);
    std::cout << res << std::endl;

    return 0;
}