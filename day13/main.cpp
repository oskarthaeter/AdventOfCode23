#include "FileData.hpp"
#include "utils.hpp"

namespace original {

// '#' represents true and '.' represents false
// `rowWise[row][col] = columnWise[col][row]`
struct Pattern {
    std::vector<std::bitset<17>> rowWise;
    std::vector<std::bitset<17>> colWise;
    size_t numRows;
    size_t numCols;

    Pattern(const std::vector<std::string_view>& input) {
        numRows = input.size();
        numCols = input[0].size();

        // Ensure that the number of columns does not exceed 17
        if (numCols > 17) {
            throw std::runtime_error("Pattern exceeds the maximum supported column size (17)");
        }

        rowWise.resize(numRows);
        colWise.resize(numCols);

        for (size_t row = 0; row < numRows; ++row) {
            std::string_view line = input[row];
            for (size_t col = 0; col < numCols; ++col) {
                if (line[col] == '#') {
                    rowWise[row].set(col);  // Set the corresponding bit for the row
                    colWise[col].set(row);  // Set the corresponding bit for the column
                }
            }
        }
    }

    template <uint allowedDiffs>
    uint32_t symmetryResult() {
        uint32_t vertical = analyzeSymmetry<allowedDiffs>(colWise);
        uint32_t horizontal = analyzeSymmetry<allowedDiffs>(rowWise);
        return vertical + (100u * horizontal);
    }

    template <uint allowedDiffs>
    uint32_t analyzeSymmetry(std::vector<std::bitset<17>> data) {
        uint32_t result{0};
        for (auto m = 0; m < data.size() - 1; ++m) {
            uint32_t diffs{0};
            for (auto c0 = m, c1 = m + 1; c0 >= 0 && c1 < data.size(); --c0, ++c1) {
                diffs += (data[c0] ^ data[c1]).count();
            }
            result += (diffs == allowedDiffs) * (m + 1);
        }
        return result;
    }
};

uint64_t solution_one() {
    try {
        uint64_t result{0};
        std::vector<std::string_view> buffer;

        for (const std::string_view line : input::inputContent) {
            if (line.empty()) {
                result += Pattern{buffer}.symmetryResult<1>();
                buffer.clear();
            } else {
                buffer.push_back(line);
            }
        }
        result += Pattern{buffer}.symmetryResult<1>();

        return result;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

uint64_t solution_two() {
    try {
        uint64_t result{0};
        std::vector<std::string_view> buffer;

        for (const std::string_view line : input::inputContent) {
            if (line.empty()) {
                result += Pattern{buffer}.symmetryResult<1>();
                buffer.clear();
            } else {
                buffer.push_back(line);
            }
        }
        result += Pattern{buffer}.symmetryResult<1>();

        return result;
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
