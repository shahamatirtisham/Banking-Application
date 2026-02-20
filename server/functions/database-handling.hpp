#pragma once
#include <iostream>
#include "../../shared/classes/UserAccount.hpp"
#include "../classes/UserAccount_server.hpp"



bool checkUniqueUsername(std::string username);
bool checkPassword(std::string username, std::string password);
UserAccount_server getUserByUsername(std::string username);
void addUser(UserAccount user);
