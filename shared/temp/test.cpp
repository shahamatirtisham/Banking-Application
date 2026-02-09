#include "../input.hpp"
#include "../classes/Date.hpp"
#include <iostream>
#include <cstdlib>
using namespace std;












Date::Date(int d, int m, int y)
{
    setDate(d);
    setMonth(m);
    setYear(y);
    
}
Date::Date()
{
    setDate(15);
    setMonth(1);
    setYear(2004);
}

void Date::setDate(int date)
{        
        this->date = date;
}
void Date::setMonth(int month)
{
    this->month = month;
}
void Date::setYear(int year)
{
    this->year = year;
}
int Date::getDate() const
{
    return date;
}
int Date::Date::getMonth() const
{
    return month;
}
int Date::getYear() const
{
    return year;
}
bool check_valid_date(const Date& _date)
{
    int date = _date.getDate();
    int month = _date.getMonth();
    int year = _date.getYear();

    int max_date;

    bool leap_year = false;

    if((year % 400) == 0)
        leap_year = true;
    if((year % 4) == 0 && (year % 100) != 0)
        leap_year = true;

    switch (month)
    {
        case  4: 
        case  6:
        case  9:
        case 11: max_date = 30;
                 break;

        case  2: max_date = 28;
                 break;

        default: max_date = 31;
    }

    if(leap_year && month == 2)
        max_date = 29;

    if(date <= 0 || date > max_date)
        return false;

    if(month <= 0 || month > 12)
        return false;

    return true;  
}
int Date::getAge() const
{
    int age;
    time_t now;
    struct tm *current;
    time(&now); 
    current = localtime(&now);

    int current_year = current->tm_year + 1900;
    int current_month = current->tm_mon + 1;
    int current_day =current->tm_mday;


    if(current_month < month)
    {
        age = current_year - year - 1;
    }
    else if(current_month == month && current_day <= date)
    {
        age = current_year - year - 1;
    }
    else
    {
        age = current_year - year;
    }

    return year;    
}
void Date::display() const
{
    cout << date << "-" << month << "-" << year;
}
Date stringToDate(const string &str_date)
{
    int birth_date = 0, birth_month = 0, birth_year = 0;
    int multiplier = 10;
    for (int i = 0; i <= 1; i++)
    {
        birth_date += (str_date[i] - '0') * multiplier;
        multiplier /= 10;
    }
    multiplier = 10;
    for (int i = 3; i <= 4; i++)
    {
        birth_month += (str_date[i] - '0') * multiplier;
        multiplier /= 10;
    }
    multiplier = 1000;
    for (int i = 6; i <= 9; i++)
    {
        birth_year += (str_date[i] - '0') * multiplier;
        multiplier /= 10;
    }
    return Date(birth_date, birth_month, birth_year);
}

bool checkValidDOB(const string& dob)
{
    if(dob.empty())
    {
        cout << "Field cannot be empty\n";
        return false;
    }
        
        if(dob.size() != 10)
        {
             
            cout << "Invalid format. Size /= 10\n";
            return false;
        }
        if(dob[2] != '-' || dob[5] != '-')
        {
             

            printf("Invalid format. Dashes not at appropriate positions\n");
            return false;
        }

        for(int i = 0; i < 10; i++)
        {

            if(dob[i] - '\0' == '\t' || dob[i] - '\0' == '\n' || dob[i] - '\0' == '\0' || dob[i] - '\0' == '\b')
            {
                 
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

        if(check_valid_date(DOB) == false)
        {
             
            printf("Invalid date/month.\n");
            return false;
        }
        else if (DOB.getAge() < 18)
        {
            
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
        getline(cin, str_dob);
        cout << str_dob.size() << endl;

        for(int i = 0; i < str_dob.size(); i++)
        {
            cout << str_dob[i] << " ";
        }
        cout << endl;

        if(checkValidDOB(str_dob) == false)
        {
             
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
        cin >> balance;
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
        cin.ignore();
        getline(cin, favAni);
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
        cin.ignore();
        getline(cin, name);
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
            
            cout << "Username cannot contain spaces\n";
            return false;
        }
        
            

        if(isdigit(username[0]))
        {
             
            cout << "Username cannot start with a digit.\n";
            return false;
        }
        else if(username.size() > 20)
        {
             
            cout << "Username cannot be over 20 characters.\n";
            return false;
        }
        
        for(int i = 0; i < username.size(); i++)
        {
            if(!isdigit(username[i]) && !isalpha(username[i]))
            {
                 
                printf("Username cannot contain special characters or spaces.\n");
                return false;
            } 
            else if(isupper(username[i])) 
            {
                 
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
             
            printf("Password cannot be over 20 characters.\n");
            return false;
        }
        if(password.size() < 7)
        {
             
            printf("Password needs to be atleast 8 characters long.\n");
            return false;
        }

        if(password[0] == ' ' || password[password.size() - 1] == ' ')
        {
             
            printf("Password cannot contain spaces in the beginning or end.\n");
            return false;
        }

        if(password.find_first_of("\t") != string::npos ||
           password.find_first_of("\n") != string::npos ||
           password.find_first_of("\b") != string::npos ||
           password.find_first_of("\0") != string::npos)
        {
             
            printf("Password cannot contain control characters.\n");
            return false;       
        }
    
    return true;
}
string acceptUsername()
{
    string username; 
    while(1)
    {
        cout << "Enter username: ";
        cin.ignore();
        getline(cin, username);
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
        cout << "Enter password: ";
        cin.ignore();
        getline(cin, password);
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

int main()
{
    string str3, str4;
    cin.ignore();
    getline(cin, str3);
    cin.ignore();
    getline(cin, str4);
    string str1;
    cin >> str1;
    string str2;
    cin >> str2;

    cout << "str1: "<< str1 << "\nstr2: "<< str2 << "\nstr3: "<< str3 << "\nstr4: "<< str4;


    return 0;
}
