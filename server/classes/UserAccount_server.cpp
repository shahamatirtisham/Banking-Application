#include <iostream>
#include <ctime>
#include "UserAccount_server.hpp"
#include "../../shared/classes/Hexadecimal.hpp"
#include "../../shared/classes/Packet.hpp"
#include "BankData.hpp"
using namespace std;

UserAccount_server::UserAccount_server(UserAccount userAccount)
: UserAccount(userAccount), clientID(Hexadecimal(0)), salt("") {}

UserAccount_server::UserAccount_server(Hexadecimal clientID, UserAccount userAccount, std::string salt) 
    : clientID(clientID), UserAccount(userAccount), salt(salt) {}

UserAccount_server::UserAccount_server()
: UserAccount(UserAccount()), clientID(Hexadecimal(0)), salt("") {}

void UserAccount_server::setClientID(Hexadecimal clientID)
{
    this->clientID = clientID;
}
void UserAccount_server::setSalt(string salt)
{
    this->salt = salt;
}
Hexadecimal UserAccount_server::getClientID() const
{
    return clientID;
}
std::string UserAccount_server::getSalt() const
{
    return salt;
}
Hexadecimal UserAccount_server::generateClientID() const
{
    BankData data;
    int accIndex = data.getAccountsCreated() + 1;
    data.createdUser();
    Hexadecimal ID(accIndex);
    return ID;
}
string UserAccount_server::generateSalt() const
{
    string characters;
    for (int i = 0; i < 26; i++)
    {
        characters.push_back('A' + i);
    } 
        
    for (int i = 0; i < 26; i++)
    {
        characters.push_back('a' + i);
    } 

    string salt = "";
    srand(time(NULL));
    for(int i = 0; i < 16; i++)
    {
        int randIndex = rand() % (26*2);
        salt += characters[randIndex];
    }
    return salt;
}
void UserAccount_server::printAccountInfo(bool printAsterisk) const
{
    cout << "--------------Displaying Server Account--------------\n";
    cout << "Client ID: " << clientID << endl;
    UserAccount::printAccountInfo(printAsterisk);
    cout << "Salt:" << salt << endl;
    cout << "-----------------------------------------------------\n";
}
UserAccount_server& UserAccount_server::operator = (const Packet& packet)
{
    UserAccount::operator=(packet);
    this->clientID = packet.getID();
    this->salt = packet.getSalt();
    return *this;
}