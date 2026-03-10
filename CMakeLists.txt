cmake_minimum_required(VERSION 3.10)
project(SocketBanking)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- libpq (PostgreSQL client) ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQ REQUIRED IMPORTED_TARGET libpq)

# ---- SERVER executable ----
add_executable(server
    server/main.cpp
    server/classes/Server.cpp
    shared/hash.cpp
    shared/input.cpp
    shared/classes/UserAccount.cpp
    shared/classes/Packet.cpp
    shared/classes/Account.cpp
    shared/classes/Date.cpp
    shared/classes/Hexadecimal.cpp
    server/requests/executeRequests.cpp
    server/functions/database-handling.cpp
    server/classes/UserAccount_server.cpp
    server/classes/BankData.cpp
    server/functions/logger.cpp
    shared/classes/TransactionRecord.cpp
)

target_include_directories(server PRIVATE
    ${PROJECT_SOURCE_DIR}/server
    ${PROJECT_SOURCE_DIR}/shared
    ${LIBPQ_INCLUDE_DIRS}
)

target_link_directories(server PRIVATE ${LIBPQ_LIBRARY_DIRS})
target_link_libraries(server PRIVATE crypto PkgConfig::LIBPQ)
target_compile_options(server PRIVATE ${LIBPQ_CFLAGS_OTHER})

# ---- CLIENT executable ----
add_executable(client
    client/main.cpp
    client/classes/client.cpp
    client/screens/pre-login.cpp
    client/classes/user-operations.cpp
	client/screens/HomePage.cpp
	client/screens/homepage-screen.cpp
	client/classes/TextBox.cpp
    shared/hash.cpp
    shared/input.cpp
    shared/classes/UserAccount.cpp
    shared/classes/Packet.cpp
    shared/classes/Account.cpp
    shared/classes/Date.cpp
    shared/classes/Hexadecimal.cpp
    shared/classes/Colors.cpp 
)

target_include_directories(client PRIVATE
    ${PROJECT_SOURCE_DIR}/client
    ${PROJECT_SOURCE_DIR}/shared
)

target_link_libraries(client PRIVATE crypto)
