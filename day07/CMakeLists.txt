add_executable(day07 main.cpp)
target_link_libraries(day07 PRIVATE common)
target_include_directories(day07 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
