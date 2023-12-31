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
target_include_directories(day$DAY_NUMBER PRIVATE \${CMAKE_CURRENT_BINARY_DIR})
EOF

# Create a template main.cpp file
cat << EOF > "$DAY_DIR/main.cpp"
#include "FileData.hpp"
#include "utils.hpp"

namespace original {
uint64_t solution_one() {
    try {
        uint64_t result{0};

        for (const std::string_view line : input::inputContent) {
            
        }
        return result;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

uint64_t solution_two() {
    try {
        uint64_t result{0};

        for (const std::string_view line : input::inputContent) {
            
        }
        return result;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}
} // namespace original

int main() {
    constexpr size_t n{100};

    std::cout << "Part 1: " << original::solution_one() << std::endl;
    utils::benchmark<n>(original::solution_one);

    std::cout << "Part 2: " << original::solution_two() << std::endl;
    utils::benchmark<n>(original::solution_two);

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
