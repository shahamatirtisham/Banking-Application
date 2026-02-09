cmake_minimum_required(VERSION 3.10)
project(SocketBanking)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- SERVER executable ----
add_executable(server
    server/main.cpp
    server/classes/server.cpp
    shared/hash.cpp
    shared/input.cpp
    shared/classes/UserAccount.cpp
    shared/classes/Packet.cpp
    shared/classes/Account.cpp
    shared/classes/Date.cpp
    server/requests/readRequests.cpp
    server/functions/database-handling.cpp
)

target_include_directories(server PRIVATE
    ${PROJECT_SOURCE_DIR}/server
    ${PROJECT_SOURCE_DIR}/shared
)

# ---- CLIENT executable ----
add_executable(client
    client/main.cpp
    client/classes/client.cpp
    client/screens/pre-login.cpp
    shared/hash.cpp
    shared/input.cpp
    shared/classes/UserAccount.cpp
    shared/classes/Packet.cpp
    shared/classes/Account.cpp
    shared/classes/Date.cpp
)

target_include_directories(client PRIVATE
    ${PROJECT_SOURCE_DIR}/client
    ${PROJECT_SOURCE_DIR}/shared
)

target_link_libraries(server PRIVATE crypto)
target_link_libraries(client PRIVATE crypto)
