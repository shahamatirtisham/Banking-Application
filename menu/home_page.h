#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <string>

class HomePage 
{
    private:
    std::string name;
    int accountNumber;
    void printHeader();
    void printMenu();
    int readChoice();

    public:
    HomePage(std::string name, int accountNumber);
    void display();
    int getChoice();    
};

#endif