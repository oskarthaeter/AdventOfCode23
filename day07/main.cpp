#include <map>
#include <set>

#include "FileData.hpp"
#include "utils.hpp"

namespace original {

enum Card { joker,
            two,
            three,
            four,
            five,
            six,
            seven,
            eight,
            nine,
            ten,
            jack,
            queen,
            king,
            ace };
enum Type { highCard,
            onePair,
            twoPair,
            threeOfAKind,
            fullHouse,
            fourOfAKind,
            fiveOfAKind };

Card charToCard(char c) {
    switch (c) {
        case '2':
            return two;
        case '3':
            return three;
        case '4':
            return four;
        case '5':
            return five;
        case '6':
            return six;
        case '7':
            return seven;
        case '8':
            return eight;
        case '9':
            return nine;
        case 'T':
            return ten;
        case 'J':
            return jack;
        case 'Q':
            return queen;
        case 'K':
            return king;
        case 'A':
            return ace;
        default:
            throw std::invalid_argument("Invalid card character");
    }
}

Card charToJokerCard(char c) {
    switch (c) {
        case 'J':
            return joker;
        case '2':
            return two;
        case '3':
            return three;
        case '4':
            return four;
        case '5':
            return five;
        case '6':
            return six;
        case '7':
            return seven;
        case '8':
            return eight;
        case '9':
            return nine;
        case 'T':
            return ten;
        case 'Q':
            return queen;
        case 'K':
            return king;
        case 'A':
            return ace;
        default:
            throw std::invalid_argument("Invalid card character");
    }
}

struct Hand {
    std::array<Card, 5> cards;
    Type type;
    int bid;

    Hand(std::string_view line, bool withJoker = false) {
        using enum Card;
        size_t i{0};
        if (withJoker) {
            for (; i < 5; ++i) {
                cards[i] = charToJokerCard(line[i]);
            }
        } else {
            for (; i < 5; ++i) {
                cards[i] = charToCard(line[i]);
            }
        }

        std::string_view bidPart = line.substr(line.find_last_of(' ', 6) + 1);
        auto [ptr, ec] = std::from_chars(bidPart.data(), bidPart.data() + bidPart.size(), bid);
        if (ec != std::errc()) {
            // Handle error
            throw std::runtime_error("Parsing error");
        }

        type = getType(withJoker);
    }

    Type getType(bool withJoker) {
        std::map<Card, int> numbers;
        for (Card& c : cards)
            numbers[c]++;

        std::multiset<int> types;
        for (auto& [card, num] : numbers) {
            if (withJoker and card == Card::joker)
                continue;
            types.insert(num);
        }

        Type t;
        using enum Type;
        if (types.empty())
            t = Type::highCard;
        else {
            auto typesIt = types.crbegin();
            switch (*typesIt) {
                case 5:
                    t = Type::fiveOfAKind;
                    break;
                case 4:
                    t = Type::fourOfAKind;
                    break;
                case 3:
                    t = Type::threeOfAKind;
                    break;
                case 2:
                    t = Type::onePair;
                    break;
                default:
                    t = Type::highCard;
                    break;
            }

            if (typesIt != types.crend()) {
                ++typesIt;
                if (t == Type::onePair and *typesIt == 2) t = Type::twoPair;
                if (t == Type::threeOfAKind and *typesIt == 2) t = Type::fullHouse;
            }
        }

        if (withJoker) {
            int countJokers = numbers[Card::joker];

            if (countJokers >= 4) t = Type::fiveOfAKind;
            if (countJokers == 3) {
                if (t >= Type::onePair)
                    t = Type::fiveOfAKind;
                else
                    t = Type::fourOfAKind;
            }
            if (countJokers == 2) {
                if (t >= Type::threeOfAKind)
                    t = Type::fiveOfAKind;
                else if (t >= Type::onePair)
                    t = Type::fourOfAKind;
                else
                    t = Type::threeOfAKind;
            }
            if (countJokers == 1) {
                if (t >= Type::fourOfAKind)
                    t = Type::fiveOfAKind;
                else if (t >= Type::threeOfAKind)
                    t = Type::fourOfAKind;
                else if (t == Type::twoPair)
                    t = Type::fullHouse;
                else if (t == Type::onePair)
                    t = Type::threeOfAKind;
                else
                    t = Type::onePair;
            }
        }

        return t;
    }
};

bool compare(const Hand& lhs, const Hand& rhs) {
    // First compare the type of each hand
    if (lhs.type != rhs.type)
        return lhs.type < rhs.type;

    // If types are the same, compare the cards in order
    for (size_t i{0}; i < 5; ++i) {
        if (lhs.cards[i] != rhs.cards[i])
            return lhs.cards[i] < rhs.cards[i];
    }

    // If all cards are the same, the hands are equal, so return false
    return false;
}

uint64_t solution_one() {
    try {
        uint64_t result{0};
        std::vector<Hand> hands;

        for (std::string_view line : input::inputContent) {
            hands.emplace_back(line);
        }
        std::sort(hands.begin(), hands.end(), compare);

        for (size_t i{0}; i < hands.size(); ++i) {
            result += (i + 1) * hands[i].bid;
        }
        return result;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

uint64_t solution_two() {
    try {
        uint64_t result{0};
        std::vector<Hand> hands;

        for (std::string_view line : input::inputContent) {
            hands.push_back(Hand{line, true});
        }

        std::sort(hands.begin(), hands.end(), compare);

        for (size_t i{0}; i < hands.size(); ++i) {
            result += (i + 1) * hands[i].bid;
        }
        return result;
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
