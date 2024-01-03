cmake_minimum_required(VERSION 3.21)
project(CloveceNezlobSa)

set(CMAKE_CXX_STANDARD 17)

add_executable(CloveceNezlobSa
        soc_server/passive_socket.cpp
        soc_server/passive_socket.h
        soc_server/active_socket.cpp
        soc_server/active_socket.h
        soc_client/client_socket.cpp
        soc_client/client_socket.h
        main.cpp)

target_link_libraries(CloveceNezlobSa ws2_32)