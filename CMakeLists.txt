cmake_minimum_required(VERSION 3.18.4)
project(CloveceNezlobSa)

set(CMAKE_CXX_STANDARD 17)

add_executable(CloveceNezlobSa
        main.cpp
        soc_server/server.cpp
        soc_client/client.cpp
)

