add_executable(day04 main.cpp)
target_link_libraries(day04 PRIVATE common)
target_include_directories(day04 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})