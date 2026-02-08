#pragma once
#include <iostream>
#include "Date.hpp"
using namespace std;

bool checkValidDOB(const string& dob)
{
    if(dob.empty())
            return false;
        
        if(dob.size() != 10)
        {
            clearScreen(); 
            printf("Invalid format.\n");
            return false;
        }
        if(dob[2] != '-' || dob[5] != '-')
        {
            clearScreen(); 

            printf("Invalid format.\n");
            return false;
        }

        for(int i = 0; i < 10; i++)
        {

            if(dob[i] - '\0' == '\t' || dob[i] - '\0' == '\n' || dob[i] - '\0' == '\0' || dob[i] - '\0' == '\b')
            {
                clearScreen(); 
                printf("Date of birth cannot contain control characters.\n");
                return false;
            }            
        }

        int birth_date = 0, birth_month = 0, birth_year = 0;
        int age;

        int multiplier = 10;
        for(int i = 0; i <= 1; i++)
        {
            birth_date += (dob[i] - '0') * multiplier;
            multiplier /= 10;
        }

        multiplier = 10;
        for(int i = 3; i <= 4; i++)
        {
            birth_month += (dob[i] - '0') * multiplier;
            multiplier /= 10;
        }
        
        multiplier = 1000;
        for(int i = 6; i <= 9; i++)
        {
            birth_year += (dob[i] - '0') * multiplier;
            multiplier /= 10;
        }

        Date DOB(birth_date, birth_month, birth_year);

        if(DOB.check_valid_date() == false)
        {
            clearScreen(); 
            printf("Invalid date/month.\n");
            return false;
        }
        else if (DOB.getAge() < 18)
        {
            clearScreen();
            cout << "You must be over 18 years old\n";
            return false;
        }
             
    
    return true;
}

Date acceptDOB()
{
    string str_dob;
    while(1)
    {
        cout << "Set date of birth (DD-MM-YYYY): ";
        cin.ignore();
        getline(cin, str_dob);

        if(checkValidDOB(str_dob) == false)
        {
            clearScreen(); 
            printf("Invalid date/month.\n");
            continue;
        }    
        else
        {
            break;
        }           
    }    
    Date DOB = stringToDate(str_dob);
    if (DOB.getAge() < 18)
    {
        clearScreen();
        cout << "You must be over 18 years old\n";
        acceptDOB();
    } 
    else
    {
        return DOB;
    }
}
double acceptBalance()
{
    while(1)
    {
        double balance;
        cin >> balance;
        if(balance >= 0)
        {
            break;
        }
        else
        {
            return balance;
        }

    }
}
string acceptFavAni()
{
    while(1)
    {
        string favAni;
        cin.ignore();
        getline(cin, favAni);
        if(favAni != "")
            return favAni;
    }
}
string acceptName()
{
    while(1)
    {
        string name;
        cin.ignore();
        getline(cin, name);
        if(name.size() > 30)
        {
            cout << "Name cannot be overe 30 characters long\n";
        }
        if(name != "")
            return name;
    }
}
bool checkValidUsername(const string& username)
{
    bool valid_username = false;

        if(username.empty())
            return false;

        if(username.find_first_of(" ") != string::npos)
        {
            clearScreen();
            cout << "Username cannot contain spaces\n";
            return false;
        }
        
            

        if(isdigit(username[0]))
        {
            clearScreen(); 
            cout << "Username cannot start with a digit.\n";
            return false;
        }
        else if(username.size() > 20)
        {
            clearScreen(); 
            cout << "Username cannot be over 20 characters.\n";
            return false;
        }
        
        for(int i = 0; i < username.size(); i++)
        {
            if(!isdigit(username[i]) && !isalpha(username[i]))
            {
                clearScreen(); 
                printf("Username cannot contain special characters or spaces.\n");
                return false;
            } 
            else if(isupper(username[i])) 
            {
                clearScreen(); 
                printf("Username cannot contain uppercase characters.\n");
                return false;
            }       
        }
            valid_username = true;      
    
    return true;

}
bool checkValidPassword(const string& password)
{
    bool valid_password = false;
    
        if(password.empty())
            return false;
        
        if(password.size() > 20)
        {
            clearScreen(); 
            printf("Password cannot be over 20 characters.\n");
            return false;
        }
        if(password.size() < 7)
        {
            clearScreen(); 
            printf("Password needs to be atleast 8 characters long.\n");
            return false;
        }

        if(password[0] == ' ' || password[password.size() - 1] == ' ')
        {
            clearScreen(); 
            printf("Password cannot contain spaces in the beginning or end.\n");
            return false;
        }

        if(password.find_first_of("\t") != string::npos ||
           password.find_first_of("\n") != string::npos ||
           password.find_first_of("\b") != string::npos ||
           password.find_first_of("\0") != string::npos)
        {
            clearScreen(); 
            printf("Password cannot contain control characters.\n");
            return false;       
        }
    
    return true;
}
void acceptUsername()
{
    while(1)
    {
        string username; 
        cout << "Enter username: ";
        cin.ignore();
        getline(cin, username);
        if(checkValidUsername(username) == true)
        {
            this->username = username;
            break;
        }    
        
    }  
}
void acceptPassword()
{

}
