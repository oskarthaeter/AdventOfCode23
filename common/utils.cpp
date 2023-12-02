#include "utils.hpp"

namespace utils {
LineIterator::LineIterator() : isEnd(true) {}  // End iterator
LineIterator::LineIterator(const std::string& filePath)
    : file(std::make_shared<std::ifstream>(filePath)), isEnd(false) {
    if (!file->is_open()) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }
    ++(*this);
}

LineIterator& LineIterator::operator++() {
    if (!getline(*file, currentLine)) {
        isEnd = true;
        file->close();
    }
    return *this;
}

bool LineIterator::operator!=(const LineIterator& other) const {
    return isEnd != other.isEnd;
}

const std::string& LineIterator::operator*() const {
    return currentLine;
}

std::string_view LineIterator::currentLineView() const {
    return std::string_view(currentLine);
}

LineIterator LineIterator::begin() {
    return *this;
}

LineIterator LineIterator::end() {
    return LineIterator();
}

void benchmark(std::function<void()> code, const size_t n) {
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point stop;

    std::vector<std::chrono::microseconds> durations;
    std::chrono::microseconds duration;

    for (size_t i = 0; i < n; ++i) {
        start = std::chrono::high_resolution_clock::now();

        code();

        stop = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        durations.emplace_back(duration);
    }

    // Calculate average, min, and max durations
    std::chrono::microseconds totalDuration = std::accumulate(durations.begin(), durations.end(), std::chrono::microseconds(0));
    std::chrono::microseconds avgDuration = totalDuration / n;
    std::chrono::microseconds minDuration = *std::min_element(durations.begin(), durations.end());
    std::chrono::microseconds maxDuration = *std::max_element(durations.begin(), durations.end());

    // Output stats
    std::cout << "Average Duration: " << avgDuration.count() << " microseconds\n";
    std::cout << "Min Duration: " << minDuration.count() << " microseconds\n";
    std::cout << "Max Duration: " << maxDuration.count() << " microseconds\n";
}
}  // namespace utils