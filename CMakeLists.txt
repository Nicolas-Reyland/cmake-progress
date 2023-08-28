cmake_minimum_required(VERSION 3.25)
project(cmake_progress C)

set(CMAKE_C_STANDARD 11)

add_executable(cmake_progress
        src/main.c)
target_compile_options(cmake_progress PUBLIC -Wall -Wextra -Werror)
target_compile_definitions(cmake_progress PUBLIC CMAKE_DEBUG)
