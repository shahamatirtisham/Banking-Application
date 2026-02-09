#pragma once
#include <iostream>
// #include <string>
#include "../functions/database-handling.hpp"
#include "../../shared/classes/Packet.hpp"

void readRequests_init(int passed_sockfd);

namespace requests::user
{
    void login(Packet packet);
    void signup(Packet packet);
} 
namespace requests::admin
{
    void login(Packet packet);
    void signup(Packet packet);
} 