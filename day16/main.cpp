#include <bitset>
#include <stack>
#include <unordered_set>

#include "FileData.hpp"
#include "utils.hpp"

namespace original {
constexpr size_t WIDTH{110};
constexpr size_t HEIGHT{WIDTH};

// '.': empty space
// '|': vertical splitter, NORTH-SOUTH if coming from left or right
// '-': horizontal splitter, WEST-EAST if coming from NORTH or SOUTH
// '\': right mirror, turn right if current direction WEST or EAST, turn left if current direction NORTH or SOUTH
// '/': left mirror, turn left if current direction WEST or EAST, turn right if current direction NORTH or SOUTH

enum class Direction : uint8_t { NORTH,
                                 SOUTH,
                                 WEST,
                                 EAST };
enum class tile_t : uint8_t { EMPTY,
                              VSPLITTER,
                              HSPLITTER,
                              RIGHTMIRROR,
                              LEFTMIRROR };

struct Tile {
    tile_t type{tile_t::EMPTY};
    std::bitset<4> visited{0};

    bool checkVisit(Direction dir) const {
        return visited[static_cast<uint8_t>(dir)];
    }

    void addVisit(Direction dir) {
        visited[static_cast<uint8_t>(dir)] = true;
    }
};

struct Coordinate {
    int col{0};
    int row{0};

    [[nodiscard]] bool operator==(const Coordinate& other) const {
        return col == other.col && row == other.row;
    }
    [[nodiscard]] bool valid() const {
        return row >= 0 && row < HEIGHT && col >= 0 && col < WIDTH;
    }
};

struct Beam {
    Coordinate pos{0, -1};
    Direction dir{Direction::EAST};

