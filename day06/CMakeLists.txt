add_executable(day06 main.cpp)
target_link_libraries(day06 PRIVATE common)
target_include_directories(day06 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
