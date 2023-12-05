add_executable(day05 main.cpp)

target_link_libraries(day05 PRIVATE common)

add_custom_command(
    TARGET day05 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/day05/input.txt
    ${CMAKE_CURRENT_BINARY_DIR}/input.txt)
