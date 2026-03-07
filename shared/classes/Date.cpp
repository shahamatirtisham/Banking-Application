#include "Date.hpp"
#include <iostream>
#include <ctime>
#include <sstream>
using namespace std;

Date::Date(int d, int m, int y)
{
    setDate(d);
    setMonth(m);
    setYear(y);
    
}
Date::Date()
{
    setDate(0);
    setMonth(0);
    setYear(0);
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
string Date::getDate_string() const
{
    stringstream ss;
    ss << year << "-" << month << "-" << date;
    return ss.str();
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