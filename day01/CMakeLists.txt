add_executable(day01 main.cpp)

target_link_libraries(day01 PRIVATE common)

add_custom_command(
    TARGET day01 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/day01/input.txt
    ${CMAKE_CURRENT_BINARY_DIR}/input.txt)
