#include "FileData.hpp"
#include "utils.hpp"

namespace original {

constexpr size_t SEQ_LENGTH{21};  // 21 or 6

std::vector<int> diffSeq(const std::vector<int>& seq) {
    std::vector<int> diffs;
    diffs.reserve(static_cast<size_t>(seq.size() - 1));
    for (size_t i{0}; i < seq.size() - 1; ++i) {
        diffs.emplace_back(seq[i + 1] - seq[i]);
    }

    return diffs;
}

bool allEqual(const std::vector<int>& seq) {
    return std::all_of(seq.begin() + 1, seq.end(), [&seq](int val) {
        return val == seq[0];
    });
}

int extrapolateForward(const std::vector<int>& seq) {
    std::vector<int> lastValues;
    lastValues.emplace_back(seq.back());

    std::vector<int> differences = seq;
    while (differences.size() > 1 and !allEqual(differences)) {
        differences = diffSeq(differences);
        lastValues.emplace_back(differences.back());
    }

    // extrapolate value based on last values of difference table entries
    int value{0};
    for (auto it = lastValues.crbegin(); it != lastValues.crend(); it++) {
        value += *it;
    }

    return value;
}

int64_t solution_one() {
    try {
        std::array<std::vector<int>, input::inputContent.size()> sequences;

        size_t i{0};
        for (std::string_view line : input::inputContent) {
            sequences[i++] = utils::extractNumbers<int>(line, SEQ_LENGTH);
        }

        int64_t result{0};
        for (const auto& seq : sequences) {
            result += extrapolateForward(seq);
        }

        return result;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

int extrapolateBackward(const std::vector<int>& seq) {
    std::vector<int> firstValues;
    firstValues.emplace_back(seq.front());

    std::vector<int> differences = seq;
    while (differences.size() > 1 and !allEqual(differences)) {
        differences = diffSeq(differences);
        firstValues.emplace_back(differences.front());
    }

    // extrapolate value based on first values of difference table entries
    int value{0};
    for (auto it = firstValues.crbegin(); it != firstValues.crend(); it++) {
        value = *it - value;
    }

    return value;
}

int64_t solution_two() {
    try {
        std::array<std::vector<int>, input::inputContent.size()> sequences;

        size_t i{0};
        for (std::string_view line : input::inputContent) {
            sequences[i++] = utils::extractNumbers<int>(line, SEQ_LENGTH);
        }

        int64_t result{0};
        for (const auto& seq : sequences) {
            result += extrapolateBackward(seq);
        }

        return result;
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
