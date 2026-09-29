cmake_minimum_required(VERSION 3.20)
project(Lab1_GroupProject CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/utf-8 /W4)
endif()

add_executable(
    Lab1App
    main.cpp
    shared_types.h
    student_a.h
    student_a.cpp
    student_b.cpp
)