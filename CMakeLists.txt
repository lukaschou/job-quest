cmake_minimum_required(VERSION 3.16)
project(JobQuest)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -std=c99)

file(GLOB SOURCES "src/*.c" "game.c") 
add_executable(jobquest ${SOURCES})

find_package(Curses REQUIRED)
target_include_directories(jobquest PRIVATE ${CURSES_INCLUDE_DIRS} include)
target_link_libraries(jobquest ${CURSES_LIBRARIES})
