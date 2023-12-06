#pragma once
#include <array>
#include <string>
#include <vector>

#include "utils.hpp"

namespace input {
constexpr char rawContent[] = R"(Time:        40     81     77     72\nDistance:   219   1012   1365   1089\n)";

inline const std::array<std::string, 2> content = {
    "Time:        40     81     77     72", "Distance:   219   1012   1365   1089"};
}