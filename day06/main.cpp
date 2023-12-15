#include "FileData.hpp"
#include "utils.hpp"

namespace original {

uint64_t countOptimalChargingTimes(uint64_t duration, uint64_t record_distance) {
    double sqrtPart = std::sqrt(duration * duration - 4 * record_distance);
    uint64_t lowerBound = std::floor(0.5 * (duration - sqrtPart) + 1.);
    uint64_t upperBound = std::ceil(0.5 * (duration + sqrtPart) - 1.);

    return (upperBound - lowerBound + 1);
}

std::vector<uint64_t> extractDurations(const std::string_view& line) {
    if (line.starts_with("Time: ")) {
        return utils::extractNumbers<uint64_t>(line.substr(7));
    } else {
        throw std::invalid_argument("could not parse durations");
    }
}

std::vector<uint64_t> extractRecords(const std::string_view& line) {
    if (line.starts_with("Distance: ")) {
        return utils::extractNumbers<uint64_t>(line.substr(11));
    } else {
        throw std::invalid_argument("could not parse durations");
    }
}

uint64_t solution_one() {
    try {
        const std::array<std::string, 2> lines = input::content;
        auto durations = extractDurations(lines[0]);
        auto records = extractRecords(lines[1]);
        uint64_t result{1};
        for (auto i = 0; i < durations.size(); ++i) {
            result *= countOptimalChargingTimes(durations[i], records[i]);
        }
        return result;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

uint64_t concatNumbers(const std::vector<uint64_t>& numbers) {
    std::ostringstream oss;
    for (const auto& number : numbers) {
        oss << number;
    }

    std::string concatenatedString = oss.str();

    // Convert back to a number
    uint64_t concatenatedNumber = std::stoull(concatenatedString);
    return concatenatedNumber;
}

uint64_t solution_two() {
    try {
        const std::array<std::string, 2> lines = input::content;
        uint64_t duration = concatNumbers(extractDurations(lines[0]));
        uint64_t record = concatNumbers(extractRecords(lines[1]));
        return countOptimalChargingTimes(duration, record);
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
