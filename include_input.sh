#!/bin/bash

# Check if a day number was provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <day_number>"
    exit 1
fi

# Assign the day number to a variable
DAY_NUMBER=$(printf "%02d" $1)
SOURCE_DIR="day$DAY_NUMBER"

HEADER_FILE="FileData.hpp"

process_file() {
    local INPUT_FILE="$1"
    local VAR_PREFIX="${INPUT_FILE%.txt}" # Prefix for variables

    local file_contents_raw=""
    local file_contents_array=""
    local line_count=0

    while IFS= read -r line || [ -n "$line" ]; do
        local escaped_line=$(echo "$line" | sed 's/"/\\"/g')
        file_contents_raw="${file_contents_raw}${escaped_line}\\n"
        file_contents_array="${file_contents_array}R\"(${escaped_line})\"sv,"
        ((line_count++))
    done < "$SOURCE_DIR/$INPUT_FILE"

    # Remove the last comma from the vector and array
    file_contents_array=${file_contents_array%,}

    # Append the processed content to the header file
    {
        echo "namespace input {"
        echo "using namespace std::literals::string_view_literals;"
        echo ""
        echo "constexpr char ${VAR_PREFIX}Raw[] = R\"(${file_contents_raw})\";"
        echo ""
        echo "constexpr inline const std::array<std::string_view, ${line_count}> ${VAR_PREFIX}Content = {"
        echo "    ${file_contents_array}"
        echo "};"
        echo ""
        echo "}"
        echo ""
    } >> "$SOURCE_DIR/$HEADER_FILE"
}

# Create or clear the header file
> "$SOURCE_DIR/$HEADER_FILE"

# Add necessary includes
echo "#pragma once" >> "$SOURCE_DIR/$HEADER_FILE"
echo "#include <string_view>" >> "$SOURCE_DIR/$HEADER_FILE"
echo "#include <array>" >> "$SOURCE_DIR/$HEADER_FILE"
echo "" >> "$SOURCE_DIR/$HEADER_FILE"

# Process each file and append its content to the header file
process_file "input.txt"

echo "$HEADER_FILE generated in $SOURCE_DIR"