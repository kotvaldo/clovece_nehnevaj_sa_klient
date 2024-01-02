cmake_minimum_required(VERSION 3.21)
project(CloveceNezlobSa)

set(CMAKE_CXX_STANDARD 17)

add_executable(CloveceNezlobSa main.cpp)

target_link_libraries(CloveceNezlobSa ws2_32)