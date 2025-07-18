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

class ReversePolishNotashion {
public:
    ReversePolishNotashion() {}

    static bool isNumber(const std::string& word) {
        static const std::regex
            pattern(R"(^^\s*[-+]?((\d+(\.\d+)?)|(\d+\.)|(\.\d+))([eE][-+]?\d+)?\s*$)");
        return std::regex_match(word, pattern);
    }

    // Dijkstra's algorithm
    static std::string to_postfix(std::string& expression) {
        std::string result = "";
        std::stack<std::string> operations;

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
                std::string word = getWord(expression, pos);

                if (isNumber(word)) {
                    result += word + " ";
                    std::cout << "    result(add_word \"" << word << " \") = [" << result << "]\n";
                }
                else if (isFunction(word)) {
                    operations.push(word);
                    std::cout << "    push_function(" << word << ")\n";
                }
                else {
                    std::string message =
                        "Incorrect expression. Unknown word: \"" + word +
                        "\".\nProcessing stopped at position " +
                        std::to_string(pos) + " :\n" + expression.substr(0, pos);
                    throw std::invalid_argument(message);
                }
            }
            else if (isOperator(current)) {
                std::string operation = getOperator(expression, pos);
                std::cout << "operation = " << operation << std::endl;

                if (operation == "%") {
                    result += "% ";
                    std::cout << "    result(add_operation \"% \") = [" << result << "]\n";
                }
                else {
                    if (!isRightAssociative(current)) {
                        while (operations.size() > 0 &&
                            precedence(operations.top()) >= precedence(operation)) {
                            result += operations.top() + " ";
                            std::cout << "    result(top \"" << operations.top() << " \") = [" << result << "]\n";
                            operations.pop();
                        }
                    }
                    else {
                        while (operations.size() > 0 &&
                            precedence(operations.top()) > precedence(operation)) {
                            result += operations.top() + " ";
                            std::cout << "    result(top \"" << operations.top() << " \") = [" << result << "]\n";
                            operations.pop();
                        }
                    }
                    operations.push(operation);
                    std::cout << "    push_operation(" << operation << ")" << std::endl;
                }
            }
            else if (current == '(') {
                if (operations.size() > 0) {
                    if (isFunction(operations.top())) {
                        result += "' ";
                        std::cout << "    result(add \"'\") = [" << result << "]\n";
                    }
                }
                operations.push("(");
                std::cout << "    push('(')" << std::endl;
            }
            else if (current == ')') {
                bool balanced_brackets = false;
                while (operations.size() > 0) {
                    if (operations.top() == "(") {
                        operations.pop();
                        std::cout << "found '('\n";
                        balanced_brackets = true;
                        break;
                    }
                    result += operations.top() + " ";
                    std::cout << "    result(top \"" << operations.top() << " \") = [" << result << "]\n";
                    operations.pop();
                }

                if (!balanced_brackets) {
                    std::string message =
                        "Incorrect expression. The brackets are not balanced.\n";
                    message += "The opening bracket '(' is missing. ";
                    message += "Processing stopped at position " +
                        std::to_string(pos) + " :\n" + expression.substr(0, pos);
                    throw std::invalid_argument(message);
                }

                if (operations.size() > 0) {
                    if (isFunction(operations.top())) {
                        result += operations.top() + " ";
                        std::cout << "    result(top \"" << operations.top() << " \") = [" << result << "]\n";
                        operations.pop();
                    }
                }
            }
            else if (current == ',') {
                while (operations.size() > 0 && operations.top() != "(") {
                    result += operations.top() + " ";
                    std::cout << "    result(top \"" << operations.top() << " \") = [" << result << "]\n";
                    operations.pop();
                }
            }
            else {
                std::string message =
                    "Incorrect expression. Unknown symbol: '" +
                    std::string(1, current) + "'.\n" +
                    "Processing stopped at position " +
                    std::to_string(pos) + " :\n" + expression.substr(0, pos);
                throw std::invalid_argument(message);
            }

