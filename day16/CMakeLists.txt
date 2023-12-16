add_executable(day16 main.cpp)
target_link_libraries(day16 PRIVATE common)
target_include_directories(day16 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
