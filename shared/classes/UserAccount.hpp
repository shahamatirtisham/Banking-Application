#include <iostream>
#include <cctype>
#include <ctime>
#include "Date.cpp"
#include "Account.cpp"
#include "clearScreen.hpp"
using namespace std;

class UserAccount : public Account
{
private:
    Date DOB;
    double balance;
    string accountNo;
    string favAni;

public:
    UserAccount() : Account()
    {
        setDOB();
        setBalance(0);
        generateAccountNo();
        setFavAni();
    }
    void setDOB();
    void setBalance(float balance);
    void generateAccountNo();
    void setFavAni();
    Date getDOB();
    double getBalance();
    string getAccountNo();
    string getFavAni(); 
    void printAccountInfo(bool printAsterik);

};
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

void UserAccount::setDOB()
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
        UserAccount::setDOB();
    } 
    else
    {
        this->DOB = DOB;
    }   
    
}    

void UserAccount::setBalance(float balance=0)
{
    this->balance = balance;
}    
bool checkUniqueAccountNo(const string& account_no)
{
    // input dbms code
    return true;
}
void UserAccount::generateAccountNo()
{
    LABEL01:
    string accountNo = "";
    srand(time(NULL));
    
    for(int i = 0; i < 13; i++)
    {
        int random_no = rand() % 10;
        accountNo += random_no + '0';
    }
    accountNo[13] = '\0';

    if(checkUniqueAccountNo(accountNo) == false)
        goto LABEL01;

}    
void UserAccount::setFavAni()
{
    while(1)
    {
        string favAni;
        cin.ignore();
        getline(cin, favAni);
        if(favAni != "")
        break;
    }
    this->favAni = favAni;
}  
Date UserAccount::getDOB()
{
    return DOB;
}
double UserAccount::getBalance()
{
    return balance;
}
string UserAccount::getAccountNo()
{
    return accountNo;
}
string UserAccount::getFavAni()
{
    return favAni;
}
void UserAccount::printAccountInfo(bool printAsterik=0)
{
    cout << "Name: " << name << endl;
    if(printAsterik)
    {
        cout << "Username: ***\n";
        cout << "Password: ***\n";
    }
    cout << "Date of Birth: ";
    DOB.display(); cout << endl;
    cout << "Balance: " << balance;
    cout << "Account Number: " << accountNo << endl;
    if(printAsterik)
    {
        cout << "Favorite Animal: ***\n";
    }
}


