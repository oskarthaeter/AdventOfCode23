#include <map>
#include <sstream>

#include "FileData.hpp"
#include "utils.hpp"

namespace original {

using namespace std::literals::string_view_literals;

struct MapEntry {
    int64_t destStart;
    int64_t sourceStart;
    int64_t rangeLength;
};

class NumberMapper {
   public:
    std::string source_;
    std::string destination_;

    NumberMapper(std::string source, std::string destination, std::vector<MapEntry>& mappings) {
        source_ = source;
        destination_ = destination;

        for (const auto& mapping : mappings) {
            rangeMap_[{mapping.sourceStart, mapping.sourceStart + mapping.rangeLength - 1}] = mapping.destStart - mapping.sourceStart;
        }
    }

    int64_t mapNumber(int64_t number) const {
        for (auto& range : rangeMap_) {
            if (number >= range.first.first and number <= range.first.second) {
                return number + range.second;
            }
        }

        // std::cout << "Number does not fall in any range. Returning the original number: " << number << std::endl;
        return number;  // Default case: return the number as is
    }

   private:
    std::map<std::pair<int64_t, int64_t>, int64_t> rangeMap_;
};

bool isSeedLine(const std::string_view& line) {
    return line.starts_with("seeds:"sv);
}

std::vector<int64_t> extractSeeds(const std::string_view& line) {
    return utils::extractNumbers<int64_t>(line.substr(7));
}

bool isMapNameLine(const std::string_view& line) {
    size_t toPos = line.find("-to-");
    return toPos != std::string_view::npos && line.find(" map:") != std::string_view::npos;
}

std::pair<std::string, std::string> extractMapName(const std::string_view& line) {
    size_t toPos = line.find("-to-");

    std::string_view source = line.substr(0, toPos);
    size_t destStart = toPos + 4;  // Length of "-to-" is 4
    size_t destEnd = line.find(" map:");
    std::string_view destination = line.substr(destStart, destEnd - destStart);
    return std::make_pair(std::string(source), std::string(destination));
}

bool isMapEntry(const std::string_view& line) {
    int spaceCount = 0;
    for (char ch : line) {
        if (ch == ' ') {
            ++spaceCount;
        }
    }
    return spaceCount == 2;
}

MapEntry extractEntry(const std::string_view& line) {
    std::vector<int64_t> numbers = utils::extractNumbers<int64_t>(line);
    if (numbers.size() == 3) {
        return MapEntry{numbers[0], numbers[1], numbers[2]};
    } else {
        std::cout << std::quoted(line);
        throw std::invalid_argument("line could not be parsed into MapEntry");
    }
}

std::string translateNumbers(const std::vector<NumberMapper>& mappers, std::string sourcename, std::vector<int64_t>& numbers) {
    // find the NumberMapper that has the given sourcename
    auto mapper = *std::find_if(mappers.begin(), mappers.end(), [sourcename](const NumberMapper& mapper) { return mapper.source_ == sourcename; });
    for (auto i{0}; i < numbers.size(); ++i) {
        numbers[i] = mapper.mapNumber(numbers[i]);
    }
    return mapper.destination_;
}

int64_t solution_one() {
    try {
        std::vector<int64_t> seeds;
        std::vector<NumberMapper> mappers;
        bool readingMap{false};
        std::pair<std::string, std::string> mapname;
        std::vector<MapEntry> entries;

        for (const std::string_view line : input::inputContent) {
            // first line should contain the seeds
            if (seeds.empty() and isSeedLine(line)) {
                seeds = extractSeeds(line);
            } else if (!readingMap and isMapNameLine(line)) {
                mapname = extractMapName(line);
                readingMap = true;
            } else if (readingMap and isMapEntry(line)) {
                entries.push_back(extractEntry(line));
            } else if (readingMap and line.empty()) {
                readingMap = false;
                mappers.push_back(NumberMapper{
                    mapname.first, mapname.second, entries});
                // clear entries
                entries.clear();
            }
        }
        // create last mapper
        mappers.push_back(NumberMapper{mapname.first, mapname.second, entries});

        // now we have the seeds and all maps
        std::string start{"seed"};
        std::string goal{"location"};
        while (start != goal) {
            start = translateNumbers(mappers, start, seeds);
        }

        // seeds now contains the locations
        // what is the lowest location number that corresponds to any of the initial seed numbers?
        auto result = std::min_element(seeds.begin(), seeds.end());
        return *result;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

std::vector<std::pair<int64_t, int64_t>> extractSeedRanges(const std::string_view& line) {
    auto seednumbers{utils::extractNumbers<int64_t>(line.substr(7))};
    std::vector<std::pair<int64_t, int64_t>> seedpairs;

    for (size_t i = 0; i < seednumbers.size(); i += 2) {
        seedpairs.emplace_back(seednumbers[i], seednumbers[i + 1]);
    }

    return seedpairs;
}

const NumberMapper& getMapper(const std::vector<NumberMapper>& mappers, std::string sourcename) {
    return *std::find_if(mappers.begin(), mappers.end(), [sourcename](const NumberMapper& mapper) { return mapper.source_ == sourcename; });
}

// should use something like interval/segment tree, currently VERY slow
int64_t solution_two() {
    try {
        std::vector<std::pair<int64_t, int64_t>> seedpairs;
        std::vector<NumberMapper> mappers;
        bool readingMap{false};
        std::pair<std::string, std::string> mapname;
        std::vector<MapEntry> entries;

        for (const std::string_view line : input::inputContent) {
            // first line should contain the seeds
            if (seedpairs.empty() and isSeedLine(line)) {
                seedpairs = extractSeedRanges(line);
            } else if (!readingMap and isMapNameLine(line)) {
                mapname = extractMapName(line);
                readingMap = true;
            } else if (readingMap and isMapEntry(line)) {
                entries.push_back(extractEntry(line));
            } else if (readingMap and line.empty()) {
                readingMap = false;
                mappers.push_back(NumberMapper{
                    mapname.first, mapname.second, entries});
                // clear entries
                entries.clear();
            }
        }
        // create last mapper
        mappers.push_back(NumberMapper{mapname.first, mapname.second, entries});

        // now we have the seeds and all maps
        std::string source{"seed"};
        std::string goal{"location"};
        std::vector<int64_t> results;
        uint64_t i;
        for (const auto& [seedstart, seedlength] : seedpairs) {
            std::vector<int64_t> seedrange(seedlength);
            for (uint64_t i = 0; i < seedlength; ++i) {
                seedrange[i] = seedstart + i;
            }
            while (source != goal) {
                auto mapper = getMapper(mappers, source);
                for (uint64_t i = 0; i < seedlength; ++i) {
                    seedrange[i] = mapper.mapNumber(seedrange[i]);
                }
                source = mapper.destination_;
            }
            auto temp = *std::min_element(seedrange.begin(), seedrange.end());
            std::cout << "minimum element of the seed range: " << temp << '\n';
            results.push_back(temp);
        }

        // seeds now contains the locations
        // what is the lowest location number that corresponds to any of the initial seed numbers?
        return *std::min_element(results.begin(), results.end());
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