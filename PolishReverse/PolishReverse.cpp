#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <exception>
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
    for (const char& symbol : "+-*/`~^!|&=><%") {
        if (c == symbol) return true;
    }
    return false;
}

static bool isOperator(const std::string& word) {
//    std::cout << "stringver " << word << std::endl;
    const std::regex pattern(R"(^(?:\+|-|\*|/|`|~|\^|!|\||\|\||&|&&|=|!=|\<\>|\>|\<|\>=|\<=|%)$)");
    return std::regex_match(word, pattern);
}

static bool isUnary(const char& c) {
    return (c == '^') || (c == '!') || (c == '~') || (c == '`');
}

static bool isRightAssociative(const char& c) {
    return (c == '^') || (c == '~') || (c == '!');
}

static bool isNumber(const std::string& word) {
    const std::regex pattern(R"(^^\s*[-+]?((\d+(\.\d+)?)|(\d+\.)|(\.\d+))([eE][-+]?\d+)?\s*$)");
    return std::regex_match(word, pattern);
}

static bool isFunction(std::string& word) {
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
        if (current == ' ') {
            ++pos;
            continue;
        }

        size_t pos_next = pos + 1;
        next = 0;
        while (pos_next < size) {
            next = expression[pos_next++];
            if (next != ' ') {
                break;
            }
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
                if (previous == 0 || previous == '(' || previous == ',') {
                    current = '`';
                }
                else if (previous == '+' || previous == '*' || previous == '/' ||
                         previous == '!' || previous == '^') {
                    current = '~';
                }
                else {
                    current = '-';
                }
                is_minus = false;
            }
            else {
                if (previous == 0 || previous == '(' || previous == ',' ||
                    previous == '+' || previous == '*' || previous == '/' ||
                    previous == '!' || previous == '^') {
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

    return result;
}



// Dijkstra's algorithm
std::string to_postfix(std::string& expression) {
    std::string result = "";
    std::stack<std::string> actions;

    expression = remove_unary_pluses(expression);
    std::cout << expression << std::endl;
    expression = replace_unary_minuses(expression);
    std::cout << expression << std::endl;

    auto size = expression.size();
    size_t pos = 0;
    char current = 0;

    while (pos < size) {
        current = expression[pos];
        std::cout << "current=" << current << std::endl;
        if (isCharacter(current)) {
            std::string word(1, current);
            ++pos;
            while (pos < size) {
                current = expression[pos];
                if (!isCharacter(current)) {
                    --pos;
                    break;
                }
                word += current;
                ++pos;
            }

            if (isNumber(word)) {
                result += word;
                std::cout << "    result = [" << result << "]\n";
            }
            else if (isFunction(word)) {
                actions.push(word);
            }
            else {
                throw std::invalid_argument(
                    "Incorrect expression. Unknown word: \"" + word + "\"");
            }
        }
        else if (isOperator(current)) {
            char next = (pos < size - 1) ? expression[pos + 1] : 0;
            std::string action(1, current);
            if (isOperator(action + next)) {
                action += next;
            }
            std::cout << "action = " << action << std::endl;

            if (!isRightAssociative(current)) {
                while (actions.size() > 0 && 
                       precedence[actions.top()] >= precedence[action]) {
                    result += actions.top();
                    actions.pop();
                }
            }
            else {
                while (actions.size() > 0 &&
                    precedence[actions.top()] > precedence[action]) {
                    result += actions.top();
                    actions.pop();
                }
                actions.push(action);
            }
        }
        else if (current == '(') {
            if (actions.size() > 0) {
                if (isFunction(actions.top())) {
                    result += "'";
                }
            }
            actions.push("(");
        }
        else if (current == ')') {
            if (actions.size() > 0) {
                while (actions.top() != "(") {
                    result += actions.top();
                    actions.pop();
                }
                actions.pop();
            }
            if (actions.size() > 0) {
                if (isFunction(actions.top())) {
                    result += actions.top();
                }
            }
        }
        else if (current == ',') {
            while (actions.size() > 0 && actions.top() != "(") {
                result += actions.top();
                actions.pop();
            }
        }
        else {
            std::cout << "!\n";
            throw std::invalid_argument(
                "Incorrect expression. Unknown symbol: '" + 
                std::string(1, current) + "'");
        }

        ++pos;
    }

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

    std::string expr = "-2 + +++- (--3, -+--+8)+++ -++- 566++--++ --+2 / ( 55 -+- -45 -+3+9) ---2^---2+---";
    try {
        auto res = to_postfix(expr);
        std::cout << res << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    }

    /*
    std::cout << "+ " << isOperator("+") << std::endl;
    std::cout << "- " << isOperator("-") << std::endl;
    std::cout << "* " << isOperator("*") << std::endl;
    std::cout << "/ " << isOperator("/") << std::endl;
    std::cout << "` " << isOperator("`") << std::endl;
    std::cout << "~ " << isOperator("~") << std::endl;
    std::cout << "^ " << isOperator("^") << std::endl;

    std::cout << "! " << isOperator("!") << std::endl;
    std::cout << "| " << isOperator("|") << std::endl;
    std::cout << "|| " << isOperator("||") << std::endl;
    std::cout << "& " << isOperator("&") << std::endl;
    std::cout << "&& " << isOperator("&&") << std::endl;

    std::cout << "= " << isOperator("=") << std::endl;
    std::cout << "!= " << isOperator("!=") << std::endl;
    std::cout << "<> " << isOperator("<>") << std::endl;
    std::cout << "> " << isOperator(">") << std::endl;
    std::cout << ">= " << isOperator(">=") << std::endl;
    std::cout << "< " << isOperator("<") << std::endl;
    std::cout << "<= " << isOperator("<=") << std::endl;
    std::cout << "% " << isOperator("%") << std::endl;

    std::cout << "@ " << isOperator("@") << std::endl;
    std::cout << "# " << isOperator("#") << std::endl;
    std::cout << "$ " << isOperator("$") << std::endl;
    std::cout << "( " << isOperator("(") << std::endl;
    std::cout << ") " << isOperator(")") << std::endl;
    std::cout << "_ " << isOperator("_") << std::endl;
    std::cout << "\\ " << isOperator("\\") << std::endl;
    std::cout << ": " << isOperator(":") << std::endl;
    std::cout << "; " << isOperator(";") << std::endl;
    std::cout << "? " << isOperator("?") << std::endl;
    std::cout << ". " << isOperator(".") << std::endl;
    std::cout << ", " << isOperator(",") << std::endl;
    std::cout << "** " << isOperator("**") << std::endl;

    std::cout << std::endl;
    std::cout << "+ " << isOperator('+') << std::endl;
    std::cout << "- " << isOperator('-') << std::endl;
    std::cout << "* " << isOperator('*') << std::endl;
    std::cout << "/ " << isOperator('/') << std::endl;
    std::cout << "` " << isOperator('`') << std::endl;
    std::cout << "~ " << isOperator('~') << std::endl;
    std::cout << "^ " << isOperator('^') << std::endl;

    std::cout << "! " << isOperator('!') << std::endl;
    std::cout << "| " << isOperator('|') << std::endl;
    std::cout << "& " << isOperator('&') << std::endl;

    std::cout << "= " << isOperator('=') << std::endl;
    std::cout << "> " << isOperator('>') << std::endl;
    std::cout << "< " << isOperator('<') << std::endl;
    std::cout << "% " << isOperator('%') << std::endl;

    std::cout << "@ " << isOperator('@') << std::endl;
    std::cout << "# " << isOperator('#') << std::endl;
    std::cout << "$ " << isOperator('$') << std::endl;
    std::cout << "( " << isOperator('(') << std::endl;
    std::cout << ") " << isOperator(')') << std::endl;
    std::cout << "_ " << isOperator('_') << std::endl;
    std::cout << "\\ " << isOperator('\\') << std::endl;
    std::cout << ": " << isOperator(':') << std::endl;
    std::cout << "; " << isOperator(';') << std::endl;
    std::cout << "? " << isOperator('?') << std::endl;
    std::cout << ". " << isOperator('.') << std::endl;
    std::cout << ", " << isOperator(',') << std::endl;

    std::string e(1, '&');
    std::cout << "isop " << isOperator(e + '&') << std::endl;
    */

    return 0;
}