add_executable(day11 main.cpp)
target_link_libraries(day11 PRIVATE common)
target_include_directories(day11 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
