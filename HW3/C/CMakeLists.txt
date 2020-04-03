cmake_minimum_required(VERSION 3.15)
project(C C)

set(CMAKE_C_STANDARD 11)

add_executable(C main.c)

target_link_libraries(${PROJECT_NAME} pthread)