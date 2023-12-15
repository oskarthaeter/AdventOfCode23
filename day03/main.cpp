#include <cctype>
#include <charconv>
#include <unordered_map>
#include <unordered_set>

#include "FileData.hpp"
#include "utils.hpp"

namespace original {

const std::string_view digits{"0123456789"};

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};

typedef std::pair<uint8_t, uint8_t> Coordinate;

struct Number {
    uint value;
    uint8_t length;
    Coordinate pos;
};

void parseLine(const std::string_view& line, uint8_t row,
               std::vector<Number>& numbers,
               std::unordered_map<Coordinate, char, pair_hash>& symbols) {
    size_t position = 0;

    while (position < line.size()) {
        char currentChar = line[position];

        if (std::isdigit(currentChar)) {
            // parse number
            uint value;
            auto [ptr, ec] = std::from_chars(line.data() + position, line.data() + line.size(), value);

            if (ec == std::errc()) {
                size_t length = ptr - (line.data() + position);
                numbers.emplace_back(Number{value, static_cast<uint8_t>(length), Coordinate{row, static_cast<uint8_t>(position)}});
                position += length;
            } else {
                ++position;
            }
        } else if (currentChar != '.') {
            // handle symbol
            symbols.emplace(Coordinate{row, static_cast<uint8_t>(position)}, currentChar);
            ++position;
        } else {
            // skip over '.'
            ++position;
        }
    }
}

bool isValid(const std::unordered_map<Coordinate, char, pair_hash>& symbols, const Number& num) {
    for (auto i = num.pos.second - 1; i <= num.pos.second + num.length; ++i) {
        if (i > 0 and num.pos.first != 0 and symbols.contains(std::make_pair(static_cast<uint8_t>(num.pos.first - 1), static_cast<uint8_t>(i))))
            return true;
        if (i > 0 and symbols.contains(std::make_pair(static_cast<uint8_t>(num.pos.first + 1), static_cast<uint8_t>(i))))
            return true;
    }
    bool leftCheck = (num.pos.second != 0 and symbols.contains(std::make_pair(num.pos.first, static_cast<uint8_t>(num.pos.second - 1))));
    bool rightCheck = (symbols.contains(std::make_pair(num.pos.first, static_cast<uint8_t>(num.pos.second + num.length))));
    return leftCheck or rightCheck;
}

uint64_t processData(const std::vector<Number>& numbers,
                     const std::unordered_map<Coordinate, char, pair_hash>& symbols) {
    uint64_t result{0};
    // for every number, check whether their neighboring coordinates are in the symbols set
    for (const auto& num : numbers) {
        auto valid = isValid(symbols, num);
        if (valid) {
            result += num.value;
        }
    }
    return result;
}

uint64_t solution_one() {
    try {
        std::vector<Number> numbers;
        std::unordered_map<Coordinate, char, pair_hash> symbols;

        uint8_t row{0};

        for (const std::string_view line : input::inputContent) {
            parseLine(line, row, numbers, symbols);
            ++row;
        }
        return processData(numbers, symbols);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

// remove invalid numbers
void cleanData(std::vector<Number>& numbers, const std::unordered_map<Coordinate, char, pair_hash>& symbols) {
    numbers.erase(
        std::remove_if(numbers.begin(), numbers.end(),
                       [&symbols](const Number& num) {
                           return !isValid(symbols, num);
                       }),
        numbers.end());
}

void parseGearLine(const std::string_view& line, uint8_t row,
                   std::vector<Number>& numbers,
                   std::unordered_map<Coordinate, char, pair_hash>& symbols, std::vector<Coordinate>& gears) {
    size_t position = 0;

    while (position < line.size()) {
        char currentChar = line[position];

        if (std::isdigit(currentChar)) {
            // parse number
            uint value;
            auto [ptr, ec] = std::from_chars(line.data() + position, line.data() + line.size(), value);

            if (ec == std::errc()) {
                size_t length = ptr - (line.data() + position);
                numbers.emplace_back(Number{value, static_cast<uint8_t>(length), Coordinate{row, static_cast<uint8_t>(position)}});
                position += length;
            } else {
                ++position;
            }
        } else if (currentChar != '.') {
            if (currentChar == '*') {
                gears.emplace_back(Coordinate{row, static_cast<uint8_t>(position)});
            }
            // handle symbol
            symbols.emplace(Coordinate{row, static_cast<uint8_t>(position)}, currentChar);
            ++position;
        } else {
            // skip over '.'
            ++position;
        }
    }
}

uint64_t gearRatio(const std::vector<Number>& numbers, const Coordinate& symbol) {
    std::array<uint, 2> adjacentNumbers{0, 0};
    size_t count = 0;

    for (const auto& num : numbers) {
        // check adjacency
        for (auto i = num.pos.second - 1; i <= num.pos.second + num.length; ++i) {
            if ((i >= 0 and num.pos.first != 0 and
                 (symbol.first == static_cast<uint8_t>(num.pos.first - 1) and symbol.second == static_cast<uint8_t>(i))) or
                (symbol.first == static_cast<uint8_t>(num.pos.first + 1) and symbol.second == static_cast<uint8_t>(i))) {
                adjacentNumbers[count++] = num.value;
                if (count > 2) {
                    return 0;  // early exit if more than two numbers are adjacent
                }
                continue;
            }
        }
        bool leftCheck = (num.pos.second != 0 and (symbol.first == num.pos.first and symbol.second == static_cast<uint8_t>(num.pos.second - 1)));
        bool rightCheck = (symbol.first == num.pos.first and symbol.second == static_cast<uint8_t>(num.pos.second + num.length));
        if (leftCheck or rightCheck) {
            adjacentNumbers[count++] = num.value;
            if (count > 2) {
                return 0;  // early exit if more than two numbers are adjacent
            }
            continue;
        }
    }

    if (count == 2) {
        // If exactly two numbers are adjacent, return their product
        return adjacentNumbers[0] * adjacentNumbers[1];
    } else {
        // If not exactly two numbers are adjacent, return 0
        return 0;
    }
}

uint64_t processGears(const std::vector<Number>& numbers, const std::vector<Coordinate>& gears) {
    uint64_t result{0};

    // iterate through gears
    for (const auto& gear : gears) {
        uint64_t gearResult = gearRatio(numbers, gear);
        result += gearResult;
    }

    return result;
}

uint64_t solution_two() {
    try {
        std::vector<Number> numbers;
        std::unordered_map<Coordinate, char, pair_hash> symbols;
        std::vector<Coordinate> gears;
        uint8_t row{0};

        for (const std::string_view line : input::inputContent) {
            parseGearLine(line, row, numbers, symbols, gears);
            ++row;
        }
        cleanData(numbers, symbols);
        return processGears(numbers, gears);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}
}  // namespace original

int main() {
    constexpr size_t n{100};

    std::cout << "Part 1: " << original::solution_one() << std::endl;
    utils::benchmark<n>(original::solution_one);

    std::cout << "Part 2: " << original::solution_two() << std::endl;
    utils::benchmark<n>(original::solution_two);

    return 0;
}
