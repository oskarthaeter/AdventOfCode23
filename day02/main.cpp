#include <cstddef>
#include <iterator>
#include <unordered_map>

#include "FileData.hpp"
#include "utils.hpp"

namespace original {
using namespace std::literals;

enum class Color { red,
                   green,
                   blue };

std::pair<Color, uint> extractCubecount(std::string_view gameCube) {
    // "<count> <color>" where color is one of "red", "green", "blue"
    // split at whitespace

    std::string_view countPart, colorPart;

    auto splitPos{gameCube.find(' ')};
    countPart = gameCube.substr(size_t(0), splitPos);
    colorPart = gameCube.substr(splitPos + 1);

    uint cubeCount;
    auto [ptr, ec] = std::from_chars(countPart.data(), countPart.data() + countPart.size(), cubeCount);

    if (ec != std::errc()) {
        throw std::invalid_argument("Could not convert count part");
    }

    if (colorPart == "red")
        return std::pair<Color, uint>{Color::red, cubeCount};
    else if (colorPart == "green")
        return std::pair<Color, uint>{Color::green, cubeCount};
    else if (colorPart == "blue")
        return std::pair<Color, uint>{Color::blue, cubeCount};
    else
        throw std::invalid_argument("Did not recognise color");
}

std::unordered_map<Color, uint> cubeCounts(std::vector<std::string_view> setDrawings) {
    std::unordered_map<Color, uint> cubes = {{Color::red, 0}, {Color::green, 0}, {Color::blue, 0}};
    for (auto drawing : setDrawings) {
        std::pair<Color, uint> p = extractCubecount(drawing);
        cubes[p.first] += p.second;
    }
    return cubes;
}

std::vector<std::string_view> extractDrawings(std::string_view gameSet) {
    auto numOfDrawings = std::count(gameSet.cbegin(), gameSet.cend(), ',') + 1;
    if (numOfDrawings <= 0) {
        throw std::invalid_argument("No drawings found in set");
    } else {
        std::vector<std::string_view> drawings(numOfDrawings);
        size_t delimiterPos;
        for (size_t counter{0}; counter < numOfDrawings - 1; ++counter) {
            // ends at next occurrence of ','
            delimiterPos = gameSet.find(',');
            drawings[counter] = gameSet.substr(size_t(0), delimiterPos);
            // move start of gameSet
            gameSet.remove_prefix(delimiterPos + 2);  // ", " is two chars long
        }
        // extract last drawing, ends with EOL
        drawings[numOfDrawings - 1] = gameSet.substr();
        return drawings;
    }
}

std::vector<std::string_view> extractSets(std::string_view gameRecord) {
    // game record starts with "Game <id>: " => gameSets start 1 after ':'
    gameRecord.remove_prefix(gameRecord.find(':') + 2);  // ": " is two chars long
    // extract sets delimited by ';'
    // count the number of ';' in gameRecord
    auto numOfSets = std::count(gameRecord.cbegin(), gameRecord.cend(), ';') + 1;
    if (numOfSets <= 0) {
        throw std::invalid_argument("No sets found in game");
    } else {
        std::vector<std::string_view> gameSets(numOfSets);
        size_t delimiterPos;
        for (size_t counter{0}; counter < numOfSets - 1; ++counter) {
            // ends at next occurrence of ';'
            delimiterPos = gameRecord.find(';');
            gameSets[counter] = gameRecord.substr(size_t(0), delimiterPos);
            // move start of gameRecord
            gameRecord.remove_prefix(delimiterPos + 2);  // "; " is two chars long
        }
        // extract last set, ends with EOL
        gameSets[numOfSets - 1] = gameRecord.substr();
        return gameSets;
    }
}

bool possibleGameset(std::unordered_map<Color, uint> cubes) {
    return cubes[Color::red] <= 12 and cubes[Color::green] <= 13 and cubes[Color::blue] <= 14;
}

uint64_t solution_one() {
    try {
        uint64_t resultSum{0};
        size_t gameCounter = 1;
        bool gamePossible;

        for (const std::string_view line : input::inputContent) {
            gamePossible = true;
            auto gameSets = extractSets(line);

            for (auto currentSet : gameSets) {
                const auto drawings = extractDrawings(currentSet);
                auto cubes = cubeCounts(drawings);
                auto setPossible = possibleGameset(cubes);
                gamePossible &= setPossible;
            }
            if (gamePossible == true)
                resultSum += gameCounter;
            ++gameCounter;
        }
        return resultSum;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

std::unordered_map<Color, uint> minimumCube(std::vector<std::string_view> setDrawings) {
    std::unordered_map<Color, uint> cubes = {{Color::red, 0}, {Color::green, 0}, {Color::blue, 0}};
    for (auto drawing : setDrawings) {
        std::pair<Color, uint> p = extractCubecount(drawing);
        if (cubes[p.first] < p.second)
            cubes[p.first] = p.second;
    }
    return cubes;
}

uint64_t cubePower(std::unordered_map<Color, uint> cube) {
    return cube[Color::red] * cube[Color::green] * cube[Color::blue];
}

uint64_t solution_two() {
    try {
        uint64_t resultSum{0};

        for (const std::string_view line : input::inputContent) {
            std::vector<std::string_view> gameSets = extractSets(line);

            // extract all drawings from the sets
            std::vector<std::string_view> allDrawings;
            for (auto gameSet : gameSets) {
                std::vector<std::string_view> drawings = extractDrawings(gameSet);
                allDrawings.insert(allDrawings.end(), drawings.begin(), drawings.end());
            }

            // then calculate the minimum for each cube color from all drawings in one go
            std::unordered_map<Color, uint> miniCube = minimumCube(allDrawings);
            resultSum += cubePower(miniCube);
        }
        return resultSum;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}
}  // namespace original

int main() {
    const size_t n{100};

    std::cout << "Solution one" << std::endl;
    utils::benchmark<n>(original::solution_one);

    std::cout << "\nSolution two" << std::endl;
    utils::benchmark<n>(original::solution_two);

    return 0;
}
