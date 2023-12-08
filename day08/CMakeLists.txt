add_executable(day08 main.cpp)
target_link_libraries(day08 PRIVATE common)
target_include_directories(day08 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
