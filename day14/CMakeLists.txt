add_executable(day14 main.cpp)
target_link_libraries(day14 PRIVATE common)
target_include_directories(day14 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
