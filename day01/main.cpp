#include <regex>

#include "FileData.hpp"
#include "utils.hpp"

using namespace std::literals;

// get first and last digit on line
// return combination of digits: <first digit><last digit>
int calibrationValue_one(const std::string_view line) {
    std::string number;
    char lastDigit = '\0';
    bool foundFirst = false;

    for (char c : line) {
        if (std::isdigit(c)) {
            if (!foundFirst) {
                number += c;
                foundFirst = true;
            }
            lastDigit = c;
        }
    }

    if (lastDigit != '\0') {
        number += lastDigit;
    }

    return number.empty() ? 0 : std::stoi(number);
}

uint64_t solution_one() {
    try {
        uint64_t resultSum{0};

        for (const std::string_view line : input::inputContent) {
            auto temp = calibrationValue_one(line);
            resultSum += temp;
        }

        return resultSum;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

std::string wordToDigit(const std::string& word) {
    if (word == "one")
        return "1";
    else if (word == "two")
        return "2";
    else if (word == "three")
        return "3";
    else if (word == "four")
        return "4";
    else if (word == "five")
        return "5";
    else if (word == "six")
        return "6";
    else if (word == "seven")
        return "7";
    else if (word == "eight")
        return "8";
    else if (word == "nine")
        return "9";
    else if (word == "")
        return "0";
    else
        return word;
}

int calibrationValue_two(const std::string& line) {
    const std::regex re_first("^(?:.*?)(\\d|one|two|three|four|five|six|seven|eight|nine)(?:.*)$");  // starts with lazy quantifier
    const std::regex re_last("^(?:.*)(\\d|one|two|three|four|five|six|seven|eight|nine)(?:.*)$");    // starts with greedy quantifier

    std::smatch firstMatch;
    std::smatch lastMatch;

    std::ssub_match firstCaptured;
    std::ssub_match lastCaptured;

    if (std::regex_match(line, firstMatch, re_first)) {
        // extract the submatch for the capturing group
        firstCaptured = firstMatch[1];
    }
    auto split = firstCaptured.second;
    if (std::regex_match(split, line.end(), lastMatch, re_last)) {
        // extract the submatch for the capturing group
        lastCaptured = lastMatch[1];
    }

    if (firstMatch.empty()) {
        return 0;
    } else if (lastMatch.empty()) {
        return std::stoi(wordToDigit(firstCaptured.str()) + wordToDigit(firstCaptured.str()));
    } else {
        return std::stoi(wordToDigit(firstCaptured.str()) + wordToDigit(lastCaptured.str()));
    }
}

uint64_t solution_two() {
    try {
        uint64_t resultSum{0};

        for (const std::string& line : input::inputContent) {
            auto temp = calibrationValue_two(line);
            resultSum += temp;
        }

        return resultSum;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

int main() {
    const size_t n{10};

    std::cout << "Solution one" << std::endl;
    utils::benchmark<n>(solution_one);

    std::cout << "\nSolution two" << std::endl;
    utils::benchmark<n>(solution_two);
}
