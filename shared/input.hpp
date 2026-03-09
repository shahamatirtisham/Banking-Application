#pragma once
#include <string>
#include "classes/Date.hpp"
#include <thread>
#include <chrono>


int readInt();
double readDouble();
char readChar();

bool checkValidDOB(const std::string &dob);
Date acceptDOB();
double acceptBalance();
std::string acceptFavAni();
std::string acceptName();
bool checkValidUsername(const std::string &username);
bool checkValidPassword(const std::string &password);
std::string acceptUsername();
std::string acceptPassword();

void clearScreen();
void sleep(int ms);