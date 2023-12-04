#include <array>
#include <queue>

#include "utils.hpp"

namespace original {

const size_t WINNERS{10};  // 10
const size_t PICKS{25};    // 25
const size_t CARDS{193};   // 193

template <std::size_t N>
std::array<int, N> extractNumbers(const std::string_view& line) {
    std::array<int, N> numbers;
    auto head = line.begin();
    auto end = line.end();
    size_t counter{0};

    int number;
    while (head != end) {
        if (*head == ' ') {
            // skip whitespace
            ++head;
            continue;
        } else {
            // Parse the number
            auto [ptr, ec] = std::from_chars(head, end, number);

            if (ec == std::errc() && counter < N) {
                numbers[counter++] = number;
                if (counter == N) {
                    break;
                }
            } else {
                throw std::runtime_error("Parsing error occurred");
            }

            // Advance to the next part of the string
            head = ptr;
        }
    }
    return numbers;
}

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
        utils::LineIterator it("input.txt");
        uint64_t resultSum{0};

        for (; it != utils::LineIterator(); ++it) {
            std::string_view line = it.currentLineView();
            auto [winnersSubstring, picksSubstring] = extractSubstrings(line);
            auto winners = extractNumbers<WINNERS>(winnersSubstring);
            auto picks = extractNumbers<PICKS>(picksSubstring);

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
        utils::LineIterator it("input.txt");
        std::array<size_t, CARDS> cards;
        std::queue<size_t> scratchcards;

        for (size_t id{0}; it != utils::LineIterator(); ++it, ++id) {
            std::string_view line = it.currentLineView();
            auto [winnersSubstring, picksSubstring] = extractSubstrings(line);
            auto winners = extractNumbers<WINNERS>(winnersSubstring);
            auto picks = extractNumbers<PICKS>(picksSubstring);

            auto numMatches = countMatches(winners, picks);
            cards[id] = numMatches;
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
    utils::benchmark(original::solution_one, n);

    std::cout << "\nSolution two" << std::endl;
    utils::benchmark(original::solution_two, n);

    return 0;
}
