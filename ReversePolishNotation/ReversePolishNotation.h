#include <iostream>
#include <iomanip>
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
                    std::cout << "    result + \"" << word << " \" = [" << result << "]\n";
                }
                else {
                    std::transform(word.begin(), word.end(), word.begin(), ::toupper);
                    if (isFunction(word)) {
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
            }
            else if (isOperator(current)) {
                std::string operation = getOperator(expression, pos);
                std::cout << "operation = " << operation << std::endl;

                if (operation == "%") {
                    result += "% ";
                    std::cout << "    result + \"% \" = [" << result << "]\n";
                }
                else {
                    std::cout << "    precedence : operation=" << operation << std::endl;
                    if (!isRightAssociative(current)) {
                        while (operations.size() > 0 &&
                            precedence(operations.top()) >= precedence(operation)) {
                            std::cout << "precedence : operations.top() = " << operations.top()
                                      << " operation = " << operation << std::endl;
                            result += operations.top() + " ";
                            std::cout << "    result + \"" << operations.top() << "\" = [" << result << "]\n";
                            operations.pop();
                        }
                    }
                    else {
                        while (operations.size() > 0 &&
                            precedence(operations.top()) > precedence(operation)) {
                            std::cout << "precedence : operations.top() = " << operations.top()
                                      << " operation = " << operation << std::endl;
                            result += operations.top() + " ";
                            std::cout << "    result + \"" << operations.top() << "\" = [" << result << "]\n";
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
                        std::cout << "    result + \"'\" = [" << result << "]\n";
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
                    std::cout << "    result + \"" << operations.top() << "\" = [" << result << "]\n";
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
        bool isParameter = false;
        int parameters_count = 0;
        double value = 0.0;
        std::stack<double> operands;

        std::cout << std::setprecision(15) << "START CALCULATION" << std::endl;

        while (pos < size) {
            current = postfixExpression[pos];
            std::cout << "current = [" << current << "]" << std::endl;

            if (isOperator(current)) {
                std::string operation = getOperator(postfixExpression, pos);

                std::cout << "    operation = [" << operation << "]" << std::endl;

                double operand1 = 0.0;
                double operand2 = 0.0;
                if (operands.size() > 0) {
                    operand2 = operands.top();
                    operands.pop();
                    std::cout << "    operands.top() = " << operand2 << std::endl;
                    if (isParameter) {
                        --parameters_count;
                        std::cout << "    after operand2 = [" << operand2
                            << "] parameters_count = " << parameters_count << std::endl;
                    }

                }
                else {
                    throw std::invalid_argument(
                        "Incorrect expression: no operand(s) for the \"" +
                        operation + "\" operator.");
                }

                if (!isUnary(current)) {
                    if (operands.size() > 0) {
                        operand1 = operands.top();
                        if (current != '%') {
                            operands.pop();
                            if (isParameter) {
                                --parameters_count;
                                std::cout << "    after operand1 = [" << operand1
                                    << "] parameters_count = " << parameters_count << std::endl;
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
                std::cout << "    [" << operand1 << " " << operation << " "
                    << operand2 << " = " << value << "]" << std::endl;
                operands.push(value);
                if (isParameter) {
                    ++parameters_count;
                    std::cout << "    after operator = [" << current
                              << "] parameters_count = " << parameters_count << std::endl;
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
                        std::cout << "    after number = [" << value
                            << "] parameters_count = " << parameters_count << std::endl;
                    }
                }
                else {
                    
                    std::stack<double> parameters;
                    while (operands.size() > 0 && parameters_count-- > 0) {
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
                "Incorrect expression: no operand or operator.");
        }

        return operands.top();
    }

    ~ReversePolishNotashion() {}
private:
    static const std::unordered_map<std::string, int> precedence_;

    static const int precedence(const std::string& operation) {
        try {
            return precedence_.at(operation);
        }
        catch (const std::out_of_range& e) {
            std::cerr << "precedence : The operator \"" << operation
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
        if (word == "MOD") return true;
        if (word == "POW") return true;
        if (word == "EXP") return true;
        if (word == "SQRT") return true;
        if (word == "LOG") return true;
        if (word == "LOG2") return true;
        if (word == "LOG10") return true;
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
        if (operation == "/") {
            if (std::abs(b) > 0) return a / b;
            throw std::invalid_argument("Divide by zero.");
        }
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

    static double callFunction(const std::string& word, std::stack<double>& parameters) {
        auto size = parameters.size();

        double param1 = 0.0;
        double param2 = 0.0;
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
        if (word == "EXP") {
            if (size != 1) {
                throw std::invalid_argument(message);
            }
            return std::exp(param1);
        }
        if (word == "SQRT") {
        	if (size != 1) {
                throw std::invalid_argument(message);
            }
            return std::sqrt(param1);
        }
        if (word == "LOG") {
        	if (size != 1) {
                throw std::invalid_argument(message);
            }
            return std::log(param1);
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
                (previous == 0 || previous == '(' || 
                 isOperator(previous) && previous != '%' ||
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
    { "(", 0 },
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
    expr = " (5, 1*)";
    try {
        auto res = rpn.calculate(expr);
        std::cout << std::setprecision(15) << res << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    }

    return 0;
}
