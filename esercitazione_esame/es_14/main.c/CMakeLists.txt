cmake_minimum_required(VERSION 3.28)
project(main_c C)
set(CMAKE_C_STANDARD 11)

add_executable(main_c main.c)

find_package(OpenMP REQUIRED)
if(OpenMP_C_FOUND)
    target_link_libraries(main_c PUBLIC OpenMP::OpenMP_C)
endif()