add_executable(day04 main.cpp)

target_link_libraries(day04 PRIVATE common)

add_custom_command(
    TARGET day04 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/day04/input.txt
    ${CMAKE_CURRENT_BINARY_DIR}/input.txt)
