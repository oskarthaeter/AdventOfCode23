#include <iomanip>
#include <optional>
#include <stack>

#include "FileData.hpp"
#include "utils.hpp"

namespace original {
constexpr size_t WIDTH{140};   // 11 or 140
constexpr size_t HEIGHT{140};  // 9 or 140

enum class Direction : uint8_t {
    North,
    East,
    South,
    West
};

enum class Tile_t : uint8_t { ground,
                              vPipe,
                              hPipe,
                              NEBend,
                              NWBend,
                              SEBend,
                              SWBend };

struct Tile {
    Tile_t type;
    std::optional<size_t> distance;
};

struct PipeMap {
    std::array<std::array<Tile, WIDTH>, HEIGHT> tiles;
    std::pair<size_t, size_t> start;

    PipeMap(const std::array<std::string, HEIGHT>&);
};

std::string directionToStr(const Direction& dir) {
    switch (dir) {
        case Direction::North:
            return "North";
        case Direction::East:
            return "East";
        case Direction::South:
            return "South";
        case Direction::West:
            return "West";
        default:
            return "Unknown";  // Assuming '.' represents the ground
    }
}

Tile charToTile(const char& c) {
    switch (c) {
        case '|':
            return Tile{Tile_t::vPipe, std::nullopt};
        case '-':
            return Tile{Tile_t::hPipe, std::nullopt};
        case 'L':
            return Tile{Tile_t::NEBend, std::nullopt};
        case 'J':
            return Tile{Tile_t::NWBend, std::nullopt};
        case '7':
            return Tile{Tile_t::SWBend, std::nullopt};
        case 'F':
            return Tile{Tile_t::SEBend, std::nullopt};
        default:
            return Tile{Tile_t::ground, std::nullopt};
    }
}

std::string tileToStr(const Tile& tile) {
    switch (tile.type) {
        case Tile_t::vPipe:
            return "|";
        case Tile_t::hPipe:
            return "-";
        case Tile_t::NEBend:
            return "L";
        case Tile_t::NWBend:
            return "J";
        case Tile_t::SEBend:
            return "F";
        case Tile_t::SWBend:
            return "7";
        default:
            return ".";  // Assuming "." represents the ground
    }
}

bool isTileConnected(const std::array<std::array<Tile, WIDTH>, HEIGHT>& tiles, size_t x, size_t y, Direction dir) {
    switch (dir) {
        case Direction::North:
            return y > 0 && (tiles[y - 1][x].type == Tile_t::SWBend || tiles[y - 1][x].type == Tile_t::SEBend || tiles[y - 1][x].type == Tile_t::vPipe);
        case Direction::East:
            return x < WIDTH - 1 && (tiles[y][x + 1].type == Tile_t::NWBend || tiles[y][x + 1].type == Tile_t::SWBend || tiles[y][x + 1].type == Tile_t::hPipe);
        case Direction::South:
            return y < HEIGHT - 1 && (tiles[y + 1][x].type == Tile_t::NEBend || tiles[y + 1][x].type == Tile_t::NWBend || tiles[y + 1][x].type == Tile_t::vPipe);
        case Direction::West:
            return x > 0 && (tiles[y][x - 1].type == Tile_t::NEBend || tiles[y][x - 1].type == Tile_t::SEBend || tiles[y][x - 1].type == Tile_t::hPipe);
        default:
            return false;
    }
}

Tile determineStartTile(const std::array<std::array<Tile, WIDTH>, HEIGHT>& tiles, std::pair<size_t, size_t> start) {
    // determine the type of tile at tiles[start.second][start.first] based on surrounding tiles
    // check north, east, south, west; assume only 2 of these directions are connected
    // i.e. if tile in north is a Tile_t::SWBend, Tile_t::SEBend or vPipe
    size_t x = start.first;
    size_t y = start.second;

    // Check the surrounding tiles
    bool north = isTileConnected(tiles, x, y, Direction::North);
    bool east = isTileConnected(tiles, x, y, Direction::East);
    bool south = isTileConnected(tiles, x, y, Direction::South);
    bool west = isTileConnected(tiles, x, y, Direction::West);

    // Determine the start tile type based on the connected directions
    if (north && west) return Tile{Tile_t::NWBend, std::optional<size_t>(0)};
    if (north && east) return Tile{Tile_t::NEBend, std::optional<size_t>(0)};
    if (south && west) return Tile{Tile_t::SWBend, std::optional<size_t>(0)};
    if (south && east) return Tile{Tile_t::SEBend, std::optional<size_t>(0)};
    if (north && south) return Tile{Tile_t::vPipe, std::optional<size_t>(0)};
    if (east && west) return Tile{Tile_t::hPipe, std::optional<size_t>(0)};

    throw std::runtime_error("Start must be a path tile, can't be ground");
}

PipeMap::PipeMap(const std::array<std::string, HEIGHT>& inputContent) {
    size_t x{0};
    size_t y{0};
    for (std::string_view line : inputContent) {
        x = 0;
        for (const char c : line) {
            if (c == 'S') {
                start = std::make_pair(x, y);
            } else {
                tiles[y][x] = charToTile(c);
            }
            ++x;
        }
        ++y;
    }
    tiles[start.second][start.first] = determineStartTile(tiles, start);
}

Direction nextDirection(Direction previous, Tile* currentTile) {
    switch (currentTile->type) {
        case Tile_t::vPipe:
            // previous can only be North or South, which we want to maintain
            return previous;

        case Tile_t::hPipe:
            // previous can only be East or West, which we want to maintain
            return previous;

        case Tile_t::NEBend:
            return (previous == Direction::South) ? Direction::East : (previous == Direction::West) ? Direction::North
                                                                                                    : throw std::runtime_error("Invalid direction");

        case Tile_t::NWBend:
            return (previous == Direction::South) ? Direction::West : (previous == Direction::East) ? Direction::North
                                                                                                    : throw std::runtime_error("Invalid direction");

        case Tile_t::SWBend:
            return (previous == Direction::North) ? Direction::West : (previous == Direction::East) ? Direction::South
                                                                                                    : throw std::runtime_error("Invalid direction");

        case Tile_t::SEBend:
            return (previous == Direction::North) ? Direction::East : (previous == Direction::West) ? Direction::South
                                                                                                    : throw std::runtime_error("Invalid direction");
        default:
            throw std::runtime_error("Unknown tile type");
    }
    throw std::runtime_error("Could not determine next direction");
}

std::pair<size_t, size_t> nextPosition(std::pair<size_t, size_t> pos, Direction dir) {
    switch (dir) {
        case Direction::North:
            // Assuming pos.second > 0 to prevent underflow
            return std::make_pair(pos.first, pos.second - 1);
        case Direction::East:
            // Assuming pos.first < WIDTH - 1 for boundary check
            return std::make_pair(pos.first + 1, pos.second);
        case Direction::South:
            // Assuming pos.second < HEIGHT - 1 for boundary check
            return std::make_pair(pos.first, pos.second + 1);
        case Direction::West:
            // Assuming pos.first > 0 to prevent underflow
            return std::make_pair(pos.first - 1, pos.second);
        default:
            throw std::invalid_argument("Unknown direction");
    }
}

std::pair<Direction, Direction> startDirections(PipeMap& map) {
    auto startTile = &map.tiles[map.start.second][map.start.first];
    switch (startTile->type) {
        case Tile_t::vPipe:
            return std::make_pair(Direction::North, Direction::South);
        case Tile_t::hPipe:
            return std::make_pair(Direction::East, Direction::West);
        case Tile_t::NEBend:
            return std::make_pair(Direction::North, Direction::East);
        case Tile_t::NWBend:
            return std::make_pair(Direction::North, Direction::West);
        case Tile_t::SWBend:
            return std::make_pair(Direction::South, Direction::West);
        case Tile_t::SEBend:
            return std::make_pair(Direction::South, Direction::East);
        default:
            throw std::invalid_argument("Start tile can't be ground");
    }
}

struct SearchState {
    Direction dir;
    std::pair<size_t, size_t> pos;
    Tile* current;
    size_t distance;
};

std::string stateToStr(const SearchState& state) {
    std::ostringstream oss;
    oss << "Direction: " << directionToStr(state.dir)
        << ", Position: (" << state.pos.first << ", " << state.pos.second << ")"
        << ", Current Tile: " << tileToStr(*state.current)
        << ", Distance: " << state.distance;
    return oss.str();
}

size_t twoProngedDFS(PipeMap& map) {
    std::pair<Direction, Direction> dirs{startDirections(map)};

    auto onePos = nextPosition(map.start, dirs.first);
    SearchState one{nextDirection(dirs.first, &map.tiles[onePos.second][onePos.first]), onePos, &map.tiles[onePos.second][onePos.first], 1uz};

    auto twoPos = nextPosition(map.start, dirs.second);
    SearchState two{nextDirection(dirs.second, &map.tiles[twoPos.second][twoPos.first]), twoPos, &map.tiles[twoPos.second][twoPos.first], 1uz};

    while (true) {
        // does the headTile already have a distance set?
        //      if not -> has not been visited yet
        if (not one.current->distance.has_value()) {
            // process current tile before getting the next
            one.current->distance = one.distance;

            one.pos = nextPosition(one.pos, one.dir);
            one.current = &map.tiles[one.pos.second][one.pos.first];
            one.dir = nextDirection(one.dir, one.current);
            ++one.distance;
        } else
            break;

        // check whether updated one and two are now looking at the same tile
        if (one.pos == two.pos) {
            if (not one.current->distance.has_value()) {
                one.current->distance = one.distance;
            }
            // there are two tiles with the furthest distance from the start
            break;
        }
        if (not two.current->distance.has_value()) {
            // process current tile before getting the next
            two.current->distance = two.distance;

            two.pos = nextPosition(two.pos, two.dir);
            two.current = &map.tiles[two.pos.second][two.pos.first];
            two.dir = nextDirection(two.dir, two.current);
            ++two.distance;
        } else
            break;

        // check whether one and two are now looking at the same tile
        if (one.pos == two.pos) {
            if (not one.current->distance.has_value()) {
                one.current->distance = one.distance;
            }
            break;
        }
    }

    return std::min(one.distance, two.distance);
}

uint64_t solution_one() {
    try {
        PipeMap map = PipeMap(input::inputContent);

        return twoProngedDFS(map);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

size_t count_interior(const std::array<std::array<Tile, WIDTH>, HEIGHT>& map) {
    size_t result{0};
    size_t wallCount;
    size_t interiorCount;
    for (std::array<Tile, WIDTH> row : map) {
        wallCount = 0;
        interiorCount = 0;
        for (Tile tile : row) {
            if (not tile.distance.has_value() and wallCount % 2 == 1) {
                ++interiorCount;
            } else if (tile.distance.has_value() and (tile.type == Tile_t::vPipe or tile.type == Tile_t::SEBend or tile.type == Tile_t::SWBend)) {
                ++wallCount;
            }
        }
        result += interiorCount;
    }
    return result;
}

uint64_t solution_two() {
    try {
        PipeMap map = PipeMap(input::inputContent);
        twoProngedDFS(map);

        return count_interior(map.tiles);
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
