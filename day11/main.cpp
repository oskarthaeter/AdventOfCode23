#include <bitset>
#include <utility>

#include "FileData.hpp"
#include "utils.hpp"

namespace original {

constexpr size_t WIDTH{140};  // 10 or 140
constexpr size_t HEIGHT{WIDTH};

struct point {
    long x;
    long y;
};

long galaxyDistances(const std::vector<point>& galaxies) {
    long result{0};

    for (size_t i = 0; i < galaxies.size(); ++i) {
        for (size_t j = i + 1; j < galaxies.size(); ++j) {
            result += std::abs(galaxies[i].x - galaxies[j].x) + std::abs(galaxies[i].y - galaxies[j].y);
        }
    }

    return result;
}

long solution_one() {
    try {
        auto emptyColumns = std::move(std::bitset<WIDTH>{}.set());
        std::vector<point> galaxies;

        size_t x{0};  // width counter
        size_t y{0};  // height counter
        bool emptyRow;
        size_t emptyRows{0};
        for (const std::string_view line : input::inputContent) {
            emptyRow = true;
            x = 0;
            for (char c : line) {
                // is the current char c a galaxy?
                if (c == '#') {
                    galaxies.emplace_back(x, y + emptyRows);
                    emptyRow = false;
                    emptyColumns[x] = false;
                }
                ++x;
            }
            ++y;
            emptyRows += emptyRow;
        }

        for (point& galaxy : galaxies) {
            galaxy.x += (emptyColumns << (WIDTH - galaxy.x)).count();
        }

        return galaxyDistances(galaxies);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

long solution_two() {
    try {
        auto emptyColumns = std::move(std::bitset<WIDTH>{}.set());
        std::vector<point> galaxies;

        size_t x{0};  // width counter
        size_t y{0};  // height counter
        bool emptyRow;
        size_t emptyRows{0};
        for (const std::string_view line : input::inputContent) {
            emptyRow = true;
            x = 0;
            for (char c : line) {
                // is the current char c a galaxy?
                if (c == '#') {
                    galaxies.emplace_back(x, y + (emptyRows * 999'999));
                    emptyRow = false;
                    emptyColumns[x] = false;
                }
                ++x;
            }
            ++y;
            emptyRows += emptyRow;
        }

        for (point& galaxy : galaxies) {
            galaxy.x += (emptyColumns << (WIDTH - galaxy.x)).count() * 999'999;
        }

        return galaxyDistances(galaxies);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}
}  // namespace original

int main() {
    const size_t n{1000};

    std::cout << "Solution one" << std::endl;
    utils::benchmark<n>(original::solution_one);

    std::cout << "\nSolution two" << std::endl;
    utils::benchmark<n>(original::solution_two);

    return 0;
}
