#pragma once
#include "../../shared/classes/UserAccount.hpp"
#include "../../shared/classes/Hexadecimal.hpp"
#include <iostream>

class UserAccount_server : public UserAccount
{
private:
    Hexadecimal clientID;
    std::string salt; 
public:
    UserAccount_server(UserAccount userAccount);
    UserAccount_server();
    void setClientID(Hexadecimal clientID);
    void setSalt(std::string salt);
    Hexadecimal getClientID() const;
    std::string getSalt() const;
    Hexadecimal generateClientID() const;
    std::string generateSalt() const;
    void printAccountInfo(bool printAsterisk=1) const;
    UserAccount_server& operator = (const Packet& packet);
};