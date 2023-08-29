cmake_minimum_required(VERSION 3.25)
project(cmake_progress C)

set(CMAKE_C_STANDARD 11)

add_executable(cmake_progress
        src/main.c src/progressbar.c src/re.c)
target_include_directories(cmake_progress PUBLIC src)
target_compile_options(cmake_progress PUBLIC -Wall -Wextra -Werror)
#target_compile_definitions(cmake_progress PUBLIC CMAKE_DEBUG)
#target_compile_definitions(cmake_progress PUBLIC CMAKE_SLEEP_AFTER_DRAW)

add_executable(cmake_progress_test
        tests/progressbar.c src/progressbar.c src/re.c)
target_include_directories(cmake_progress_test PUBLIC src)
target_link_options(cmake_progress_test PUBLIC -lcriterion)
target_compile_options(cmake_progress_test PUBLIC -Wall -Wextra -Werror)

add_custom_command(TARGET cmake_progress_test POST_BUILD
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/cmake_progress_test
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running the test suite..."
)

install(TARGETS cmake_progress DESTINATION bin)
