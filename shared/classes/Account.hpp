#include <iostream>
#include <cctype>
#include "clearScreen.hpp"
using namespace std;

class Account
{
protected:
    string name;
    string username;
    string password;
    
public:
    Account()
    {
        setName();
        setUsername();
        setPassword();   
    }
    void setName();
    void setUsername();
    void setPassword();
    string getUsername() const;
    string getPassword() const;
    string getName() const;
    virtual void printAccountInfo() = 0;
    
};    


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
void Account::setName()
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
            break;
    }
    this->name = name;
} 
void Account::setUsername()
{
    while(1)
    {
        string username; 
        cout << "Set username: ";
        cin.ignore();
        getline(cin, username);
        if(checkValidUsername(username) == true)
        {
            this->username = username;
            break;
        }    
        
    }    
}    
void Account::setPassword()
{
    while(1)
    {
        string password; 
        cout << "Set password: ";
        cin.ignore();
        getline(cin, password);
        if(checkValidPassword(username) == true)
        {
            this->password = password;
            break;
        }    
        
    }    
}  

string Account::getName() const
{
    return name;
}
string Account::getUsername() const
{
    return username;
}
string Account::getPassword() const
{
    return password;
}

