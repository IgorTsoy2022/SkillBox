#ifndef REVERSEPOLISHNOTATION_H
#define REVERSEPOLISHNOTATION_H

#pragma once

#define CPP20_

#include <algorithm>
#include <iostream>
#include <sstream>
#include <cmath>
#include <limits>
#include <numbers>
#include <regex>
#include <stack>
#include <string>
#include <unordered_map>

template<typename NumType>
class ReversePolishNotation {
public:
    ReversePolishNotation() = default;

    static bool isNumber(const std::string& word) {
        static const std::regex
            pattern(R"(^^\s*[-+]?((\d+(\.\d+)?)|(\.)|(\d+\.)|(\.\d+))([eE][-+]?\d+)?\s*$)");
        return std::regex_match(word, pattern);
    }

    // Dijkstra's algorithm
    static std::string to_postfix(std::string& expression) {
        std::string result;
        std::stack<std::string> operations;

        expression = remove_unary_pluses(expression);
        expression = replace_unary_minuses(expression);

        auto size = expression.size();
        size_t pos = 0;
        int brackets_count = 0;
        char current = 0;

        while (pos < size) {
            current = expression[pos];
            if (isCharacter(current)) {
                std::string word = getWord(expression, pos);

                if (isNumber(word)) {
                    result += word + " ";
                }
                else {
#ifdef CPP20_
                    std::ranges::transform(word.begin(), word.end(),
                                           word.begin(),
                    [](unsigned char c) -> unsigned char {
                       return std::toupper(c);
                    });
#else
                    std::transform(word.begin(), word.end(), word.begin(), ::toupper);
#endif
                    if (isFunction(word)) {
                        operations.push(word);
                    }
                    else if (isConstant(word)) {
                        result += word + " ";
                    }
                    else {
                        std::string message =
                            "Incorrect expression. Unknown word: \"" + word +
                            "\".\nProcessing stopped at position " +
                            std::to_string(pos + 1) + " :\n" + expression.substr(0, pos + 1);
                        throw std::invalid_argument(message);
                    }
                }
            }
            else if (isOperator(current)) {
                std::string operation = getOperator(expression, pos);

                if (operation == "%") {
                    result += "% ";
                }
                else {
                    if (!isRightAssociative(operation)) {
                        while (!operations.empty() &&
                            precedence(operations.top()) >= precedence(operation)) {
                            result += operations.top() + " ";
                            operations.pop();
                        }
                    }
                    else {
                        while (!operations.empty() &&
                            precedence(operations.top()) > precedence(operation)) {
                            result += operations.top() + " ";
                            operations.pop();
                        }
                    }
                    operations.push(operation);
                }
            }
            else if (current == '(') {
                if (!operations.empty()) {
                    if (isFunction(operations.top())) {
                        result += "' ";
                    }
                }
                operations.emplace("(");
                ++brackets_count;
            }
            else if (current == ')') {
                bool balanced_brackets = false;
                while (!operations.empty()) {
                    if (operations.top() == "(") {
                        operations.pop();
                        balanced_brackets = true;
                        break;
                    }
                    result += operations.top() + " ";
                    operations.pop();
                }

                if (!balanced_brackets) {
                    std::string message =
                        "Incorrect expression. The brackets are not balanced.\n";
                    message += "The opening bracket '(' is missing. ";
                    message += "Processing stopped at position " +
                               std::to_string(pos + 1) + " :\n" +
                               expression.substr(0, pos + 1);
                    throw std::invalid_argument(message);
                }

                if (!operations.empty()) {
                    if (isFunction(operations.top())) {
                        result += operations.top() + " ";
                        operations.pop();
                    }
                }
                --brackets_count;
            }
            else if (current == ',') {
                while (!operations.empty() && operations.top() != "(") {
                    result += operations.top() + " ";
                    operations.pop();
                }
            }
            else {
                std::string message =
                    "Incorrect expression. Unknown symbol: '";
                message += current;
                message += "'.\nProcessing stopped at position " +
                    std::to_string(pos + 1) + " :\n" +
                    expression.substr(0, pos + 1);
                throw std::invalid_argument(message);
            }

            ++pos;
        }

        if (brackets_count != 0) {
            std::string message =
                "Incorrect expression. The brackets are not balanced.\n";
            message += "The closing bracket ')' is missing. ";
            message += "Processing stopped at position " +
                std::to_string(pos) + " :\n" +
                expression.substr(0, pos + 1);
            throw std::invalid_argument(message);
        }

        while (!operations.empty()) {
            result += operations.top() + " ";
            operations.pop();
        }

        return result;
    }

