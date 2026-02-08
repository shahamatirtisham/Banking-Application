#include "Date.hpp"
#include <iostream>
#include <ctime>
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
bool Date::check_valid_date() const
{
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