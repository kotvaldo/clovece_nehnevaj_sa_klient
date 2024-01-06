cmake_minimum_required(VERSION 3.18.4)
project(CloveceNezlobSa)

set(CMAKE_CXX_STANDARD 17)

add_executable(CloveceNezlobSa client.cpp
        sockets/client_socket.cpp
        sockets/client_socket.h
)

target_link_libraries(CloveceNezlobSa ws2_32)