    static NumType calculate(std::string& infixExpression) {
        auto postfixExpression = to_postfix(infixExpression);
        auto size = postfixExpression.size();
        size_t pos = 0;
        char current = 0;
        bool isParameter = false;
        int parameters_count = 0;
        NumType value = 0.0;

#ifdef CPP20_
        static const NumType PI_ = std::numbers::pi_v<NumType>;
        static const NumType E_ = std::numbers::e_v<NumType>;
#else
        static const NumType PI_ = 3.141592653589793238462643;
        static const NumType E_ =  2.718281828459045235360287;
#endif

        std::stack<NumType> operands;

        while (pos < size) {
            current = postfixExpression[pos];

            if (isOperator(current)) {
                std::string operation = getOperator(postfixExpression, pos);
                NumType operand1 = 0.0;
                NumType operand2 = 0.0;
                if (!operands.empty()) {
                    operand2 = operands.top();
                    operands.pop();
                    if (isParameter) {
                        --parameters_count;
                    }
                }
                else {
                    throw std::invalid_argument(
                        "Incorrect expression: no operand(s) for the \"" +
                        operation + "\" operator.");
                }

                if (!isUnary(operation)) {
                    if (!operands.empty()) {
                        operand1 = operands.top();
                        if (current != '%') {
                            operands.pop();
                            if (isParameter) {
                                --parameters_count;
                            }
                        }
                    }
                    else {
                        if (current != '%') {
                            throw std::invalid_argument(
                                "Incorrect expression: no 2-d operand for the \"" +
                                operation + "\" operator.");
                        }
                    }
                }

                value = execute(operand1, operand2, operation);
                operands.push(value);
                if (isParameter) {
                    ++parameters_count;
                }
            }
            else if (current == '\'') {
                isParameter = true;
            }
            else if (isCharacter(current)) {
                std::string word = getWord(postfixExpression, pos);
                if (isNumber(word)) {
                    std::stringstream(word) >> value;
                    operands.push(value);
                    if (isParameter) {
                        ++parameters_count;
                    }
                }
                else if (isConstant(word)) {
                    if (word == "PI") {
                        operands.push(PI_);
                    }
                    else if (word == "E") {
                        operands.push(E_);
                    }
                    if (isParameter) {
                        ++parameters_count;
                    }
                }
                else {

                    std::stack<NumType> parameters;
                    while (!operands.empty() && parameters_count-- > 0) {
                        parameters.push(operands.top());
                        operands.pop();
                    }
                    isParameter = false;

                    operands.push(callFunction(word, parameters));
                }
            }

            ++pos;
        }

        if (operands.size() != 1) {
            throw std::invalid_argument(
                "Incorrect expression: no operator.");
        }

        return operands.top();
    }

    ~ReversePolishNotation() = default;
private:
    static const std::unordered_map<std::string, int> precedence_;

    static int precedence(const std::string& operation) {
        int result = 0;
        try {
            result = precedence_.at(operation);
        }
        catch (...) {
            std::cerr << "precedence : The operator \"" << operation
                      << "\" is not supported." << std::endl;
        }
        return result;
    }

    static bool isCharacter(const char& c) {
        if (c >= '0' && c <= '9') return true;
        if (c >= 'A' && c <= 'Z') return true;
        if (c >= 'a' && c <= 'z') return true;

#ifdef CPP20_
        return std::ranges::any_of("#$.@_",
                              [c](const auto& v) { return v == c; });
#else
        static const std::string pattern{ "#$.@_" };
        return std::any_of(pattern.begin(), pattern.end(),
                           [c](const auto& v) { return v == c; });
#endif

    }

