#include <array>
#include <queue>

#include "FileData.hpp"
#include "utils.hpp"

namespace original {

const size_t WINNERS{10};  // 10
const size_t PICKS{25};    // 25
const size_t CARDS{193};   // 193

std::pair<std::string_view, std::string_view> extractSubstrings(const std::string_view& line) {
    size_t colonPos = line.find(':');  // always followed by a whitespace
    size_t winnersStart = line.find_first_not_of(' ', colonPos + 2);

    size_t barPos = line.find('|', colonPos + 3);  // always followed by a whitespace
    size_t picksStart = line.find_first_not_of(' ', barPos + 2);

    auto winnersSubstring = line.substr(winnersStart, barPos - winnersStart - 1);
    auto picksSubstring = line.substr(picksStart);
    return std::make_pair(winnersSubstring, picksSubstring);
}

size_t countMatches(const std::array<int, WINNERS>& winners, const std::array<int, PICKS>& picks) {
    size_t matchCount = 0;

    for (int winner : winners) {
        for (int pick : picks) {
            if (winner == pick) {
                ++matchCount;
                break;  // assuming no duplicates within each array
            }
        }
    }

    return matchCount;
}

uint64_t solution_one() {
    try {
        uint64_t resultSum{0};

        for (const std::string_view line : input::inputContent) {
            auto [winnersSubstring, picksSubstring] = extractSubstrings(line);
            auto winners = utils::extractNumbers<int, WINNERS>(winnersSubstring);
            auto picks = utils::extractNumbers<int, PICKS>(picksSubstring);

            auto numMatches = countMatches(winners, picks);
            if (numMatches == 0)
                continue;
            else
                resultSum += (0b1 << (numMatches - 1));
        }
        return resultSum;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

void addCopies(std::queue<size_t>& scratchcards, size_t cardId, size_t numMatches) {
    // add the next <numMatches> cardIds to the stack
    for (size_t i{1}; i <= numMatches; ++i) {
        scratchcards.push(cardId + i);
    }
}

uint64_t solution_two() {
    try {
        std::array<size_t, CARDS> cards;
        std::queue<size_t> scratchcards;
        size_t id{0};

        for (const std::string_view line : input::inputContent) {
            auto [winnersSubstring, picksSubstring] = extractSubstrings(line);
            auto winners = utils::extractNumbers<int, WINNERS>(winnersSubstring);
            auto picks = utils::extractNumbers<int, PICKS>(picksSubstring);

            auto numMatches = countMatches(winners, picks);
            cards[id++] = numMatches;
            scratchcards.push(id);
        }
        uint64_t resultSum{0};
        // iterate through scratchcards queue until it is empty
        while (!scratchcards.empty()) {
            auto cardId = scratchcards.front();
            scratchcards.pop();  // remove cardId from queue
            addCopies(scratchcards, cardId, cards[cardId]);
            ++resultSum;
        }
        return resultSum;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}
}  // namespace original

int main() {
    const size_t n{10};

    std::cout << "Solution one" << std::endl;
    utils::benchmark<n>(original::solution_one);

    std::cout << "\nSolution two" << std::endl;
    utils::benchmark<n>(original::solution_two);

    return 0;
}
