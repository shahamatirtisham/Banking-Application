#include <iostream>
#include "main_menu.h"
using namespace std;

void MainMenu::printHeader()
{
    cout <<"====================================================" <<endl;
    cout <<"              WELCOME TO ASITA BANK" <<endl;
    cout <<"====================================================" <<endl <<endl;
}

void MainMenu::printMenu()
{
    cout <<"1. Login" <<endl;
    cout <<"2. Sign Up" <<endl;
    cout <<"3. Exit" <<endl <<endl;
}

int MainMenu::readChoice()
{
    int choice; 
    while(true)
    {
        cout <<"Enter Choice: "; cin >>choice;
        if(cin.fail() || choice <1 || choice >3)
        {
            cin.clear();
            cin.ignore(9999, '\n');
            cout <<"Invalid Input. Try Again." <<endl <<endl;
        } 
        else return choice;
    }
}

void MainMenu::display()
{
    system("clear");
    printHeader();
    printMenu();
}

int MainMenu::getChoice()
{
    return readChoice();
}