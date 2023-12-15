#include <unordered_map>

#include "FileData.hpp"
#include "utils.hpp"

namespace original {

constexpr size_t WIDTH{input::inputContent.size()};
constexpr size_t HEIGHT{WIDTH};

typedef std::array<std::array<char, HEIGHT>, WIDTH> Dish;
typedef std::array<std::array<char, WIDTH>, HEIGHT> Platform;

void tiltColumn(std::array<char, HEIGHT>& col) {
    int lastOIndex = -1;  // Track the position of the last moved 'O'

    for (int i = 0; i < HEIGHT; ++i) {
        if (col[i] == 'O') {
            col[i] = '.';             // Clear the current position
            col[++lastOIndex] = 'O';  // Move 'O' to the next available position
        } else if (col[i] == '#') {
            lastOIndex = i;  // Update lastOIndex to current position if it's a '#'
        }
    }
}

uint calculateColLoad(const std::array<char, HEIGHT>& col) {
    uint result{0};
    for (int i{0}; i < HEIGHT; ++i) {
        if (col[i] == 'O') {
            result += HEIGHT - i;
        }
    }
    return result;
}

uint64_t solution_one() {
    try {
        uint64_t result{0};
        Dish dish{};

        size_t x{0};
        size_t y{0};
        for (const std::string_view line : input::inputContent) {
            for (x = 0; x < WIDTH; ++x) {
                dish[x][y] = line[x];
            }
            ++y;
        }
        for (size_t i{0}; i < WIDTH; ++i) {
            tiltColumn(dish[i]);
            result += calculateColLoad(dish[i]);
        }

        return result;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

void runCycle(Platform& platform) {
    // Tilt NORTH
    bool changeMade = true;
    while (changeMade) {
        changeMade = false;

        for (size_t x{0}; x < WIDTH; ++x) {
            for (size_t y{0}; y < HEIGHT - 1; ++y) {
                auto& curr = platform[y][x];
                auto& above = platform[y + 1][x];
                if (curr == '.' and above == 'O') {
                    std::swap(curr, above);
                    changeMade = true;
                }
            }
        }
    }

    // Tilt WEST
    changeMade = true;
    while (changeMade) {
        changeMade = false;

        for (size_t y{0}; y < HEIGHT; ++y) {
            for (size_t x{0}; x < WIDTH - 1; ++x) {
                auto& curr = platform[y][x];
                auto& right = platform[y][x + 1];
                if (curr == '.' and right == 'O') {
                    std::swap(curr, right);
                    changeMade = true;
                }
            }
        }
    }

    // Tilt SOUTH
    changeMade = true;
    while (changeMade) {
        changeMade = false;

        for (size_t x{0}; x < WIDTH; ++x) {
            for (size_t y = HEIGHT - 1; y > 0; --y) {
                auto& curr = platform[y][x];
                auto& below = platform[y - 1][x];
                if (curr == '.' and below == 'O') {
                    std::swap(curr, below);
                    changeMade = true;
                }
            }
        }
    }

    // Tilt EAST
    changeMade = true;
    while (changeMade) {
        changeMade = false;

        for (size_t y{0}; y < HEIGHT; ++y) {
            for (size_t x = WIDTH - 1; x > 0; --x) {
                auto& curr = platform[y][x];
                auto& left = platform[y][x - 1];
                if (curr == '.' and left == 'O') {
                    std::swap(curr, left);
                    changeMade = true;
                }
            }
        }
    }
}

std::string stringify(const Platform& platform) {
    std::string result;
    for (const auto& row : platform) {
        for (char c : row) {
            result += c;
        }
    }
    return result;
}

uint64_t calcLoad(const Platform& platform) {
    uint64_t totalLoad = 0;
    for (size_t y{0}; y < HEIGHT; ++y) {
        for (size_t x{0}; x < WIDTH; ++x) {
            if (platform[y][x] == 'O') {
                totalLoad += HEIGHT - y;
            }
        }
    }
    return totalLoad;
}

uint64_t solve(Platform platform) {
    std::unordered_map<std::string, std::pair<size_t, uint64_t>> mapping;

    size_t i = 0;
    size_t periodStart = 0;
    size_t periodEnd = 0;

    constexpr size_t target = 1'000'000'000;
    std::string key;
    while (i < target) {
        runCycle(platform);
        ++i;

        key = stringify(platform);

        auto it = mapping.find(key);
        if (it != mapping.end()) {
            periodStart = it->second.first;
            periodEnd = i;
            break;
        } else {
            mapping[key] = {i, calcLoad(platform)};
        }
    }

    const size_t period = periodEnd - periodStart;
    const size_t cycle = target - periodStart;

    const size_t sameLoadIndex = periodStart + cycle % period;

    auto it = std::find_if(mapping.begin(), mapping.end(),
                           [sameLoadIndex](const auto& pair) {
                               return pair.second.first == sameLoadIndex;
                           });

    return it != mapping.end() ? it->second.second : 0;
}

uint64_t solution_two() {
    try {
        uint64_t result{0};
        Platform platform{};

        size_t x{0};
        size_t y{0};
        for (const std::string_view line : input::inputContent) {
            for (x = 0; x < WIDTH; ++x) {
                platform[y][x] = line[x];
            }
            ++y;
        }

        result = solve(platform);

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
