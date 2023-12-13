add_executable(day13 main.cpp)
target_link_libraries(day13 PRIVATE common)
target_include_directories(day13 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
