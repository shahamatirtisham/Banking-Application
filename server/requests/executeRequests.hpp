#pragma once
#include <iostream>
// #include <string>
#include "../functions/database-handling.hpp"
#include "../../shared/classes/Packet.hpp"

void exeRequests_init(int passed_sockfd, PGconn* connection);

namespace requests::user
{
    void login(const Packet& packet);
    void signup(const Packet& packet);
    void forgotPassword(const Packet& packet);
    void changePassword(const Packet& packet);
    bool checkUniqueUsername(const string& username);
    void check_balance(const Packet& packet);
    void deposit(const Packet& packet);
    void withdraw(const Packet& packet);
    void transfer_money(const Packet& packet);
    void transaction_history(const Packet& packet);
    void logout(const Packet& packet);
    void getAccAndBal(const Packet&p);
} 
namespace requests::admin
{
    void login(const Packet& packet);
    void signup(const Packet& packet);
} 