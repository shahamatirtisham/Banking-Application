#include "input.hpp"
// #include "classes/Date.hpp"
#include <iostream>
#include <cstdlib>
#include "hash.hpp"
using namespace std;




int readInt()
{
    while (true)
    {
        std::string line;
        std::getline(std::cin, line);

        try
        {
            // pos tells you how many characters were successfully changed to number
            size_t pos;
            int value = std::stoi(line, &pos);

            // ensure whole line was a number
            if (pos == line.size())
                return value;
        }
        catch (...)
        {

        }

        std::cout << "Invalid integer, try again: ";
    }
}
double readDouble()
{
    while (true)
    {
        std::string line;
        std::getline(std::cin, line);

        try
        {
            size_t pos;
            double value = std::stoi(line, &pos);

            // ensure whole line was a number
            if (pos == line.size())
                return value;
        }
        catch (...)
        {
            // fall through
        }

        std::cout << "Invalid integer, try again: ";
    }
}
char readChar()
{
    while (true)
    {
        std::string line;
        std::getline(std::cin, line);

        if(line.size() == 1)
        {
            return line[0];
        }

        std::cout << "Invalid input\n";
    }
}

bool checkValidDOB(const string& dob)
{
    if(dob.empty())
            return false;
        
        if(dob.size() != 10)
        {
            clearScreen(); 
            cout << "Invalid format. Size /= 10. Size: " << dob.size() << endl;
            return false;
        }
        if(dob[2] != '-' || dob[5] != '-')
        {
            clearScreen(); 

            cout << "Invalid format.\n";
            return false;
        }

        for(int i = 0; i < 10; i++)
        {

            if(dob[i] - '\0' == '\t' || dob[i] - '\0' == '\n' || dob[i] - '\0' == '\0' || dob[i] - '\0' == '\b')
            {
                clearScreen(); 
                cout << "Date of birth cannot contain control characters.\n";
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

        if(check_valid_date(DOB) == false)
        {
            clearScreen(); 
            cout << "Invalid date/month.\n";
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
        // cout << "Set date of birth (DD-MM-YYYY): ";
        getline(cin, str_dob);
        //cin.ignore();

        if(checkValidDOB(str_dob) == false)
        {
            cout << "Invalid date/month.\n";
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

    return DOB;
    
}
double acceptBalance()
{
    double balance;
    while(1)
    {
        balance = readDouble();
        if(balance >= 0)
        {
            break;
        }
    }
    return balance;
}
string acceptFavAni()
{
    string favAni;
    while(1)
    {
        // cout << "Enter favorite animal: ";
        getline(cin, favAni);
        //cin.ignore();
        if(favAni.size() > 30)
        {
            cout << "Animal name cannot be greater than 30 characters\n";
        }
        if(favAni != "")
        {
            break;
        }
    }
    return favAni;
}
string acceptName()
{
    string name;
    while(1)
    {
        // cout << "Enter name: ";
        getline(cin, name);
        //cin.ignore();
        if(name.size() > 30)
        {
            cout << "Name cannot be overe 30 characters long\n";
        }
        if(name != "")
            break;
    }
    return name;
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
                cout << "Username cannot contain special characters or spaces.\n";
                return false;
            } 
            else if(isupper(username[i])) 
            {
                clearScreen(); 
                cout << "Username cannot contain uppercase characters.\n";
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
            //clearScreen(); 
            cout << "Password cannot be over 20 characters.\n";
            return false;
        }
        if(password.size() < 7)
        {
            //clearScreen(); 
            cout << "Password needs to be atleast 8 characters long.\n";
            return false;
        }

        if(password[0] == ' ' || password[password.size() - 1] == ' ')
        {
            clearScreen(); 
            cout << "Password cannot contain spaces in the beginning or end.\n";
            return false;
        }

        if(password.find_first_of("\t") != string::npos ||
           password.find_first_of("\n") != string::npos ||
           password.find_first_of("\b") != string::npos ||
           password.find_first_of("\0") != string::npos)
        {
            clearScreen(); 
            cout << "Password cannot contain control characters.\n";
            return false;       
        }
    
    return true;
}
string acceptUsername()
{
    string username; 
    while(1)
    {
        // cout << "Enter username: ";
        getline(cin, username);
        //cin.ignore();
        if(checkValidUsername(username) == true)
        { 
            break;
        }    
        
    }  
    return username;

}
string acceptPassword()
{
    string password; 
    while(1)
    {
        // cout << "Enter password: ";

        getline(cin, password);
        //cin.ignore();
        if(checkValidPassword(password) == true)
        {
            break;
        }    
    }  
    return password;
}
void clearScreen()
{
    system("clear");
}
void sleep(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
