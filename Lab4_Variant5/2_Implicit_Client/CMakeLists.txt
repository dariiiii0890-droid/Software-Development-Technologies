cmake_minimum_required(VERSION 3.15)
project(ImplicitClient)

set(CMAKE_CXX_STANDARD 17)

add_executable(ImplicitClient main.cpp)

target_include_directories(ImplicitClient PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/EigenLib/include")

target_link_libraries(ImplicitClient PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/EigenLib/lib/EigenLib.lib")