#include "readRequests.hpp"
#include "../../shared/classes/UserAccount.hpp"
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
    UserAccount acc;
    acc = packet;        // overloaded the UserAccount class to support ' = ' operations with Packet class
    acc.printAccountInfo();
}