    static bool isOperator(const char& c) {
#ifdef CPP20_
        return std::ranges::any_of("+-*/`~^!|&=><%",
                              [c](const auto& v) { return v == c; });
#else
        static const std::string pattern{ "+-*/`~^!|&=><%" };
        return std::any_of(pattern.begin(), pattern.end(),
                           [c](const auto& v) { return v == c; });
#endif

    }

    static bool isOperator(const std::string& word) {
        static const std::regex
            pattern(R"(^(?:\+|-|\*|/|`|~|\^|!|\||\|\||&|&&|=|!=|\<\>|\>|\<|\>=|\<=|%)$)");
        return std::regex_match(word, pattern);
    }

    static bool isUnary(const std::string& word) {
        return (word == "!") || (word == "~") || (word == "`");
    }

    static bool isRightAssociative(const std::string& word) {
        return (word == "^") || (word == "~") || (word == "!");
    }

    static bool isConstant(const std::string& word) {
        return (word == "PI") || (word == "E");
    }

    static bool isFunction(const std::string& word) {
        if (word == "MOD") return true;
        if (word == "POW") return true;
        if (word == "SQRT") return true;
        if (word == "EXP") return true;
        if (word == "LOG") return true;
        if (word == "LOG2") return true;
        if (word == "LOG10") return true;
        if (word == "LN") return true;
        if (word == "SIN") return true;
        if (word == "COS") return true;
        if (word == "TAN") return true;
        if (word == "ASIN") return true;
        if (word == "ACOS") return true;
        if (word == "ATAN") return true;
        return false;
    }

    static std::string getWord(const std::string& expression, size_t& pos) {
        auto size = expression.size();
        std::string word;
        char previous = 0;
        char current = 0;
        while (pos < size) {
            current = expression[pos];
            if (!isCharacter(current)) {
                if ((previous == 'e' || previous == 'E') &&
                    (current == '+' || current == '-')) {

                }
                else {
                    --pos;
                    break;
                }
            }

            word += current;
            previous = current;
            ++pos;
        }
        return word;
    }

    static std::string getOperator(const std::string& expression, size_t& pos) {
        std::string operation(1, expression[pos]);
        if (pos < expression.size() - 1) {
            char next = expression[pos + 1];
            if (isOperator(operation + next)) {
                operation += next;
                ++pos;
            }
        }
        return operation;
    }

    static NumType abs(const NumType a) {
        return a < 0.0 ? -a : a;
    }

    static NumType execute(const NumType a, const NumType b,
        const std::string& operation) {
        static const NumType epsilon = std::numeric_limits<NumType>::min();

        if (operation == "+") return a + b;
        if (operation == "-") return a - b;
        if (operation == "*") return a * b;
        if (operation == "/") {
            if (abs(b) > 0) return a / b;
            throw std::invalid_argument("Divide by zero.");
        }
        if (operation == "^") return std::pow(a, b);
        if (operation == "%") return (abs(a) > 0) ? a * b / 100.0 : b / 100.0;

        if (operation == "=") return abs(a - b) < epsilon;
        if (operation == "!=") return abs(a - b) > epsilon;
        if (operation == "<>") return abs(a - b) > epsilon;
        if (operation == ">") return a > b;
        if (operation == ">=") return a >= b;
        if (operation == "<") return a < b;
        if (operation == "<=") return a <= b;

        if (operation == "||") return a || b;
        if (operation == "&&") return a && b;

        if (operation == "!") return !b;
        if (operation == "`") return -b;
        if (operation == "~") return -b;

        throw std::invalid_argument("The operator \"" + operation +
            "\" is not supported.");

    }

