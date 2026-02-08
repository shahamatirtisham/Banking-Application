#pragma once
#include <iostream>
#include "classes/Date.hpp"
using namespace std;

bool checkValidDOB(const string &dob);
Date acceptDOB();
double acceptBalance();
string acceptFavAni();
string acceptName();
bool checkValidUsername(const string &username);
bool checkValidPassword(const string &password);
string acceptUsername();
string acceptPassword();

Date stringToDate(const string &str_date);
void clearScreen();