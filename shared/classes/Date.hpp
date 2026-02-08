#pragma once
#include <iostream>

class Date
{
private:
    int date;
    int month;
    int year;
public:
    Date(int d, int m, int y);
    Date();
    void setDate(int d);
    void setMonth(int m);
    void setYear(int y);
    int getDate() const;
    int getMonth() const;
    int getYear() const;
    bool check_valid_date() const;
    int getAge() const;
    void display() const;
};
