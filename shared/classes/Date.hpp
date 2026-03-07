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
    Date(const std::string& date_str);
    Date();

    void setDate(int d);
    void setMonth(int m);
    void setYear(int y);

    int getDate() const;
    std::string getDate_string() const;
    int getMonth() const;
    int getYear() const;
    int getAge() const;
    void display() const;
};

Date stringToDate(const std::string& str_date);
bool check_valid_date(const Date& _date);
