#pragma once
#include "../shared/classes/UserAccount.hpp"
#include "../client/classes/user-operations.hpp"

class HomePage
{
private:
    int sockfd;
    UserAccount user;
    UserOperations operation;
    bool printBal;
    void printHeader();
    string notice;
    void printMenu();
public:
    HomePage(int sockfd, UserAccount user);
    void display();
    int getChoice();    
    void userOperation(int choice);
};