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
    void check_balance(Packet packet);
    void deposit(Packet packet);
    void withdraw(Packet packet);
    void transfer_money(Packet packet);
    void transaction_history(Packet packet);
    void logout(Packet packet);
} 
namespace requests::admin
{
    void login(Packet packet);
    void signup(Packet packet);
} 