#include "FileData.hpp"
#include "utils.hpp"

namespace original {

enum class Condition {
    operational,
    damaged,
    unknown
};

inline Condition parseCondition(const char c) {
    switch (c) {
        case '.':
            return Condition::operational;
        case '#':
            return Condition::damaged;
        case '?':
            return Condition::unknown;
        default:
            throw std::invalid_argument("Invalid condition");
    }
}

inline std::string conditionToString(Condition condition) {
    switch (condition) {
        case Condition::operational:
            return ".";
        case Condition::damaged:
            return "#";
        case Condition::unknown:
            return "?";
        default:
            throw std::invalid_argument("Invalid condition");
    }
}

std::vector<Condition> extractConditions(std::string_view line) {
    std::vector<Condition> conditions;
    conditions.reserve(line.size());
    for (const char c : line) {
        conditions.emplace_back(parseCondition(c));
    }
    return conditions;
}

struct Record {
    std::vector<Condition> conditions;
    std::vector<uint> groups;

    Record() = default;

    explicit Record(std::string_view line, size_t N = 1) {
        auto splitAt = line.find_first_of(' ');
        auto conditionsPart = line.substr(0, splitAt);
        auto groupsPart = line.substr(splitAt + 1);

        conditions.reserve((conditionsPart.size() * N) + N - 1);
        groups.reserve(((groupsPart.size() + 1) / 2) * N);

        auto extractedConditions = extractConditions(conditionsPart);
        auto extractedGroups = utils::extractNumbers<uint>(groupsPart, (groupsPart.size() + 1) / 2);

        conditions.insert(conditions.end(), extractedConditions.begin(), extractedConditions.end());
        groups.insert(groups.end(), extractedGroups.begin(), extractedGroups.end());

        for (size_t n = 1; n < N; ++n) {
            conditions.emplace_back(Condition::unknown);
            conditions.insert(conditions.end(), extractedConditions.begin(), extractedConditions.end());
            groups.insert(groups.end(), extractedGroups.begin(), extractedGroups.end());
        }
    }

    std::string to_string() const {
        std::string result = "Record {\n  Conditions: [";
        for (size_t i = 0; i < this->conditions.size(); ++i) {
            result += conditionToString(this->conditions[i]);
            if (i < this->conditions.size() - 1) {
                result += ", ";
            }
        }
        result += "],\n  Groups: ";
        result += utils::vectorToString<uint>(this->groups);
        result += "\n}";
        return result;
    }
};

uint64_t solveRecursive(const Record& record, int i, int j, int cur, std::vector<std::vector<std::vector<uint64_t>>>& dp) {
    const size_t conditionsSize = record.conditions.size();
    const size_t groupsSize = record.groups.size();

    if (i >= conditionsSize) {
        return j == groupsSize;
    }

    if (dp[i][j][cur] != std::numeric_limits<uint64_t>::max()) {
        return dp[i][j][cur];
    }

    uint64_t& res = dp[i][j][cur];
    res = 0;

    if ((record.conditions[i] == Condition::operational or record.conditions[i] == Condition::unknown) and cur == 0) {
        res += solveRecursive(record, i + 1, j, 0, dp);
    }

    if ((record.conditions[i] == Condition::damaged or record.conditions[i] == Condition::unknown) and j < groupsSize) {
        if (cur + 1 == record.groups[j]) {
            res += (i + 1 == conditionsSize or record.conditions[i + 1] != Condition::damaged) * solveRecursive(record, i + 2, j + 1, 0, dp);
        } else {
            res += solveRecursive(record, i + 1, j, cur + 1, dp);
        }
    }

    return res;
}

uint64_t solve(const Record& record) {
    const size_t maxLen = *std::max_element(record.groups.begin(), record.groups.end());
    std::vector<std::vector<std::vector<uint64_t>>> dp(record.conditions.size(), std::vector<std::vector<uint64_t>>(record.groups.size() + 1, std::vector<uint64_t>(maxLen + 1, std::numeric_limits<uint64_t>::max())));

    return solveRecursive(record, 0, 0, 0, dp);
}

uint64_t solution_one() {
    try {
        uint64_t result{0};

        size_t i{0};
        for (const std::string_view line : input::inputContent) {
            result += solve(Record(line));
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

        size_t i{0};
        for (const std::string_view line : input::inputContent) {
            result += solve(Record(line, 5));
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
