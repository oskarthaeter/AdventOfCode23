add_executable(day12 main.cpp)
target_link_libraries(day12 PRIVATE common)
target_include_directories(day12 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
