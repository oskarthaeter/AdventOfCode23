add_executable(day01 main.cpp)
target_link_libraries(day01 PRIVATE common)
target_include_directories(day01 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})