    static NumType callFunction(const std::string& word, std::stack<NumType>& parameters) {
        auto size = parameters.size();

        NumType param1 = 0.0;
        NumType param2 = 0.0;
        if (size > 0) {
            param1 = parameters.top();
            parameters.pop();
        }
        if (size > 1) {
            param2 = parameters.top();
            parameters.pop();
        }

        std::string
            message = "The wrong number of parameters is passed to the " +
            word + " function.";

        if (word == "MOD") {
            if (size != 2) {
                throw std::invalid_argument(message);
            }
            return std::fmod(param1, param2);
        }
        if (word == "POW") {
            if (size != 2) {
                throw std::invalid_argument(message);
            }
            return std::pow(param1, param2);
        }
        if (word == "SQRT") {
            if (size != 1) {
                throw std::invalid_argument(message);
            }
            return std::sqrt(param1);
        }
        if (word == "EXP") {
            if (size != 1) {
                throw std::invalid_argument(message);
            }
            return std::exp(param1);
        }
        if (word == "LOG") {
            if (size != 2) {
                throw std::invalid_argument(message);
            }
            return std::log10(param2) / std::log10(param1);
        }
        if (word == "LOG2") {
            if (size != 1) {
                throw std::invalid_argument(message);
            }
            return std::log2(param1);
        }
        if (word == "LOG10") {
            if (size != 1) {
                throw std::invalid_argument(message);
            }
            return std::log10(param1);
        }
        if (word == "LN") {
            if (size != 1) {
                throw std::invalid_argument(message);
            }
            return std::log(param1);
        }
        if (word == "SIN") {
            if (size != 1) {
                throw std::invalid_argument(message);
            }
            return std::sin(param1);
        }
        if (word == "COS") {
            if (size != 1) {
                throw std::invalid_argument(message);
            }
            return std::cos(param1);
        }
        if (word == "TAN") {
            if (size != 1) {
                throw std::invalid_argument(message);
            }
            return std::tan(param1);
        }
        if (word == "ASIN") {
            if (size != 1) {
                throw std::invalid_argument(message);
            }
            return std::asin(param1);
        }
        if (word == "ACOS") {
            if (size != 1) {
                throw std::invalid_argument(message);
            }
            return std::acos(param1);
        }
        if (word == "ATAN") {
            if (size != 1) {
                throw std::invalid_argument(message);
            }
            return std::atan(param1);
        }
        return 0.0;
    }

    static std::string remove_unary_pluses(const std::string& expression) {
        //    const std::regex pattern(R"(^\+|(?<=[(*\/+^-])(\+)|\+$)");
        //    return std::regex_replace(expression, pattern, "");
        auto size = expression.size();
        size_t pos = 0;
        char previous = 0;
        char current = 0;
        char next = 0;
        std::string result;

        while (pos < size) {
            current = expression[pos];
            if (current == ' ') {
                ++pos;
                continue;
            }

            size_t pos_next = pos + 1;
            next = 0;
            while (pos_next < size) {
                next = expression[pos_next];
                if (next != ' ') {
                    break;
                }
                ++pos_next;
            }

            if (current == '+' &&
                (previous == 0 || previous == '(' ||
                    isOperator(previous) && previous != '+' && previous != '%' ||
                    isOperator(next))) {
                previous = current;
                pos = pos_next;
                continue;
            }

            result += current;
            previous = current;
            pos = pos_next;
        }

        return result;
    }

    static std::string replace_unary_minuses(const std::string& expression) {
        auto size = expression.size();
        size_t pos = 0;
        char previous = 0;
        char current = 0;
        char next = 0;
        bool is_minus = false;
        std::string result;

        while (pos < size) {
            current = expression[pos];

            if (current == '-') {
                while (true) {
                    is_minus = !is_minus;

                    if (pos == size - 1) {
                        break;
                    }
                    next = expression[pos + 1];
                    if (next != '-') {
                        break;
                    }
                    ++pos;
                }

                if (is_minus) {
                    if (previous == 0 || previous == '(' || previous == ',' ||
                        previous == '=' || previous == '>' || previous == '<') {
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
                        previous == '!' || previous == '^' || next == '+') {
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

};

template<typename NumType>
const std::unordered_map<std::string, int>
ReversePolishNotation<NumType>::precedence_{
    { "(", 0 },
    { "=", 1 }, { "!=", 1 }, { "<>", 1 },
    { ">", 1 }, { ">=", 1 }, { "<", 1 }, { "<=", 1 },
    { "||", 2 },
    { "&&", 3 },
    { "+", 4 }, { "-", 4 }, { "`", 4 },
    { "*", 5 }, { "/", 5 }, { "%", 5 },
    { "^", 6 },
    { "!", 7 }, { "~", 7 }
};

#endif // REVERSEPOLISHNOTATION_H
