add_executable(day02 main.cpp)
target_link_libraries(day02 PRIVATE common)
target_include_directories(day02 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
