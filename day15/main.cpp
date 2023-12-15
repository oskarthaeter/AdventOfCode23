#include <list>
#include <optional>

#include "FileData.hpp"
#include "utils.hpp"

namespace original {

uint8_t hash(std::string_view s) {
    uint8_t currentValue = 0;

    for (char c : s) {
        // Increase by ASCII code of current character
        currentValue += static_cast<uint8_t>(c);

        // Multiply by 17
        currentValue *= 17;

        // Modulo not neccessary as uin8_t wraps around automatically
    }

    return currentValue;
}

uint64_t hashOnTheFly(std::string_view data) {
    uint64_t result{0};
    uint8_t running{0};

    size_t start = 0;  // Start position of the next value
    for (size_t i = 0; i <= data.size(); ++i) {
        // Check for the end of a value or the end of the input
        if (i == data.size() || data[i] == ',') {
            // If there's a value to add (non-empty)
            if (i != start) {
                // result += hash(data.substr(start, i - start));
                result += running;
                running = 0;
            }
            start = i + 1;  // Move past the comma
        } else if (data[i] == '\n') {
            // Skip newline characters
            start = i + 1;
        } else {
            running += static_cast<uint8_t>(data[i]);
            running *= 17;
        }
    }

    return result;
}

uint64_t solution_one() {
    try {
        uint64_t result{0};

        result = hashOnTheFly(input::inputContent[0]);

        return result;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

struct Datavalue {
    std::string_view label;
    std::optional<int> focalLength;
};

struct Lens {
    std::string_view label;
    std::uint8_t focalLength;
};

Datavalue parseDatavalue(std::string_view data) {
    // Find the position of the dash or equal sign
    size_t pos = data.find_first_of("-=");

    if (pos == std::string_view::npos || pos == 0) {
        throw std::invalid_argument("Invalid input format");
    }

    std::string_view label = data.substr(0, pos);
    char opChar = data[pos];

    if (opChar == '-') {
        return Datavalue{label, std::nullopt};
    } else if (opChar == '=') {
        // Extract the numeric part and convert to int
        std::string_view numPart = data.substr(pos + 1);
        int value;
        auto [ptr, ec] = std::from_chars(numPart.begin(), numPart.end(), value);
        if (ec != std::errc()) {
            std::cout << numPart << " from " << data << " => " << value << std::endl;
            throw std::runtime_error("Parsing error occurred");
        }
        return Datavalue{label, std::optional(value)};
    } else {
        throw std::invalid_argument("Unknown operation");
    }
}

void processDatavalue(std::array<std::list<Lens>, 256>& boxes, Datavalue dataval) {
    uint8_t h = hash(dataval.label);
    std::list<Lens>& box = boxes[h];
    if (dataval.focalLength.has_value()) {
        // Find the lens with the matching label
        bool found = false;
        for (Lens& lens : box) {
            if (lens.label == dataval.label) {
                // Update the lens
                lens.focalLength = dataval.focalLength.value();
                found = true;
                break;
            }
        }

        // If not found, append to the list
        if (!found) {
            box.push_back(Lens{dataval.label, static_cast<uint8_t>(dataval.focalLength.value())});
        }
    } else {
        box.remove_if([dataval](Lens l) {
            return l.label == dataval.label;
        });
    }
}

uint focusingPower(std::list<Lens>& box) {
    //     The slot number of the lens within the box: 1 for the first lens, 2 for the second lens, and so on.
    //     The focal length of the lens.
    uint result{0};
    size_t slot{1};
    for (const auto& lens : box) {
        result += lens.focalLength * slot++;
    }
    return result;
}

uint focusingPower(std::array<std::list<Lens>, 256>& boxes) {
    //     One plus the box number of the lens in question.

    uint result{0};
    size_t boxnumber{1};
    for (auto& box : boxes) {
        result += focusingPower(box) * boxnumber++;
    }
    return result;
}

std::array<std::list<Lens>, 256> constructBoxes(std::string_view data) {
    std::array<std::list<Lens>, 256> boxes{};

    size_t start = 0;  // Start position of the next value
    for (size_t i = 0; i <= data.size(); ++i) {
        // Check for the end of a value or the end of the input
        if (i == data.size() || data[i] == ',') {
            // If there's a value to add (non-empty)
            if (i != start) {
                processDatavalue(boxes, parseDatavalue(data.substr(start, i - start)));
            }
            start = i + 1;  // Move past the comma
        } else if (data[i] == '\n') {
            // Skip newline characters
            start = i + 1;
        }
    }

    return boxes;
}

uint64_t solution_two() {
    try {
        std::array<std::list<Lens>, 256> boxes = constructBoxes(input::inputContent[0]);
        return focusingPower(boxes);
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