            ++pos;
        }

        while (operations.size() > 0) {
            result += operations.top() + " ";
            std::cout << "top=" << operations.top() << " " << "res=[" << result << "]\n";
            operations.pop();
        }

        return result;
    }

    static double calculate(std::string& infixExpression) {
        auto postfixExpression = to_postfix(infixExpression);
        auto size = postfixExpression.size();
        size_t pos = 0;
        char current = 0;
        double result = 0;
        std::stack<std::string> operands;

        std::cout << "START CALCULATION" << std::endl;

        while (pos < size) {
            current = postfixExpression[pos];
            std::cout << "current=" << current << std::endl;

            if (isOperator(current)) {
                char next = (pos < size - 1) ? postfixExpression[pos + 1] : 0;
                std::string operation(1, current);
                if (isOperator(operation + next)) {
                    operation += next;
                }
                std::cout << "operation = " << operation << std::endl;

                double operand1 = 0.0, operand2 = 0.0;
                if (operands.size() > 0) {
                    std::stringstream(operands.top()) >> operand2;
                    operands.pop();
                }

                if (!isUnary(current)) {
                    if (operands.size() > 0) {
                        std::stringstream(operands.top()) >> operand1;
                        if (current != '%') {
                            operands.pop();
                        }
                    }
                }

                operand2 = execute(operand1, operand2, operation);
                operands.push(std::to_string(operand2));
            }
            else if (current == '\'') {
                operands.push("'");
            }
            else if (isCharacter(current)) {
                std::string word = getWord(postfixExpression, pos);
                if (isNumber(word)) {
                    operands.push(word);
                }
                else {

                }

            }

            ++pos;
        }

        return result;
    }

    ~ReversePolishNotashion() {}
private:
    static const std::unordered_map<std::string, int> precedence_;

    static const int precedence(const std::string& operation) {
        try {
            return precedence_.at(operation);
        }
        catch (const std::out_of_range& e) {
            std::cerr << "The operator \"" << operation
                      << "\" is not supported." << std::endl;
        }
    }

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
        static const std::string 
        operators{ "+-*/`~^!|&=><%" };
        for (const char& symbol : operators) {
            if (c == symbol) return true;
        }
        return false;
    }

    static bool isOperator(const std::string& word) {
        static const std::regex 
        pattern(R"(^(?:\+|-|\*|/|`|~|\^|!|\||\|\||&|&&|=|!=|\<\>|\>|\<|\>=|\<=|%)$)");
        return std::regex_match(word, pattern);
    }

    static bool isUnary(const char& c) {
        return (c == '^') || (c == '!') || (c == '~') || (c == '`');
    }

    static bool isRightAssociative(const char& c) {
        return (c == '^') || (c == '~') || (c == '!');
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
        return false;
    }

    static std::string getWord(const std::string& expression, size_t& pos) {
        auto size = expression.size() - 1;
        std::string word(1, expression[pos]);
        while (pos < size) {
            char current = expression[pos + 1];
            if (!isCharacter(current)) {
                break;
            }
            word += current;
            ++pos;
        }
        return word;
    }

    static std::string getOperator(const std::string& expression, size_t pos) {
        std::string operation(1, expression[pos++]);
        if (pos < expression.size()) {
            char next = expression[pos];
            if (isOperator(operation + next)) {
                operation += next;
            }
        }
        return operation;
    }

    static double execute(const double a, const double b,
        const std::string& operation) {
        static const double epsilon = 1.0e-12;
        if (operation == "+") return a + b;
        if (operation == "-") return a - b;
        if (operation == "*") return a * b;
        if (operation == "/") return a / b;
        if (operation == "^") return std::pow(a, b);
        if (operation == "%") return (std::abs(a) > 0) ? a * b / 100.0 : b / 100.0;

        if (operation == "=") return std::abs(a - b) < epsilon;
        if (operation == "!=") return std::abs(a - b) > epsilon;
        if (operation == "<>") return std::abs(a - b) > epsilon;
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

        return 0.0;
    }

    static std::string remove_unary_pluses(std::string& expression) {
        //    const std::regex pattern(R"(^\+|(?<=[(*\/+^-])(\+)|\+$)");
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
                next = expression[pos_next];
                if (next != ' ') {
                    break;
                }
                ++pos_next;
            }

            if (current == '+' &&
                (previous == 0 || previous == '(' || isOperator(previous) ||
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

    static std::string replace_unary_minuses(std::string& expression) {
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
                while (true) {
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

};

const std::unordered_map<std::string, int> 
ReversePolishNotashion::precedence_ {
    { "=", 1 }, { "!=", 1 }, { "<>", 1 },
    { "||", 2 },
    { "&&", 3 },
    { "+", 4 }, { "-", 4 }, { "`", 4 },
    { "*", 5 }, { "/", 5 }, { "%", 5 },
    { "^", 6 },
    { "!", 7 }, { "~", 7 }
};

int main() {
    ReversePolishNotashion rpn;

    std::string num = "012345678901234567890123456789012345678901234567890123456789.";
    std::cout << std::boolalpha << rpn.isNumber(num) << " " << std::stold(num) << std::endl;

    double x;
    std::stringstream(num) >> x;
    std::cout << x << std::endl;

    std::string expr = "-2 + +++- (--3, 4,-+--+8)+++ -++- 566++- 10%   %%  -++ --+2 / ( 55 -+- -45 -+3+9) ---2^---2+---";
    expr = "6 - 5 %";
    try {
        auto res = rpn.calculate(expr);
        std::cout << res << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    }


    return 0;
}