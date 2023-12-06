#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace utils {
class LineIterator {
   private:
    std::shared_ptr<std::ifstream> file;
    std::string currentLine;
    bool isEnd;

   public:
    LineIterator();
    explicit LineIterator(const std::string& filePath);

    LineIterator& operator++();
    bool operator!=(const LineIterator& other) const;
    const std::string& operator*() const;
    std::string_view currentLineView() const;

    LineIterator begin();
    LineIterator end();
};

std::vector<std::string> readLinesFromFile(const std::string&);

template <size_t N>
std::array<std::string, N> readLinesFromFile(const std::string& filename) {
    std::array<std::string, N> lines;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return lines;
    }

    size_t counter{0};
    std::string line;
    while (std::getline(file, line) and counter < N) {
        lines[counter++] = line;
    }

    file.close();
    return lines;
}

template <size_t N>
void benchmark(std::function<void()> code) {
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point stop;

    std::vector<std::chrono::microseconds> durations;
    std::chrono::microseconds duration;

    for (size_t i = 0; i < N; ++i) {
        start = std::chrono::high_resolution_clock::now();

        code();

        stop = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        durations.emplace_back(duration);
    }

    // Calculate average, min, and max durations
    std::chrono::microseconds totalDuration = std::accumulate(durations.begin(), durations.end(), std::chrono::microseconds(0));
    std::chrono::microseconds avgDuration = totalDuration / N;
    std::chrono::microseconds minDuration = *std::min_element(durations.begin(), durations.end());
    std::chrono::microseconds maxDuration = *std::max_element(durations.begin(), durations.end());

    // Output stats
    std::cout << "Average Duration: " << avgDuration.count() << " microseconds\n";
    std::cout << "Min Duration: " << minDuration.count() << " microseconds\n";
    std::cout << "Max Duration: " << maxDuration.count() << " microseconds\n";
}

template <typename T>
std::vector<T> extractNumbers(const std::string_view& line) {
    static_assert(std::is_integral<T>::value, "Template type T must be an integral type.");
    std::vector<T> numbers;
    auto head = line.begin();
    auto end = line.end();

    T number;
    while (head != end) {
        if (*head == ' ') {
            // skip whitespace
            ++head;
            continue;
        } else {
            // Parse the number
            auto [ptr, ec] = std::from_chars(head, end, number);
            if (ec == std::errc()) {
                numbers.push_back(number);
            } else {
                std::cout << line;
                throw std::runtime_error("Parsing error occurred");
            }
            // Advance to the next part of the string
            head = ptr;
        }
    }
    return numbers;
}

template <typename T, size_t N>
std::array<T, N> extractNumbers(const std::string_view& line) {
    static_assert(std::is_integral<T>::value, "Template type T must be an integral type.");
    std::array<T, N> numbers{};
    auto head = line.begin();
    auto end = line.end();

    size_t counter{0};
    T number;
    while (head != end and counter < N) {
        if (*head == ' ') {
            // skip whitespace
            ++head;
            continue;
        } else {
            // Parse the number
            auto [ptr, ec] = std::from_chars(head, end, number);
            if (ec == std::errc()) {
                numbers[counter++] = number;
            } else {
                std::cout << line;
                throw std::runtime_error("Parsing error occurred");
            }
            // Advance to the next part of the string
            head = ptr;
        }
    }
    return numbers;
}

template <typename T>
std::string vectorToString(const std::vector<T>& vec) {
    if (vec.empty()) {
        return "[]";
    }

    std::ostringstream oss;
    oss << "[";

    for (size_t i = 0; i < vec.size(); ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << vec[i];
    }

    oss << "]";
    return oss.str();
}

}  // namespace utils