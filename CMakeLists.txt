cmake_minimum_required(VERSION 3.10)
project(SocketBanking)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- SERVER executable ----
add_executable(server
    server/main.cpp
    server/socket/server.cpp
    shared/packet.cpp
)

target_include_directories(server PRIVATE
    ${PROJECT_SOURCE_DIR}/server
    ${PROJECT_SOURCE_DIR}/shared
)

# ---- CLIENT executable ----
add_executable(client
    client/main.cpp
    client/socket/client.cpp
    shared/packet.cpp
)

target_include_directories(client PRIVATE
    ${PROJECT_SOURCE_DIR}/client
    ${PROJECT_SOURCE_DIR}/shared
)
