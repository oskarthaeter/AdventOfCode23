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

std::vector<std::string> readTextFile(const char* filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return lines;
    }

    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();
    return lines;
}

}  // namespace utils