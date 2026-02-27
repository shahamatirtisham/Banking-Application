#pragma once
#include <iostream>
#include "../classes/UserAccount_server.hpp"


bool checkUniqueUsername(std::string username);
void addUser(UserAccount user);
bool checkUserPassword(string username, string password);
UserAccount_server getUserByUsername(string username);
