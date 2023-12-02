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

void benchmark(std::function<void()>, const size_t);
}