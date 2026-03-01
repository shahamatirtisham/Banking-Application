#pragma once
#include <iostream>
// #include <string>
#include "../functions/database-handling.hpp"
#include "../../shared/classes/Packet.hpp"

void exeRequests_init(int passed_sockfd, PGconn* connection);

namespace requests::user
{
    void login(Packet packet);
    void signup(Packet packet);
    void forgotPassword(Packet packet);
    bool checkUniqueUsername(string username);
} 
namespace requests::admin
{
    void login(Packet packet);
    void signup(Packet packet);
} 