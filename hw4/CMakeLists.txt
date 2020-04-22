cmake_minimum_required(VERSION 3.15)
project(pageReplacement)

set(CMAKE_CXX_STANDARD 14)

add_executable(pageReplacement main.c queue.c queue.h clock.c clock.h lru.c lru.h)