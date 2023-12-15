#include <unordered_map>

#include "FileData.hpp"
#include "utils.hpp"

namespace original {
using namespace std::literals;

struct Node {
    std::string_view left;
    std::string_view right;
};

struct Map {
    std::vector<char> instructions;
    std::unordered_map<std::string_view, Node> nodes;
    std::vector<std::string_view> startNodes;
};

std::vector<char> parseInstructions(std::string_view line) {
    std::vector<char> instructions(line.size());
    for (size_t i{0}; i < line.size(); ++i) {
        instructions[i] = line[i];
    }

    return instructions;
}

std::pair<std::string_view, Node> parseNode(std::string_view line) {
    // example: "AAA = (BBB, CCC)"

    std::string_view name = line.substr(0, 3);
    std::string_view left = line.substr(7, 3);
    std::string_view right = line.substr(12, 3);
    return std::make_pair(name, Node{left, right});
}

Map parseMap() {
    std::unordered_map<std::string_view, Node> nodes;
    std::vector<std::string_view> startNodes;

    auto instructions = parseInstructions(std::string_view(input::inputContent[0]));

    for (auto it = input::inputContent.begin() + 2; it != input::inputContent.end(); ++it) {
        auto [name, node] = parseNode(std::string_view(*it));
        nodes[name] = node;
        if (name[2] == 'A') {
            startNodes.push_back(name);
        }
    }
    return Map{instructions, nodes, startNodes};
}

uint64_t solution_one() {
    try {
        uint64_t result{0};
        Map map = parseMap();
        size_t i{0};
        size_t numInstructions{map.instructions.size()};

        std::string_view current("AAA"sv);
        std::string_view goal("ZZZ"sv);
        // follow instructions
        while (current != goal) {
            auto instruct = map.instructions[i];
            if (instruct == 'R') {
                current = map.nodes[current].right;
            } else if (instruct == 'L') {
                current = map.nodes[current].left;
            }
            i = (i + 1) % numInstructions;
            ++result;
        }
        return result;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

size_t cycle(std::string_view startNode, Map& map) {
    std::string_view current{startNode};
    size_t i{0};
    size_t numInstructions{map.instructions.size()};

    while (current[2] != 'Z') {
        if (map.instructions[i] == 'L')
            current = map.nodes[current].left;
        else
            current = map.nodes[current].right;

        i = (i + 1) % numInstructions;
    }

    std::string_view goal{current};

    // when do we get back to this node?
    size_t steps{0};
    while (current != goal or steps == 0) {
        ++steps;

        if (map.instructions[i] == 'R')
            current = map.nodes[current].right;
        else
            current = map.nodes[current].left;

        i = (i + 1) % numInstructions;
    }

    return steps;
}

size_t lcm(const std::vector<size_t>& numbers) {
    if (numbers.size() < 2) return 0ul;
    size_t result = std::lcm(numbers[0], numbers[1]);
    for (size_t i{2}; i < numbers.size(); ++i)
        result = std::lcm(result, numbers[i]);
    return result;
}

uint64_t solution_two() {
    try {
        Map map = parseMap();

        std::vector<size_t> cycles;
        for (auto start : map.startNodes) {
            cycles.push_back(cycle(start, map));
        }

        return lcm(cycles);
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
