add_executable(day02 main.cpp)

target_link_libraries(day02 PRIVATE common)

add_custom_command(
    TARGET day02 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/day02/input.txt
    ${CMAKE_CURRENT_BINARY_DIR}/input.txt)