    [[nodiscard]] Coordinate nextPos() const {
        Coordinate nextPosition{pos};
        switch (dir) {
            case Direction::NORTH:
                --nextPosition.row;
                break;
            case Direction::SOUTH:
                ++nextPosition.row;
                break;
            case Direction::WEST:
                --nextPosition.col;
                break;
            case Direction::EAST:
                ++nextPosition.col;
                break;
        }
        return nextPosition;
    }
};

std::array<std::array<Tile, WIDTH>, HEIGHT> parseInput() {
    using enum tile_t;
    std::array<std::array<Tile, WIDTH>, HEIGHT> grid{};

    for (size_t i{0}; i < HEIGHT; ++i) {
        for (size_t j{0}; j < WIDTH; ++j) {
            Tile tile;
            switch (input::inputContent[i][j]) {
                case '.':
                    tile.type = EMPTY;
                    grid[i][j] = tile;
                    break;
                case '|':
                    tile.type = VSPLITTER;
                    grid[i][j] = tile;
                    break;
                case '-':
                    tile.type = HSPLITTER;
                    grid[i][j] = tile;
                    break;
                case '\\':
                    tile.type = RIGHTMIRROR;
                    grid[i][j] = tile;
                    break;
                case '/':
                    tile.type = LEFTMIRROR;
                    grid[i][j] = tile;
                    break;
                default:
                    throw std::runtime_error("Invalid character in input");
            }
        }
    }

    return grid;
}

void printGrid(const std::array<std::array<Tile, WIDTH>, HEIGHT>& grid) {
    using enum tile_t;
    for (size_t i{0}; i < HEIGHT; ++i) {
        for (size_t j{0}; j < WIDTH; ++j) {
            switch (grid[i][j].type) {
                case EMPTY:
                    std::cout << '.';
                    break;
                case VSPLITTER:
                    std::cout << '|';
                    break;
                case HSPLITTER:
                    std::cout << '-';
                    break;
                case RIGHTMIRROR:
                    std::cout << '\\';
                    break;
                case LEFTMIRROR:
                    std::cout << '/';
                    break;
            }
        }
        std::cout << std::endl;
    }
}

void simulateBeam(const Beam& beamStart, std::array<std::array<Tile, WIDTH>, HEIGHT>& grid) {
    using enum tile_t;
    using enum Direction;

    Beam beam{beamStart};
    std::stack<Beam> beams;
    beams.push(beam);
    while (!beams.empty()) {
        beam = beams.top();
        beams.pop();
        if (beam.pos.valid()) {
            if (grid[beam.pos.row][beam.pos.col].checkVisit(beam.dir))
                continue;
            else {
                grid[beam.pos.row][beam.pos.col].addVisit(beam.dir);
            }
        }

        auto nextCoord = beam.nextPos();
        if (!nextCoord.valid())
            continue;

        const Tile& nextTile = grid[nextCoord.row][nextCoord.col];
        if (nextTile.type == EMPTY)
            beams.emplace(nextCoord, beam.dir);
        else if (nextTile.type == LEFTMIRROR || nextTile.type == RIGHTMIRROR) {
            switch (beam.dir) {
                case NORTH:
                    beams.emplace(nextCoord, (nextTile.type == LEFTMIRROR) ? EAST : WEST);
                    break;
                case SOUTH:
                    beams.emplace(nextCoord, (nextTile.type == LEFTMIRROR) ? WEST : EAST);
                    break;
                case EAST:
                    beams.emplace(nextCoord, (nextTile.type == LEFTMIRROR) ? NORTH : SOUTH);
                    break;
                case WEST:
                    beams.emplace(nextCoord, (nextTile.type == LEFTMIRROR) ? SOUTH : NORTH);
                    break;
            }
        } else {  // splitter
            if ((nextTile.type == HSPLITTER && (beam.dir == WEST || beam.dir == EAST)) || (nextTile.type == VSPLITTER && (beam.dir == NORTH || beam.dir == SOUTH)))
                beams.emplace(nextCoord, beam.dir);
            else if (nextTile.type == HSPLITTER) {
                beams.emplace(nextCoord, WEST);
                beams.emplace(nextCoord, EAST);
            } else {
                beams.emplace(nextCoord, NORTH);
                beams.emplace(nextCoord, SOUTH);
            }
        }
    }
}

uint countBeams(const std::array<std::array<Tile, WIDTH>, HEIGHT>& grid) {
    uint count{0};
    for (const auto& row : grid) {
        for (const auto& tile : row) {
            if (tile.visited.any())
                ++count;
        }
    }
    return count;
}

uint64_t solution_one() {
    try {
        uint64_t result{0};
        std::array<std::array<Tile, WIDTH>, HEIGHT> grid{parseInput()};

        Beam startBeam{{-1, 0}, Direction::EAST};
        simulateBeam(startBeam, grid);
        result = countBeams(grid);
        return result;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

std::array<Beam, 2 * (WIDTH + HEIGHT)> possibleStarts() {
    std::array<Beam, 2 * (WIDTH + HEIGHT)> starts{};
    // we can start from any position at the edge, facing towards the center
    // i.e. from the top row, facing south
    //      from the left column, facing east
    //      from the bottom row, facing north
    //      from the right column, facing west
    size_t index{0};
    for (size_t i{0}; i < WIDTH; ++i) {
        starts[index++] = Beam{{static_cast<int>(i), -1}, Direction::SOUTH};
        starts[index++] = Beam{{static_cast<int>(i), static_cast<int>(HEIGHT)}, Direction::NORTH};
    }
    for (size_t i{0}; i < HEIGHT; ++i) {
        starts[index++] = Beam{{-1, static_cast<int>(i)}, Direction::EAST};
        starts[index++] = Beam{{static_cast<int>(WIDTH), static_cast<int>(i)}, Direction::WEST};
    }
    return starts;
}

uint64_t solution_two() {
    try {
        uint result{0};
        std::array<std::array<Tile, WIDTH>, HEIGHT> grid{parseInput()};
        auto beamStarts{possibleStarts()};

        for (const auto& beamStart : beamStarts) {
            std::array<std::array<Tile, WIDTH>, HEIGHT> gridCopy{grid};
            simulateBeam(beamStart, gridCopy);
            result = std::max(result, countBeams(gridCopy));
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

    std::cout << "Part 1: \n"
              << original::solution_one() << std::endl;
    utils::benchmark<n>(original::solution_one);

    std::cout << "Part 2: " << original::solution_two() << std::endl;
    utils::benchmark<n>(original::solution_two);

    return 0;
}
