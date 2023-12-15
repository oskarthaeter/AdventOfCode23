add_executable(day15 main.cpp)
target_link_libraries(day15 PRIVATE common)
target_include_directories(day15 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
