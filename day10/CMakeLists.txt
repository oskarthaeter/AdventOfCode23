add_executable(day10 main.cpp)
target_link_libraries(day10 PRIVATE common)
target_include_directories(day10 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
