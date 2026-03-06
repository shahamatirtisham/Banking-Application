#pragma once
#include "../shared/classes/UserAccount.hpp"
#include "../client/classes/user-operations.hpp"

class HomePage
{
    private:
    int sockfd;
    UserAccount user;
    UserOperations operation;
    void printHeader();
    void printMenu();

    public:
    HomePage(UserAccount user);
    void display();
    int getChoice();    
    void userOperation(int choice);
};