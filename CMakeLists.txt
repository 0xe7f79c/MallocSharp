cmake_minimum_required(VERSION 3.10)

project(MallocSharp)

set(CMAKE_CXX_STANDARD 23)

find_package(dpp CONFIG REQUIRED)

add_executable(MallocSharp src/mallocsharp.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE dpp::dpp)