add_executable(day03 main.cpp)
target_link_libraries(day03 PRIVATE common)
target_include_directories(day03 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
