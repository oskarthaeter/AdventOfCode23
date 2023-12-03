add_executable(day03 main.cpp)

target_link_libraries(day03 PRIVATE common)

add_custom_command(
    TARGET day03 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/day03/input.txt
    ${CMAKE_CURRENT_BINARY_DIR}/input.txt)
