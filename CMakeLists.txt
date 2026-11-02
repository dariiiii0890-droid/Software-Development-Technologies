
cmake_minimum_required(VERSION 3.20)

project(Lab18_Optimization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(OptimizationTask STATIC
    src/OptimizationTask.cpp
    src/ScopeLogger.cpp
)

target_include_directories(OptimizationTask PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(OptimizationDemo
    demo/main.cpp
)

target_link_libraries(OptimizationDemo PRIVATE OptimizationTask)

add_executable(ExceptionDemo
    exceptions_demo/main.cpp
)

include(CTest)

if(BUILD_TESTING)
    find_package(GTest CONFIG REQUIRED)

    add_executable(OptimizationTests
        tests/OptimizationTests.cpp
    )

    target_link_libraries(OptimizationTests PRIVATE
        OptimizationTask
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(OptimizationTests)
endif()
