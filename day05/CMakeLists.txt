add_executable(day05 main.cpp)
target_link_libraries(day05 PRIVATE common)
target_include_directories(day05 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
