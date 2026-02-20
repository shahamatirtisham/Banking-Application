#include "executeRequests.hpp"
#include "../classes/UserAccount_server.hpp"
#include <iostream>
#include <string>
using namespace std;

static int sockfd = -1;

void readRequests_init(int passed_sockfd)
{
    sockfd = passed_sockfd;
}

void requests::user::login(Packet packet)
{

}

void requests::user::signup(Packet packet)
{

    UserAccount_server acc;
    acc = packet;        // overloaded the UserAccount class to support ' = ' operations with Packet class
    acc.printAccountInfo(0);
    acc.setAccountNo(generateAccountNo());
    cout << "generated acc no \n";
    acc.setClientID(acc.generateClientID());
    acc.printAccountInfo(0);


    // enter dbms signup code here

}