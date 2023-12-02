#!/bin/bash

# Check if a day number was provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <day_number>"
    exit 1
fi

# Assign the day number to a variable
DAY_NUMBER=$(printf "%02d" $1)
DAY_DIR="day$DAY_NUMBER"

# Check if the directory already exists
if [ -d "$DAY_DIR" ]; then
    echo "Directory $DAY_DIR already exists!"
    exit 1
fi

# Create the day directory
mkdir "$DAY_DIR"
echo "Created directory: $DAY_DIR"

# Create a basic CMakeLists.txt file for the day
cat << EOF > "$DAY_DIR/CMakeLists.txt"
add_executable(day$DAY_NUMBER main.cpp)

target_link_libraries(day$DAY_NUMBER PRIVATE common)

add_custom_command(
    TARGET day$DAY_NUMBER POST_BUILD
    COMMAND \${CMAKE_COMMAND} -E copy
    \${CMAKE_SOURCE_DIR}/day$DAY_NUMBER/input.txt
    \${CMAKE_CURRENT_BINARY_DIR}/input.txt)
EOF

# Create a template main.cpp file
cat << EOF > "$DAY_DIR/main.cpp"
#include "utils.hpp"

namespace original {
uint64_t solution_one() {
    try {
        utils::LineIterator it("input.txt");
        uint64_t resultSum{0};

        for (; it != utils::LineIterator(); ++it) {
            std::string_view line = it.currentLineView();
        }
        return resultSum;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

uint64_t solution_two() {
    try {
        utils::LineIterator it("input.txt");
        uint64_t resultSum{0};

        for (; it != utils::LineIterator(); ++it) {
            std::string_view line = it.currentLineView();
        }
        return resultSum;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}
} // namespace original

int main() {
    const size_t n{100};

    std::cout << "Solution one" << std::endl;
    utils::benchmark(original::solution_one, n);

    std::cout << "\nSolution two" << std::endl;
    utils::benchmark(original::solution_two, n);

    return 0;
}
EOF

# Create an empty task.md file
touch "$DAY_DIR/task.md"

# Create an empty input.txt file
touch "$DAY_DIR/input.txt"

# Main CMakeLists.txt file path
MAIN_CMAKE="CMakeLists.txt"

# Marker and new day's add_subdirectory line
MARKER="# ADD SUBDIRECTORY"
ADD_SUB="add_subdirectory($DAY_DIR)"

# Check if the marker exists
if ! grep -q "$MARKER" "$MAIN_CMAKE"; then
    echo "Marker not found in $MAIN_CMAKE. Please add the marker or manually update the file."
    exit 1
fi

# Split the file at the marker, sort and insert the new day, then concatenate back
awk -v add_sub="$ADD_SUB" -v marker="$MARKER" -v day_num="$DAY_NUMBER" '
    BEGIN { print_marker=0; count=0; inserted=0; }
    $0 ~ marker { print; print_marker=1; next }
    print_marker && /^add_subdirectory\(day[0-9]{2}\)/ {
        if ($2 >= "day" day_num ")" && !inserted) {
            print add_sub;
            inserted=1;
        }
        print $0;
        next;
    }
    print_marker && !/^add_subdirectory\(day[0-9]{2}\)/ {
        if (!inserted) {
            print add_sub;
            inserted=1;
        }
        print_marker=0;
    }
    { print }
' "$MAIN_CMAKE" > tmp && mv tmp "$MAIN_CMAKE"

echo "Day $DAY_NUMBER setup complete."
