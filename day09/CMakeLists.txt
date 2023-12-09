add_executable(day09 main.cpp)
target_link_libraries(day09 PRIVATE common)
target_include_directories(day09 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
